#include <header.h>

// put function declarations here:
int decide_rotary_encoder_direction();

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
void setup()
{
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
  stepper1.setSpeedInStepsPerSecond(64); // (60 * SpS / 2048) to find RPM

  // Rotary Encoder Setup
  encoder.setPosition(0);
}

// Library Globals
int lastPos = 0;
int intent = 0;
int dir = 0;

void loop()
{
  // Advance Motor
  stepper1.processMovement();

  encoder.tick();
  dir = encoder.getDirection();
  if (dir == 0)
  {
    return;
  }

  stepper1.moveRelativeInSteps(dir * 64);
}
