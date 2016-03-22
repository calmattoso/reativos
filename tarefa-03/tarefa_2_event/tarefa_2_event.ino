#include "event_driven.h"

#define LED_PIN   13
#define BUT_PIN_1 2
#define BUT_PIN_2 3
#define DELTA     250l
#define DEBOUNCE  100l
  
bool state; // LED state: On/Off
bool updated; // `true` if button press has already been dealt with
unsigned long interval;   // interval between each blink
unsigned long button1t, button2t; // last time each button was pressed

void button_changed(int pin, int v) {
  long diff = 0;
  unsigned long t_now = millis();
  
  if (!v) return; // ignore button unpress event
  
  // if `button 1` has been pressed
  if (pin == BUT_PIN_1 && t_now > button1t + DEBOUNCE) { 
    button1t = millis();
    updated = false;
  }
  // if `button 2` has been pressed
  else if (pin == BUT_PIN_2 && t_now > button2t + DEBOUNCE) {
    button2t = millis();
    updated = false;
  }
  
  diff = button2t - button1t;
  diff = abs(diff);
  
  // only check after both buttons have been pressed at least once
  if (button1t != 0 && button2t != 0 && diff <= 500) { 
    digitalWrite(LED_PIN, LOW);
    while(1); // "stop" running
  }
  else if(!updated && button1t > button2t) {
    interval = ((interval - DELTA) > 0 ? (interval - DELTA) : 250l); // set 250ms as lower-bound
    updated = true;
  }
  else if(!updated && button2t > button1t) {
    interval += DELTA;
    updated = true;
  }
}

// Blink the led every `interval`
void timer_expired() {
    state = !state;
    digitalWrite(LED_PIN, state); 
    timer_set(interval); // reset the timer
}

void user_init() {
  button_listen(BUT_PIN_1);
  button_listen(BUT_PIN_2);
  pinMode(LED_PIN, OUTPUT);
  
  interval = 1000l; // 1s each blink
  button1t = button2t = 0;
  state = false;
  updated = true;
  
  timer_set(interval);
}

