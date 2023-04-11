#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 3, 2);

//max is 4000 steps per second

//LED: Digital Pins at 3, 5, 6

//Stepper Driver 8, 9

//LCD 4, 6, 10, 11, 12, 13

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(50);

}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.runSpeed();

}
