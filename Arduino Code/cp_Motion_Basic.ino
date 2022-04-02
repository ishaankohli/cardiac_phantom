//----------------------------------- Variables -----------------------------------
// Pins
int feedbackPin = A0; // Potentiometer Reading Pin
int outputPins[3] = {12, 10, 8}; // {Speed_PWM_Pin, DirectionPin_1, DirectionPin_2}

// Positional Feedback
int current;
int current2;
int reread;
uint32_t tStart;
uint32_t tNow;
int bufferVal = 10; //Potentiometer Buffer Value
//--------------------------------- Functions ---------------------------------
void initializePins(){
  pinMode(feedbackPin, INPUT);
  for (int i = 0; i < 3; i++){pinMode(outputPins[i], OUTPUT);}
}
void drive(int Direction, int speedVal){
  // Drive the actuator at the specific speed and direction
  switch(Direction){
    case 1:       //extension
      digitalWrite(outputPins[1], HIGH);
      digitalWrite(outputPins[2], LOW); //Establishes forward direction
      analogWrite(outputPins[0], speedVal);   //PWM
      break;
   
    case 0:       //stopping
      digitalWrite(outputPins[1], LOW);
      digitalWrite(outputPins[2], LOW);  //Eengage the Brake
      break;

    case -1:      //retraction
      digitalWrite(outputPins[1], LOW);
      digitalWrite(outputPins[2], HIGH); //Establishes reverse direction
      analogWrite(outputPins[0], speedVal);   //PWM
      break;
  }
}
void driveToPosition(int goalPosition, int Speed){
  // Move to a Position from the look-up table or potentiometer value
  int currentPosition = cpos();
  if (currentPosition > goalPosition){
    drive(-1, 100); // Move Backwards
    while(cpos() > goalPosition){};
    drive(0, 100); // Stop
  } 
  else if (currentPosition < goalPosition){
    drive(1, 100); // Move Forewards
    while(cpos() < goalPosition){};
    drive(0, 100); // Stop
  } 
  else{}//Do Nothing if goalPosition = currentPosition}
}
int cpos(){
  // Returns current position and discards analog read spikes
  current = analogRead(feedbackPin);
  current2 = analogRead(feedbackPin);
  reread = abs(current - current2);
  while (reread > bufferVal){
    current = analogRead(feedbackPin);
    current2 = analogRead(feedbackPin);
    reread = abs(current - current2);
  }
  return current;
}
