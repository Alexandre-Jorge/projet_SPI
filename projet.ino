#include <Stepper.h>
#include <EEPROM.h>


const int stepsPerRevolution = 2048;

Stepper myStepper(stepsPerRevolution, 3, 4, 5, 6);

int currentstep;
void setup() {
  EEPROM.get(8000,currentstep);
  Serial.begin(9600); 
  Serial.println(currentstep);
  myStepper.setSpeed(10);

 
  
  
  int theta = 200;
  myStepper.step(theta);
  currentstep = currentstep + theta;
  Serial.println(currentstep);
  EEPROM.put(8000, theta);
}

void loop() {
  currentstep = currentstep % 2048;
  
  
}
