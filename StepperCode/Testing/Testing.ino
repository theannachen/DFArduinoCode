// Motor Notes
// 4000 steps/s max
// 200 steps in rotation

// Lead Screw Notes
// Each rotation is 0.2 cm

// Pin Notes
// 22 and up are all digital
// 2-13 are PWM

#include <AccelStepper.h>
#include <LiquidCrystal.h>

const int maxSpeed = 4000;

// LCD Pins
const int rs = 42, en = 44, d4 = 46, d5 = 48, d6 = 50, d7 = 52;

// Motor Pins
const int stepPin = 2, directionPin = 22;

const int testingPin1 = 32;
const int testingPin2 = 34;
const int testingPin3 = 36;


// Setup Libraries
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
AccelStepper stepper(AccelStepper::DRIVER, stepPin, directionPin);

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);

  stepper.setMaxSpeed(maxSpeed);

  pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);
  
  pinMode(testingPin1, OUTPUT);
  pinMode(testingPin2, OUTPUT);
  pinMode(testingPin3, OUTPUT);


  digitalWrite(testingPin1, HIGH);
  digitalWrite(testingPin2, HIGH);
  digitalWrite(testingPin3, HIGH);


  lcd.setCursor(0,0);
  lcd.print("Testing");
}

void loop() {
  stepper.setSpeed(1000);
  stepper.runSpeed();
}
