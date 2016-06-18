#ifndef CAR_H
#define CAR_H

#include "Servo.h"
#include "env_config.h"

Servo servoLeft;                             
Servo servoRight;

void CAR_setup() {
  servoLeft.attach(SERVO_LEFT);                      
  servoRight.attach(SERVO_RIGHT); 
  setServo(0,0);
}

// Sensing
int sensorDistance(int sensorLedPin, int sensorReceivePin) {  
  int distance = 0;
  for(long f = FREQ_RANGE_LOWER; f <= FREQ_RANGE_UPPER; f += 1000) {
    distance += sensorDetect(sensorLedPin, sensorReceivePin, f);
  }
  return distance;
}

int sensorDetect(int sensorLedPin, int sensorReceiverPin, long frequency) {
  tone(sensorLedPin, frequency, 8);             
  delay(1);                                
  int sensorRead = digitalRead(sensorReceiverPin);       
  delay(1); 
  // Return 0 if detect something  
  return sensorRead;                                
}    

// Actuator
void setServo(int speedLeft, int speedRight)
{
  servoLeft.writeMicroseconds(1500 + speedLeft);  
  servoRight.writeMicroseconds(1500 - speedRight);                                   
}

#endif
