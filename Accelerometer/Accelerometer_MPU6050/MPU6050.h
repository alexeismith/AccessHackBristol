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

1.0  29 December 2022 -  initial release
*/

#include <Wire.h>

namespace MPU6050 {

float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;

float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
float AccCalibrationX, AccCalibrationY, AccCalibrationZ;

float AngleRoll, AnglePitch;

float KalmanAngleRoll=0, KalmanUncertaintyAngleRoll=2*2;
float KalmanAnglePitch=0, KalmanUncertaintyAnglePitch=2*2;
float Kalman1DOutput[]={0,0};

void kalman_1d(float KalmanState, float KalmanUncertainty, float KalmanInput, float KalmanMeasurement) {
  KalmanState=KalmanState+0.004*KalmanInput;
  KalmanUncertainty=KalmanUncertainty + 0.004 * 0.004 * 4 * 4;
  float KalmanGain=KalmanUncertainty * 1/(1*KalmanUncertainty + 3 * 3);
  KalmanState=KalmanState+KalmanGain * (KalmanMeasurement-KalmanState);
  KalmanUncertainty=(1-KalmanGain) * KalmanUncertainty;
  Kalman1DOutput[0]=KalmanState; 
  Kalman1DOutput[1]=KalmanUncertainty;
}

void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8);
  Wire.endTransmission();     
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
  AccX=(float)AccXLSB/4096 - AccCalibrationX;
  AccY=(float)AccYLSB/4096 - AccCalibrationY;
  AccZ=(float)AccZLSB/4096 - AccCalibrationZ;
  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
  AnglePitch=-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180);
}

void setup(float rateCalibrationRoll, float rateCalibrationPitch, float rateCalibrationYaw,
           float accCalibrationX, float accCalibrationY, float accCalibrationZ) {
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

  RateCalibrationRoll = rateCalibrationRoll;
  RateCalibrationPitch = rateCalibrationPitch;
  RateCalibrationYaw = rateCalibrationYaw;

  AccCalibrationX = accCalibrationX;
  AccCalibrationY = accCalibrationY;
  AccCalibrationZ = accCalibrationZ;
}

void printCalibration() {
  int calibrationLength = 2000;

  float rateCalibrationRoll = 0.0;
  float rateCalibrationPitch = 0.0;
  float rateCalibrationYaw = 0.0;

  float accCalibrationX = 0.0;
  float accCalibrationY = 0.0;
  float accCalibrationZ = 0.0;

  for (int i = 0; i <calibrationLength; i++) {
    gyro_signals();
  
    rateCalibrationRoll+=RateRoll;
    rateCalibrationPitch+=RatePitch;
    rateCalibrationYaw+=RateYaw;

    accCalibrationX+=AccX;
    accCalibrationY+=AccY;
    accCalibrationZ+=AccZ;
  
    delay(1);
  }

  rateCalibrationRoll/=calibrationLength;
  rateCalibrationPitch/=calibrationLength;
  rateCalibrationYaw/=calibrationLength;

  accCalibrationX/=calibrationLength;
  accCalibrationY/=calibrationLength;
  accCalibrationZ/=calibrationLength;

  Serial.print("rateCalibrationRoll: ");
  Serial.print(rateCalibrationRoll);
  Serial.print(" rateCalibrationPitch: ");
  Serial.print(rateCalibrationPitch);
  Serial.print(" rateCalibrationYaw: ");
  Serial.println(rateCalibrationYaw);

  Serial.print("accCalibrationX: ");
  Serial.print(accCalibrationX);
  Serial.print(" accCalibrationY: ");
  Serial.print(accCalibrationY);
  Serial.print(" accCalibrationZ - 1.0: ");
  Serial.println(accCalibrationZ - 1.0);
}

void process(float &rollRate, float &pitchRate, float &yawRate, float &rollAngle, float &pitchAngle) {
  gyro_signals();
  RateRoll-=RateCalibrationRoll;
  RatePitch-=RateCalibrationPitch;
  RateYaw-=RateCalibrationYaw;
  kalman_1d(KalmanAngleRoll, KalmanUncertaintyAngleRoll, RateRoll, AngleRoll);
  KalmanAngleRoll=Kalman1DOutput[0]; 
  KalmanUncertaintyAngleRoll=Kalman1DOutput[1];
  kalman_1d(KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch);
  KalmanAnglePitch=Kalman1DOutput[0]; 
  KalmanUncertaintyAnglePitch=Kalman1DOutput[1];

  rollRate = RateRoll;
  pitchRate = RatePitch;
  yawRate = RateYaw;

  // Kalman filter not working, so use raw angle instead
//   rollAngle = KalmanAngleRoll;
//   pitchAngle = KalmanAnglePitch;
  rollAngle = AngleRoll;
  pitchAngle = AnglePitch;
}

}
