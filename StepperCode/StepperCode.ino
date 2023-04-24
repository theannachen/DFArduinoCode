// Motor Notes
// 4000 steps/s max
// 200 steps in rotation


//Pin Notes
//22 and up are all digital
//2-13 are PWM



#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <Math.h>

//Set Geometry of Syringe
const float radiusMm = 5.0;
const float lengthMm = 20.0;
const int startingMillis = 20;

//Milliliters per minute
const float flowRateMinutes = 3.0;

//Milliliters per second
const flowRate = flowRateMinutes/60;

//LCD Pins
const int rs = 42, en = 44, d4 = 46, d5 = 48, d6 = 50, d7 = 52;

//Motor Bins
const int stepPin = 2, motorDirection = 22;

const int[3] red = [255, 0 , 0];
const int[3] yellow = [255, 255, 0];
const int[3] green = [0, 0, 255];

//Buttons + Limit switch
const int mb =  24, fb = 28, bb = 30, ls = 26;

//RGB Components
const int rp = 3, gp = 4, bp = 5;

//TODO: How to calculate max speed
const int maxSpeed = 40;

//Setup Libraries
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
AccelStepper stepper(AccelSteppere::DRIVER, motorDirection, stepPin);

void setup() {
  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(motorDirection, OUTPUT);

  //configure input pins and enable the internal pull-up resistor
  pinMode(mb, INPUT_PULLUP);
  pinMode(fb, INPUT_PULLUP);
  pinMode(bb, INPUT_PULLUP);
  pinMode(ls, INPUT_PULLUP);

  pinMode(rp, OUTPUT);
  pinMode(gp, OUTPUT);
  pinMode(bp, OUTPUT);

  //Show flow rate and time remaining
  lcd.begin(16, 2);

  pinMode(rs, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);

  int running = 0;

  float surfaceArea = (sq(radiusMm) * M_PI);

  //cm/s
  float speed = flowrate / surfaceArea;

  //steps/s
  int stepSpeed = int(speed * 8 * 200);

  stepper.setMaxSpeed(maxSpeed);
  stepper.setSpeed(min(speed, maxSpeed));

  lcd.setCursor(0,0);
  lcd.print(String(flowRate, 3));
  lcd.print(" cm/s");

  lcd.setCursor(0,1);
  lcd.print("Time Left: ");

  int totalMillis = (startingMillis / flowRate) * 1000;
  int timeLeft = 0;
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
  
  if (ls == HIGH){

    if (mainVal == HIGH) {
      stepper.runSpeed();
      if (running == 0){
        int startTime = millis();
        int endTime = startTime + totalMillis;
        running = 1;
      }

      timeLeft = (endTime - millis()) / 1000;

      lcd.setCursor(1,11);
      lcd.print(timeLeft);
    }

    else if (forwardVal == HIGH){
      stepper.runSpeed();
    }
    
    else if (backVal == HIGH){
      stepper.setSpeed(-1 * speed);
      stepper.runSpeed();
      stepper.setSpeed(speed);
    }

    else{
      color(yellow)
    }
    color(green)
  }
  else{
    color(red)
    running = 0;
  }
  running = 0;
}



void color3(int r, int g, int b]){
  analogWrite(rp, r);
  analogWrite(gb, g);
  analogWrite(bp, b);
}

void color(int[] color]){
  analogWrite(rp, color[0]);
  analogWrite(gb, color[1]);
  analogWrite(bp, color[2]);
}
