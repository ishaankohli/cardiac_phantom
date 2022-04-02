// Constants:
const int minimumSpeed = 25; //Experimintally Found (Below this speed, the motor stalls)
const int startingPoint = 4; //Experimintally Found from 3D Model
const int stepSize = 50; // Number of samples to take (1 per mm)
int calibrationArray[(stepSize)]; // LookUp table where the index represents the coordinate in mm


//--------------------------------- Functions ---------------------------------
int findLimit(){
  // Find the time it takes to get from the origin to the fully extended position
  driveToPosition(0, 100); // Start at Origin
  int totalTime; // Time to Return
  tStart = millis(); // Start Time
  driveToPosition(1023, 100);
  tNow = millis();
  totalTime = (tNow - tStart);
  return totalTime;
}
void calibrate(){
  //Use the time of full extension to create a look-up table using the potentiometer readings
  //  in order to fix time drift when just using time.
  int extensionTime = findLimit(); //Time it takes for full extension from the origin.  
  driveToPosition(0, 100); // Start at origin
  unsigned int i = 1;
  calibrationArray[0] = analogRead(feedbackPin); // Record Origin
  drive(1, 100);
  while (i <= stepSize){
    static uint32_t tStart = millis();
    const uint32_t period = (extensionTime / stepSize);
    uint32_t tNow = millis();
    if (tNow - tStart >= period){
      tStart += period;
      calibrationArray[i] = analogRead(feedbackPin);
      Serial.println(analogRead(feedbackPin));
      i++;
    }
  }
  drive(0, 100);
}
void startPosition(){
  driveToPosition(calibrationArray[startingPoint], 100);
}
