#include <Servo.h>                           

#define _offset -50

#define SERVO_LEFT     13
#define SERVO_RIGHT    12
#define SENSOR_ONE_IN  10
#define SENSOR_ONE_OUT  9
#define SENSOR_TWO_IN   3
#define SENSOR_TWO_OUT  2
#define BUZZER          4

#define FREQ_RANGE_LOWER 38000
#define FREQ_RANGE_UPPER 42000

Servo servoLeft;                             
Servo servoRight;

const int setpoint = 2;                      
unsigned long then;
int i = 1;
 
void setup()                                 
{ 
  pinMode(SENSOR_ONE_IN, INPUT);  
  pinMode(SENSOR_ONE_OUT, OUTPUT);   
  pinMode(SENSOR_TWO_IN, INPUT);  
  pinMode(SENSOR_TWO_OUT, OUTPUT);    
  
  then = millis();

  servoLeft.attach(SERVO_LEFT);                      
  servoRight.attach(SERVO_RIGHT);                    
}  
 
void loop()                                  
{
  int sensorLeft = sensorDistance(SENSOR_ONE_OUT, SENSOR_ONE_IN);            
  int sensorRight = sensorDistance(SENSOR_TWO_OUT, SENSOR_TWO_IN);           
  
  // Something is in front of the car
  if (!sensorLeft || !sensorRight) {
    // Stop the car
    setServo(0, 0, 2000);
    // Honks 
    tone(BUZZER, 3000, 500);
    delay(1000);
    tone(BUZZER, 3000, 500);
    delay(2000);
    
    // check if obstacle remains in front of car
    sensorLeft = sensorDistance(SENSOR_ONE_OUT, SENSOR_ONE_IN);            
    sensorRight = sensorDistance(SENSOR_TWO_OUT, SENSOR_TWO_IN); 
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
  for(long f = FREQ_RANGE_LOWER; f <= FREQ_RANGE_UPPER; f += 1000) {
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
