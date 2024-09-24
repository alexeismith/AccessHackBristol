/*
The contents of this code and instructions are the intellectual property of Carbon Aeronautics. 
The text and figures in this code and instructions are licensed under a Creative Commons Attribution - Noncommercial - ShareAlike 4.0 International Public Licence. 
This license lets you remix, adapt, and build upon your work non-commercially, as long as you credit Carbon Aeronautics 
(but not in any way that suggests that we endorse you or your use of the work) and license your new creations under the identical terms.
This code and instruction is provided "As Is” without any further warranty. Neither Carbon Aeronautics or the author has any liability to any person or entity 
with respect to any loss or damage caused or declared to be caused directly or indirectly by the instructions contained in this code or by 
the software and hardware described in it. As Carbon Aeronautics has no control over the use, setup, assembly, modification or misuse of the hardware, 
software and information described in this manual, no liability shall be assumed nor accepted for any resulting damage or injury. 
By the act of copying, use, setup or assembly, the user accepts all resulting liability.

1.0  5 October 2022 -  initial release
*/

#include <MIDI.h>

#include "MPU6050.h"

// Variables to hold measurements
float rollRate, pitchRate, yawRate;
float rollAngle, pitchAngle;

// Optional smoothing
// float rollRateSmooth = 0.0;
// float pitchRateSmooth = 0.0;
// float yawRateSmooth = 0.0;
// float rollAngleSmooth = 0.0;
// float pitchAngleSmooth = 0.0;

// Roll input range to map to MIDI CC
const int rollAngleMin = -75;
const int rollAngleMax = -25;
float rollAngleLimit = 0.0;

// Yaw input range to map to MIDI CC
const int yawRateMin = 100;
const int yawRateMax = 250;
float yawRateLimit = 0.0;

// MIDI CC range
const int midiMin = 0;
const int midiMax = 127;

// MIDI CC outputs
int chordComplexity;
int expression;

// MIDI CC slots
const int chordComplexityCC = 102;
const int expressionCC = 103;
const int midiChannel = 1;

// Copper signal threshold
const int copperThreshold = 10;

void setup() {
  // Feed the calibration values found using MPU6050::printCalibration()
  MPU6050::setup(-3.71, 1.60, 0.48, -0.05, 0.02, 0.04);

  usbMIDI.begin();
}

void loop() {
  // MPU6050::printCalibration(); // Call this function to generate hard-coded values to feed to setup()
  MPU6050::process(rollRate, pitchRate, yawRate, rollAngle, pitchAngle);

  // Optional smoothing: lower is smoother
  // float smoothing = 0.3;
  // rollRateSmooth = (smoothing * rollRate) + (1.0 - smoothing) * rollRateSmooth;
  // pitchRateSmooth = (smoothing * pitchRate) + (1.0 - smoothing) * pitchRateSmooth;
  // yawRateSmooth = (smoothing * yawRate) + (1.0 - smoothing) * yawRateSmooth;
  // rollAngleSmooth = (smoothing * rollAngle) + (1.0 - smoothing) * rollAngleSmooth;
  // pitchAngleSmooth = (smoothing * pitchAngle) + (1.0 - smoothing) * pitchAngleSmooth;

  rollAngleLimit = constrain(rollAngle, rollAngleMin, rollAngleMax);
  yawRateLimit = constrain(abs(yawRate), yawRateMin, yawRateMax);

  chordComplexity = int(roundf(map(rollAngleLimit, rollAngleMin, rollAngleMax, midiMin, midiMax)));
  expression = int(roundf(map(yawRateLimit, yawRateMin, yawRateMax, midiMin, midiMax)));

  // Debug prints
  // Serial.print("Chord Complexity: ");
  // Serial.println(chordComplexity);
  // Serial.print("Expression CC: ");
  // Serial.println(expression);

  usbMIDI.sendControlChange(expressionCC, expression, midiChannel);

  // Gate the chord CC based on copper strip  
  if (analogRead(A8) < copperThreshold) {
    usbMIDI.sendControlChange(chordComplexityCC, chordComplexity, midiChannel);
  }

  delay(50);
}
