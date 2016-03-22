#include "Arduino.h"
#include "event_driven.h"

#define N_PORTS 14

static unsigned int button_listeners;
static unsigned int button_state[N_PORTS] = {LOW};
static unsigned int pin; // invert control
static bool is_timer_set;
static unsigned int timer_interval, timer_start;

// Activate a listener for a button on `pin`
void button_listen(int pin) {
  if (pin < N_PORTS) {
    button_listeners |= (1 << pin);
    pinMode(pin, INPUT);
  }
}

// Set a one shot timer that expires after `ms` milliseconds
void timer_set(int ms) {
  is_timer_set = true;
  timer_start = millis();
  timer_interval = ms;
}

void setup() {
  button_listeners = 0;
  pin = 0;
  is_timer_set = false;
  timer_interval = 0;
  timer_start = 0;
  user_init(); // user setup
}

void loop() {
  // Check for state changes for buttons with active listeners
  if(button_listeners & (1 << pin) && digitalRead(pin) != button_state[pin]) {
    button_state[pin] = !button_state[pin];
    button_changed(pin, button_state[pin]);
  }
  pin = (pin + 1 >= N_PORTS) ? 0 : pin + 1;
  
  // If a timer has been set, check if it's been triggered
  if (is_timer_set && millis() >= timer_start + timer_interval) {
    is_timer_set = false;
    timer_expired(); // do this after updating `timer_set` because
                     //   the user might restart the timer.
  }
}
