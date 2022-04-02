
//--------------------------------- Data ---------------------------------------
// Values from "Cardiac and Respiratory Motion"
// Position:
int pos[20] = {0,3,5,7,10,11,13,14,14,13,12,10,9,8,7,7,6,5,3,0};
// Velocity:
int deriv[20] = {182,255,224,213,148,110,70,20,-34,-146,-165,-160,-75,-49,-20,-65,-209,-252,-212,-86};

//--------------------------------- Functions ---------------------------------
// Note that for testing purposes, the variables "Amplitude" and "Speed" = 1
void oscillateLoop(int start, int finish){
  for (int i = start; i< finish; i++){
    int currentSpeed = deriv[i] / Speed;
    if (currentSpeed > 0){
      while(cpos() <= calibrationArray[int(startingPoint + round(pos[i] / Amplitude))]){
        // while the current position is less than the next,
        if (currentSpeed < 30){ // Avoid stalling by always going at a speed greater than 30.
          drive(1, 30);
        }else{
          drive(1, currentSpeed); //move forward
        }
      }
    }
    if (currentSpeed < 0){
      while(cpos() >= calibrationArray[int(startingPoint + round(pos[i] / Amplitude))]){
        // while the current position is farther than the next,
        if (abs(currentSpeed) < 30){
          drive(-1, 30); 
        }
        else{
          drive(-1, (abs(currentSpeed))); //move backwards
        }
      }
    }
  }
}
void osccillate(){
  oscillateLoop(0, 20);
}
