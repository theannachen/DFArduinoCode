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
#include <Math.h>

// Geometry of Syringe
const float radiusMm = 5.0;

// Milliliters per minute
const float flowRateMinutes = 3.0;

// Milliliters per second
const float flowRate = flowRateMinutes/60;

const int maxSpeed = 4000;

const float cmPerRevolution = 0.2;

const float stepsPerRevolution = 200.0;

const float surfaceArea = (sq(radiusMm) * M_PI);

// cm/s
const float linearSpeed = flowRate / surfaceArea;

// steps/s
const int stepSpeed = int(linearSpeed / cmPerRevolution * stepsPerRevolution);
// LCD Pins
const int rs = 42, en = 44, d4 = 46, d5 = 48, d6 = 50, d7 = 52;

// Motor Pins
const int stepPin = 2, directionPin = 22;

const int red[3] = { 255, 0 , 0 };
const int yellow[3] = { 255, 255, 0 };
const int green[3] = { 0, 255, 0 };

//Buttons + Limit switch
const int mb =  24, fb = 28, bb = 30, ls = 26;

//RGB Components
const int rp = 3, gp = 4, bp = 5;



// Setup Libraries
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
AccelStepper stepper(AccelStepper::DRIVER, stepPin, directionPin);

void setup() {
  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);

  //Configure input pins and enable the internal pull-up resistor
  pinMode(mb, INPUT_PULLUP);
  pinMode(fb, INPUT_PULLUP);
  pinMode(bb, INPUT_PULLUP);
  pinMode(ls, INPUT_PULLUP);

  pinMode(rp, OUTPUT);
  pinMode(gp, OUTPUT);
  pinMode(bp, OUTPUT);

  lcd.begin(16, 2);

  pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);

  stepper.setMaxSpeed(maxSpeed);
  stepper.setSpeed(linearSpeed);

  lcd.setCursor(0,0);
  lcd.print(String(flowRate, 3));
  lcd.print(" cm/s");

  lcd.setCursor(0,1);
  lcd.print("Time Left: ");
}

void loop() {

  //Conditions:
  // Main button pressed - move forward continuous
  // Forward button pressed - move one step forward
  // Back button pressed - move one step backward
  // Limit switch - override everything with stop

  int mainVal = digitalRead(mb);
  int forwardVal = digitalRead(fb);
  int backVal = digitalRead(bb);
  int limitVal = digitalRead(ls);
  
  if (limitVal == LOW) {
    if (mainVal == LOW) {
      color(green);
    } else {
      color(yellow);
    }

    if (mainVal == LOW || forwardVal == LOW) {
      stepper.runSpeed();
    } else if (backVal == LOW) {
      stepper.setSpeed(-1 * stepSpeed);
      stepper.runSpeed();
      stepper.setSpeed(stepSpeed);
    }
  } else {
    color(red);

    stepper.setCurrentPosition(0);
    
    if (backVal == LOW) {
      stepper.setSpeed(-1 * stepSpeed);
      stepper.runSpeed();
      stepper.setSpeed(stepSpeed);
    }
  }

  float stepsRemaining = stepper.currentPosition();

  int secondsRemaining = int(stepsRemaining / stepSpeed);

  lcd.setCursor(1,11);
  lcd.print(secondsRemaining);
}

void color(int c[]){
  analogWrite(rp, c[0]);
  analogWrite(gp, c[1]);
  analogWrite(bp, c[2]);
}
