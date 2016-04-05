#define TEMP_LEVELS 5
#define TEMP_SENSOR_PIN A0
#define INTERVAL 1000
int leds[TEMP_LEVELS] = {2,3,4,5,6};
unsigned long then, now;

// Try it on: https://123d.circuits.io/circuits/1890080-tarefa-extra

// Set the LEDs
void setup() {
  int i;
  
  then = 0; // force an initial update
  now = millis();
  
  for(i = 0; i < TEMP_LEVELS; i++)
    pinMode(leds[i], OUTPUT);
}

void loop() {
  int temp;
  float tempC;
  // Read the temperature every `INTERVAL`ms
  now = millis();
  if (now > then + INTERVAL) {
    int temp = analogRead(TEMP_SENSOR_PIN);
    
    // Now we'll convert the value read from the sensor to voltage and then to 
    // degrees Celsius, using the formula from the temperature sensor datasheet for
    // the latter.
    // Source: https://learn.sparkfun.com/tutorials/sik-experiment-guide-for-arduino---v32/experiment-7-reading-a-temperature-sensor
    tempC = ((temp * 5.0/1023.0) - 0.5) * 100.0;
    setLeds(tempC);
    
    then = now;
  }
}

/* Set the leds based on the current temperature (in Celsius). The leds are turned on right to left.
   1) [0, 30) all leds are off
   2) [30, 32): first led is on
   3) [32, 34): first two leds are on
   4) [34, 36): first three leds are on
   5) [36, 38): first four leds are on
   6) [38, +maxtemp)  : all leds are on
 */  
void setLeds(float tempLevel) {
  int i, level = 5; // assume we're at the highest level 
  if (tempLevel < 30)
    level = 0;
  else if (tempLevel < 32)
    level = 1;
  else if (tempLevel < 34)
    level = 2;
  else if (tempLevel < 36)
    level = 3;
  else if (tempLevel < 38)
    level = 4;
  
  for (i = 0; i < TEMP_LEVELS; i++)
    (i < level) ? digitalWrite(leds[i], HIGH) : digitalWrite(leds[i], LOW);
}
