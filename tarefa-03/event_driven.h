#ifdef __cplusplus
extern "C" {
#endif
  // Listeners setters
  void button_listen(int pin);
  void timer_set(int ms);

  // Callbacks
  void button_changed(int pin, int value);
  void timer_expired();

  // User controlled setup
  void user_init();

  // Macros for client users to not have to implement callbacks
  #define TIMER_OFF  void timer_expired(){}
  #define BUTTON_OFF void button_changed(int pin, int value){}
#ifdef __cplusplus
}
#endif
