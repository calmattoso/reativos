#define LED_PIN   13
#define BUT_PIN_1 2
#define BUT_PIN_2 3
#define DELTA     250l

boolean state; // LED state: On/Off
boolean updated; // `true` if button press has already been dealt with
unsigned long t_last; // time of last blink 
unsigned long t_now;  // current time
unsigned long interval;   // interval between each blink
unsigned long button1t, button2t; // last time each button was pressed

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUT_PIN_1, INPUT);
  pinMode(BUT_PIN_2, INPUT);
  
  t_now = millis();
  t_last = t_now;
  interval = 1000l; // 1s each blink
  button1t = button2t = 0;
  state = false;
  updated = true;
}

void loop() {
  long diff = 0;
  t_now = millis(); 
  
  /* For many iterations of this function `digitalRead` will yield `HIGH` if the
   *   button was just pressed. This could lead to the `interval` updates below
   *   being done multiple times.
   * Therefore, when a button is pressed, make sure to only react to this event
   *   once by ignoring any further `HIGH` reads until `t_now` has past `button#t`
   *   by at least 100ms.
   * Also, reset `updated` in order to react to button press.
   */
  if (t_now > button1t + 100 && digitalRead(BUT_PIN_1)) {
    button1t = millis();
    updated = false;
  }
  if (t_now > button2t + 100 && digitalRead(BUT_PIN_2)) {
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

  // Blink the led every `interval`
  if (t_now >= t_last + interval) {
    t_last = t_now;
    state = !state;
    digitalWrite(LED_PIN, state); 
  }
}
