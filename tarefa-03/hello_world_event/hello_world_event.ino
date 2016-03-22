#include "event_driven.h" 
TIMER_OFF // use macro to not implement timer callback

#define LED_PIN 13
#define BUT_PIN  2

void button_changed(int pin, int v) {
  digitalWrite(LED_PIN, v);
}

void user_init() {
  button_listen(BUT_PIN);
}


