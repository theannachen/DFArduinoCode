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

// Geometry of Syringe (cm)
const float radius = 1.913/2.0;

const int maxSpeed = 4000;

const float cmPerRevolution = 0.2;

int microstepping = 0;

float stepsPerRevolution;

const float surfaceArea = sq(radius) * M_PI;

// LCD Pins
const int rs = 42, en = 44, d4 = 46, d5 = 48, d6 = 50, d7 = 52;

// Motor Pins
const int stepPin = 2, directionPin = 22;

const int red[3] = { 255, 0 , 0 };
const int yellow[3] = { 255, 100, 0 };
const int green[3] = { 0, 255, 0 };

//Buttons + Limit switch
const int mb =  24, fb = 28, bb = 30, ls = 26;

// Microstepping pins
const int ms1 =  32, ms2 = 34, ms3 = 36;

//RGB Components
const int rp = 3, gp = 4, bp = 5;

const int flowratePin = A0;

int oldFlowRatePotAvg = -1;

const int debounceSize = 64;
int flowRatePotVals[debounceSize];

int oldSecondsRemaining = -1;

int stepSpeed;



// Setup Libraries
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
AccelStepper stepper(AccelStepper::DRIVER, stepPin, directionPin);

void setup() {
//  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);

  pinMode(ms1, OUTPUT);
  pinMode(ms2, OUTPUT);
  pinMode(ms3, OUTPUT);

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
//  stepper.setAcceleration(12000);

  microstepping = 0;
  digitalWrite(ms1, LOW);
  digitalWrite(ms2, LOW);
  digitalWrite(ms3, LOW);

  int tempVal = analogRead(flowratePin);
  for (int i = 0; i < debounceSize; i++) {
    flowRatePotVals[i] = tempVal;
  }
}

void loop() {

  //Conditions:
  // Main button pressed - move forward continuous
  // Forward button pressed - move one step forward
  // Back button pressed - move one step backward
  // Limit switch - override everything with stop

  int flowRatePotVal = analogRead(flowratePin);
  
  int mainVal = digitalRead(mb);
  int forwardVal = digitalRead(fb);
  int backVal = digitalRead(bb);
  int limitVal = digitalRead(ls);

  for (int i = 1; i < debounceSize; i++) {
    flowRatePotVals[i-1] = flowRatePotVals[i];
  }
  flowRatePotVals[debounceSize-1] = flowRatePotVal;

  long flowRatePotAvg = 0;
  for (int i = 0; i < debounceSize; i++) {
    flowRatePotAvg += flowRatePotVals[i];
  } 

  flowRatePotAvg /= 32;

  flowRatePotAvg /= 10;

  flowRatePotAvg *= 10;

  if (flowRatePotAvg != oldFlowRatePotAvg) {
    oldFlowRatePotAvg = flowRatePotAvg;
    
//    float flowRatePerMinute = 0.005 * flowRatePotAvg * flowRatePotAvg;
    float flowRatePerMinute = 0.005 * flowRatePotAvg;


    float flowRatePerSecond = flowRatePerMinute / 60.0;
  
    float linearSpeed = flowRatePerSecond / surfaceArea;
  
    int newMicrostepping = 5;
    do {
      newMicrostepping--;
      float stepsPerRevolution = 200.0 * (1 << newMicrostepping);
      stepSpeed = int(linearSpeed / cmPerRevolution * stepsPerRevolution);
    } while (stepSpeed > 1500 && newMicrostepping > 1);
  
    if (newMicrostepping != microstepping) {
      if (newMicrostepping == 0) {
        digitalWrite(ms1, LOW);
        digitalWrite(ms2, LOW);
        digitalWrite(ms3, LOW);
      } else if (newMicrostepping == 1) {
        digitalWrite(ms1, HIGH);
        digitalWrite(ms2, LOW);
        digitalWrite(ms3, LOW);
      } else if (newMicrostepping == 2) {
        digitalWrite(ms1, LOW);
        digitalWrite(ms2, HIGH);
        digitalWrite(ms3, LOW);
      } else if (newMicrostepping == 3) {
        digitalWrite(ms1, HIGH);
        digitalWrite(ms2, HIGH);
        digitalWrite(ms3, LOW);
      } else if (newMicrostepping == 4) {
        digitalWrite(ms1, HIGH);
        digitalWrite(ms2, HIGH);
        digitalWrite(ms3, HIGH);
      }
      int position = stepper.currentPosition() << (4 - microstepping);
      stepper.setCurrentPosition(position >> (4 - newMicrostepping));
      microstepping = newMicrostepping;
    }

//    Serial.print("Potentiometer: ");
//    Serial.print(flowRatePotVal);
//    Serial.print(" Flow Rate: ");
//    Serial.print(flowRatePerSecond);
//    Serial.print(" mL/s Microstepping: 1/");
//    Serial.print(1 << microstepping);
//    Serial.print(" Step Speed: ");
//    Serial.println(stepSpeed);
  
    lcd.setCursor(0,0);
    lcd.print(String(flowRatePerMinute, 2));
    lcd.print(" mL/min ");
  }

  
  
  
  if (limitVal == LOW) {
    if (mainVal == LOW) {
      color(green);
    } else {
      color(yellow);
    }

    if (mainVal == LOW) {
      stepper.setSpeed(stepSpeed);
      stepper.runSpeed();
    } else if (forwardVal == LOW) {
      stepper.setSpeed(stepSpeed);
      stepper.runSpeed();
    } else if (backVal == LOW) {
      stepper.setSpeed(-1 * stepSpeed);
      stepper.runSpeed();
    }
  } else {
    color(red);

    stepper.setCurrentPosition(0);
    
    if (backVal == LOW) {
      stepper.setSpeed(-1 * stepSpeed);
      stepper.runSpeed();
    }
  }

  float stepsRemaining = -1 * stepper.currentPosition();

  int secondsRemaining = int(stepsRemaining / stepSpeed);

  if (secondsRemaining != oldSecondsRemaining) {
    oldSecondsRemaining = secondsRemaining;
    lcd.setCursor(0,1);
    lcd.print("Time Left: ");
    lcd.print(secondsRemaining, DEC);
    lcd.print("  ");
  }
}

void color(const int c[]){
  analogWrite(rp, c[0]);
  analogWrite(gp, c[1]);
  analogWrite(bp, c[2]);
}
