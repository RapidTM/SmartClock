#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <TinyStepper_28BYJ_48.h>
#include <RotaryEncoder.h>

// put function declarations here:
int myFunction(int, int);

// Define Pins
#define stepperPin1 13
#define stepperPin2 12
#define stepperPin3 11
#define stepperPin4 10

#define rotaryClockPin 4
#define rotaryDataPin 3
#define rotarySwitchPin 2

#define displayDataPin A4
#define displayClockPin A5




// Setup Hardware
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);
TinyStepper_28BYJ_48 stepper1;
RotaryEncoder encoder(rotaryClockPin, rotaryDataPin, RotaryEncoder::LatchMode::FOUR3);

void setup() {
  Serial.begin(9600);

  // Pin Setup
  pinMode(rotaryClockPin, INPUT);
  pinMode(rotaryDataPin, INPUT);
  pinMode(rotarySwitchPin, INPUT);

  // Display Setup
  delay(250); // power up time
  display.begin(0x3c, true);
  display.clearDisplay();
  display.setTextSize(9);
  display.setTextColor(SH110X_WHITE);

  // Stepper Setup
  stepper1.connectToPins(stepperPin1, stepperPin2, stepperPin3, stepperPin4);
  stepper1.setCurrentPositionInSteps(0);
  stepper1.setSpeedInStepsPerSecond(1024); // (60 * SpS / 2048) to find RPM

  // Rotary Encoder Setup
  encoder.setPosition(0);
}

// Library Globals
int lastPos = 0;
int newPos = 0;
int condFlag = 0;
int setUpQue = 0;

// Global Variables
int time = 0;
int menuNum = 0;

void loop() {
  // put your main code here, to run repeatedly:
  // Advance Motor
  stepper1.processMovement();

  // Check Rotary Encoder
  encoder.tick();

  newPos = encoder.getPosition();
  int condFlag = 0;

  if (newPos < 0)
  {
    encoder.setPosition(19);
    condFlag = 1;
  }
  else if (newPos > 19)
  {
    encoder.setPosition(0);
    condFlag = 1;
  }

  if (lastPos != newPos && !condFlag)
  {
    Serial.println(newPos);
    stepper1.setupMoveInSteps(newPos * 103);
    lastPos = newPos;
    return newPos;
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void showHalfdayMenu()
{ // AM/PM Screen
  bool loop1 = 0;
  bool loop2 = 0;

  display.clearDisplay();

  if (time < 60 * 60 * 12)
  {
    display.write("AM");
    display.display();
  }
  else
  {
    display.write("PM");
    display.display();
  }
}

void switchScreenIfNeeded()
{

  if (digitalRead(rotarySwitchPin))
  {
    menuNum = menuNum + 1;
  }
}

bool encoderPositionHasChanged()
{
  if (lastPos != newPos && !condFlag)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}