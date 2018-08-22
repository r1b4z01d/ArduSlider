// defines pins numbers
const int stepPin = 9; 
const int dirPin = 8; 
const int shutterPin = 2;
const int ledPin = 13;

int incomingByte = 0;   // for incoming serial data
// system config
int shutterSpeed = 200;
int bufferClear = 3000;
int shakeSettle = 1000;
int stepsPerMM = 1;
float msOfCapture = 60000;
int mmOfTravel = 8000;
int isRunning = 0;

//derived variables 
int traversalDirection=1;//TODPO
int videoLength = 0;
int stepsPerPhoto = 0;
int photo = 1;
float numPhotos = 0;
int totalSteps = 0;
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(shutterPin,OUTPUT); 
  pinMode(ledPin,OUTPUT); 
  Serial.begin(9600);
  sendDataRefresh();
}

void takePhoto(){
    digitalWrite(shutterPin,HIGH);
    digitalWrite(ledPin,HIGH);  
    delay(200); 
    digitalWrite(shutterPin,LOW); 
    digitalWrite(ledPin,LOW); 
    delay(200);
    //Serial.print("Taking Photo Number: "); 
    //Serial.println(photo);
    sendData("P",String(photo));
}

void moveCamera(){
    //Serial.print("Moving the camera ");
    //Serial.print(stepsPerPhoto);
    //Serial.println(" steps.");
    //set motor direction 
    digitalWrite(dirPin,HIGH);
    //move the motor
    for(int steps=0;steps<=stepsPerPhoto;steps++){
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(500);
      //Serial.print("Moving step:");
      //Serial.println(steps);
      totalSteps++;
      delayMicroseconds(1000);
    }
    //Serial.println("Waiting for the shake to settle.");
    sendData("L",String(totalSteps));
    delay(shakeSettle);
}

void calcParams(){
  numPhotos=msOfCapture/(shutterSpeed+bufferClear+shakeSettle);
  stepsPerPhoto=(mmOfTravel*stepsPerMM)/numPhotos;
  videoLength=numPhotos/24;
}

void parseData(){
  String theData;
  int dataFlag;

  dataFlag = Serial.read();
  while (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    theData.concat(char(incomingByte));
  }

  switch (dataFlag) {
    case 'T':// Time to capture in mins
      msOfCapture = theData.toFloat();
      break;
    case 'S':// Shutter speed in ms
      shutterSpeed = theData.toInt();
      break;
    case 'B':// Time to clear camera buffer in ms
      bufferClear = theData.toInt();
      break;
    case 'D':// Distance to travel in mm
      mmOfTravel = theData.toInt();
      break;
    case 'R':// Distance to travel in mm
      isRunning = theData.toInt();
      photo = 1;
      sendDataRefresh();
      break;
    case 'A':// Send Data Refresh
      sendDataRefresh();
      break;

  }
        sendDataRefresh();

}

void sendData(String flag, String data){
  Serial.println(flag+data);
}

void sendDataRefresh(){
  sendData("T",String(msOfCapture).substring(0,String(msOfCapture).indexOf('.')));
  sendData("P",String(photo));
  sendData("C",String(numPhotos).substring(0,String(numPhotos).indexOf('.')));
  sendData("R",String(isRunning));
  sendData("S",String(shutterSpeed));
  sendData("B",String(bufferClear));
  sendData("D",String(mmOfTravel));
  sendData("L",String(totalSteps));
}
  
void loop() {
  delay(500);
  if(Serial.available() > 0){//data in buffer
    parseData();//parse incoming data 
  }
  calcParams();  
  while(photo<=numPhotos&&Serial.available()==0&&isRunning==1){       
    takePhoto();//take photo
    delay(shutterSpeed+bufferClear);//wait for photo to take and save.
    photo++;//increment current photo take
    moveCamera();//Move Camera
    delay(shakeSettle);//wait for shaking to stop   
  }
}
