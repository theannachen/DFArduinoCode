#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 3, 2);

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(50);

}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.runSpeed();

}
