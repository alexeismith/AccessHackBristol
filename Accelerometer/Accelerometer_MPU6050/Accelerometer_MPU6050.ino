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

#include <Wire.h>

float rollRate, pitchRate, yawRate;

// Run calibrate() to regenerate the following hard-coded values:
float rollCalibration = -3.71;
float pitchCalibration = 1.75;
float yawCalibration = 0.49;

void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission(); 
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  rollRate=(float)GyroX/65.5;
  pitchRate=(float)GyroY/65.5;
  yawRate=(float)GyroZ/65.5;
}

void calibrate() {
  int calibrationLength = 2000;

  for (int i = 0; i <calibrationLength; i++) {
    gyro_signals();
    rollCalibration+=rollRate;
    pitchCalibration+=pitchRate;
    yawCalibration+=yawRate;
    delay(1);
  }
  rollCalibration/=calibrationLength;
  pitchCalibration/=calibrationLength;
  yawCalibration/=calibrationLength;

  Serial.print("rollCalibration: ");
  Serial.println(rollCalibration);
  Serial.print("pitchCalibration: ");
  Serial.println(pitchCalibration);
  Serial.print("yawCalibration: ");
  Serial.println(yawCalibration);
}

void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();  
}

void loop() {
  gyro_signals();
  // calibrate();

  rollRate-=rollCalibration;
  pitchRate-=pitchCalibration;
  yawRate-=yawCalibration;

  float pot = analogRead(A7) / float(1023);

  rollRate *= pot;
  pitchRate *= pot;
  yawRate *= pot;

  // Round to int
  rollRate = float(int(rollRate));
  pitchRate = float(int(pitchRate));
  yawRate = float(int(yawRate));

  Serial.print("Roll = ");
  Serial.print(rollRate); 
  Serial.print(" Pitch = ");
  Serial.print(pitchRate);
  Serial.print(" Yaw = ");
  Serial.println(yawRate);

  Serial.println(pot);
  
  delay(50);
}

// TODO:
// Add potentiometer smoothing
// Translate rate to auto-settling angle?