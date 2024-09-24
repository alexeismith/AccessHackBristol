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

// SOURCE: Carbon Aeronautics - How to use the MPU6050 with Arduino and Teensy
// https://www.youtube.com/watch?v=yhz3bRQLvBY
// https://github.com/CarbonAeronautics/Part-IV-MPU6050gyroscope

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
int rollAngleMin = -75;
int rollAngleMax = -25;
float rollAngleLimit = 0.0;

// Yaw input range to map to MIDI CC
int yawRateMin = 100;
int yawRateMax = 250;
float yawRateLimit = 0.0;

// MIDI CC range
int midiMin = 0;
int midiMax = 127;

// MIDI CC outputs
int chordComplexityCC;
int expressionCC;

void setup() {
  // Feed the calibration values found using MPU6050::printCalibration()
  MPU6050::setup(-3.71, 1.60, 0.48, -0.05, 0.02, 0.04);
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

  rollAngleLimit = min(max(rollAngle, rollAngleMin), rollAngleMax);
  yawRateLimit = min(max(abs(yawRate), yawRateMin), yawRateMax);

  chordComplexityCC = int(roundf(map(rollAngleLimit, rollAngleMin, rollAngleMax, midiMin, midiMax)));
  expressionCC = int(roundf(map(yawRateLimit, yawRateMin, yawRateMax, midiMin, midiMax)));

  Serial.print("Chord Complexity: ");
  Serial.println(chordComplexityCC);
  Serial.print("Expression CC: ");
  Serial.println(expressionCC);

  delay(50);
}

// Plan:
// Roll angle controls Chord complexity
// Yaw rate controls delay input

// TODO:
// MIDI output
// Connect switch, freeze chordComplexityCC when not pressed
