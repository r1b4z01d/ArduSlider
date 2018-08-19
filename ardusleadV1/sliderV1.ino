// defines pins numbers
const int stepPin = 9; 
const int dirPin = 8; 
const int shutterPin = 2;
const int ledPin = 13;
int photo = 1;

int shutterSpeed = 20;
int bufferClear = 3;
int shakeSettle = 1;
int numPhotos = 700;
int photoSteps = 1;

int traversalDirection=1;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(shutterPin,OUTPUT); 
  pinMode(ledPin,OUTPUT); 

  Serial.begin(9600);
  Serial.print("Number of Photos:");
  Serial.println(numPhotos);
  Serial.print("Steps Per Photo:");
  Serial.println(photoSteps);
}

void takePhoto(){
    digitalWrite(shutterPin,HIGH);
    digitalWrite(ledPin,HIGH);  
    delay(200); 
    digitalWrite(shutterPin,LOW); 
    digitalWrite(ledPin,LOW); 
    delay(200);
    Serial.print("Taking Photo Number: "); 
    Serial.println(photo);
}

void moveCamera(){
  Serial.print("Moving the camera ");
  Serial.print(photoSteps);
  Serial.println(" steps.");
    //set motor direction 
    digitalWrite(dirPin,HIGH);
    //move the motor
    for(int steps=0;steps<=photoSteps;steps++){
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(500);
       Serial.print("Moving step:");
       Serial.println(steps);
      delayMicroseconds(1000);
      
    }
    Serial.println("Waiting for the shake to settle.");
    delay(shakeSettle*1000);
}

void loop() {
  while(photo<=numPhotos){
      takePhoto();
      photo++;
      
      //wait for photo to take and save.
      delay((shutterSpeed+bufferClear)*1000);
 
      Serial.print("Tooking Photo Number: "); 
      Serial.println(photo);
      
      //Move Camera
      moveCamera(); 
      delay(shakeSettle * 1000 ); 
      
      }
      
Serial.println("~Done~");  
}
