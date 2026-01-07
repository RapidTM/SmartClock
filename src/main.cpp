#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <TinyStepper_28BYJ_48.h>
#include <RotaryEncoder.h>

// put function declarations here:
void update_rotary_encoder_and_intent();

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
bool roundBool = 0;
int intent = 0;

void loop() {
  // Advance Motor
  stepper1.processMovement();

  // Check Rotary Encoder
  update_rotary_encoder_and_intent();
}

// put function definitions here:
void update_rotary_encoder() {
  encoder.tick();

  newPos = encoder.getPosition();
  roundBool = 0;

  if (newPos < 0) {
    encoder.setPosition(19);
    roundBool = 1;
  }
  else if (newPos > 19) {
    encoder.setPosition(0);
    roundBool = 1;
  }

  if (newPos != lastPos &&!roundBool) {
    if (newPos > lastPos) {
      intent
    }
    lastPos = newPos;
  }
}