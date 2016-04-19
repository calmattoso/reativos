#include <Servo.h>                           

#define _offset -50

Servo servoLeft;                             
Servo servoRight;

const int setpoint = 2;                      
unsigned long then;
int blocked, not_blocked;
int i = 1;
 
void setup()                                 
{ 
  pinMode(10, INPUT);  
  pinMode(9, OUTPUT);   
  pinMode(3, INPUT);  
  pinMode(2, OUTPUT);    
  
  then = millis();
  blocked = 0;
  not_blocked = 0;

  servoLeft.attach(13);                      
  servoRight.attach(12);                    
}  
 
void loop()                                  
{
  int sensorLeft = sensorDistance(9, 10);            
  int sensorRight = sensorDistance(2, 3);           
  
  // Something is in front of the car
  if (!sensorLeft || !sensorRight) {
    // Stop the car
    setServo(0, 0, 2000);
    // Honks 
    tone(4, 3000, 500);
    delay(1000);
    tone(4, 3000, 500);
    delay(2000);
    
    // check if obstacle remains in front of car
    sensorLeft = sensorDistance(9, 10);            
    sensorRight = sensorDistance(2, 3); 
    if (!sensorLeft || !sensorRight) {
      // Changes direction
       setServo(-200, 200, 1000);
    }
    
  } else {
    // Controls left and right servos speed
    int moveLeft = (setpoint - sensorLeft) * _offset;     
    int moveRight = (setpoint - sensorRight) * _offset;
    setServo(moveLeft, moveRight, 20);       
  }
} 

int sensorDistance(int sensorLedPin, int sensorReceivePin)
{  
  int distance = 0;
  for(long f = 38000; f <= 42000; f += 1000) {
    distance += sensorDetect(sensorLedPin, sensorReceivePin, f);
  }
  return distance;
}

int sensorDetect(int sensorLedPin, int sensorReceiverPin, long frequency)
{
  tone(sensorLedPin, frequency, 8);             
  delay(1);                                
  int sensorRead = digitalRead(sensorReceiverPin);       
  delay(1); 
  // Return 0 if detect something  
  return sensorRead;                                
}     

void setServo(int speedLeft, int speedRight, int Time)
{
  servoLeft.writeMicroseconds(1500 + speedLeft);  
  servoRight.writeMicroseconds(1500 - speedRight); 
  delay(Time);                                  
}
