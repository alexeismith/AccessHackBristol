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

float rollRate, pitchRate, yawRate;
float rollAngle, pitchAngle;

float rollRateSmooth = 0.0;
float pitchRateSmooth = 0.0;
float yawRateSmooth = 0.0;

float rollAngleSmooth = 0.0;
float pitchAngleSmooth = 0.0;

void setup() {
  MPU6050::setup(-3.71, 1.60, 0.48, -0.05, 0.02, 0.04);
}

void loop() {
  // MPU6050::printCalibration(); // Call this function to generate hard-coded values to feed to setup()
  MPU6050::process(rollRate, pitchRate, yawRate, rollAngle, pitchAngle);

  float smoothing = 1.0 - (analogRead(A7) / float(1023));

  rollRateSmooth = (smoothing * rollRate) + (1.0 - smoothing) * rollRateSmooth;
  pitchRateSmooth = (smoothing * pitchRate) + (1.0 - smoothing) * pitchRateSmooth;
  yawRateSmooth = (smoothing * yawRate) + (1.0 - smoothing) * yawRateSmooth;

  rollAngleSmooth = (smoothing * rollAngle) + (1.0 - smoothing) * rollAngleSmooth;
  pitchAngleSmooth = (smoothing * pitchAngle) + (1.0 - smoothing) * pitchAngleSmooth;

  // Serial.print("Roll = ");
  // Serial.print(int(rollRateSmooth / 10)); 
  // Serial.print(" Pitch = ");
  // Serial.print(int(pitchRateSmooth / 10));
  // Serial.print(" Yaw = ");
  // Serial.println(int(yawRateSmooth / 10));

  // Serial.print("Roll Angle = ");
  // Serial.print(int(rollAngleSmooth / 10));
  // Serial.print(" Pitch Angle = ");
  // Serial.println(int(pitchAngleSmooth / 10));

  Serial.print("Chord Complexity: ");
  Serial.println(int(rollAngleSmooth / 15));
  Serial.print("Expression CC: ");
  Serial.println(int(yawRateSmooth / 10));
  
  delay(50);
}

// Plan:
// Roll angle controls Chord complexity
// Yaw rate controls delay input

// TODO: 
// Print pot value and hard-code sensitivity for rate and angle separately 
// Convert yawRate to percentage
// MIDI output
// Connect two switches, send signal only when they are pressed
