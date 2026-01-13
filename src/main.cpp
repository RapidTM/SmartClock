#include <header.h>

// put function declarations here:

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

// Setup Constants
#define RE_STEPS_PER_TICK 2048 * 1.875
#define STEPS_PER_SECOND_ROTATION 64

// Setup Hardware
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);
TinyStepper_28BYJ_48 stepper1;
RotaryEncoder encoder(rotaryClockPin, rotaryDataPin, RotaryEncoder::LatchMode::FOUR3);

// Code Globals
int intent = 0;
int currentIntent = 0;
int direction = 0;
int RE_switch_state = 0;

unsigned long previousMillis1 = 0;
const long interval1 = 1000;
unsigned long previousMillis2 = 0;
const long interval2 = 1000;

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
  stepper1.setSpeedInStepsPerSecond(300); // (60 * SpS / 2048) to find RPM
  stepper1.setAccelerationInStepsPerSecondPerSecond(500);

  // Rotary Encoder Setup
  encoder.setPosition(0);
  encoder.tick();
  direction = encoder.getDirection();
}

void loop()
{
  // Tick encoder and get intent
  encoder.tick();
  direction = encoder.getDirection();
  intent = intent + (direction * RE_STEPS_PER_TICK);

  // Advance Motor
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= interval1)
  {
    previousMillis1 = currentMillis;

    if (direction == 0 && intent != 0)
    {
      if (abs(intent) < STEPS_PER_SECOND_ROTATION)
      {
        currentIntent = intent;
        intent = 0;
      }
      else
      {
        int sign = intent / abs(intent);
        currentIntent = STEPS_PER_SECOND_ROTATION * sign;
        intent = (abs(intent) - STEPS_PER_SECOND_ROTATION) * sign;
      }

      stepper1.moveRelativeInSteps(currentIntent);
      Serial.println(intent);
    }
  }
}
