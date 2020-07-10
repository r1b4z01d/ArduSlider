*ArduSlider
** Arduino:
*** API:
The arduino expects each api call to consist of a letter (flag), set of numbers and a charige return. Example: B1000/n
In Flags: 
	T: Length of session in milliseconds
		T60000/n
	S: Sutter speed in milliseconds 
		S1000/n
	B: Length of time the cammera takes to save the image and clear it's buffer in milliseconds.
		B3000/n
	D: Distance of travel for the session in mm
		D1000/n
	R: Boolen trigger to start/stop the session.
		R1/n
	A: Request a full refresh of all parameters
		A/n 

Out:

  sendData("T",String(msOfCapture));
  sendData("P",String(photo));
  sendData("C",String(numPhotos));
  sendData("R",String(isRunning));
  sendData("S",String(shutterSpeed));
  sendData("B",String(bufferClear));
  sendData("D",String(mmOfTravel));
  sendData("L",String(totalSteps));
