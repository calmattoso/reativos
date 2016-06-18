#ifndef REACTION_H
#define REACTION_H

#include "env_config.h"
#include "car.c"

typedef enum {
  GENERIC     = 0,
  CAR_BLOCKED = 1,
  TIMER       = 2,
  TEMPERATURE = 3
} ReactionType;

typedef struct {
  ReactionType type;
  char code[MAX_CODE];
  int codeLen;
  int codePos;
  int loopPos;
  long wait;
} Reaction;

// Creates and returns a new reaction
Reaction REACTION_new() {
  Reaction r;
    r.type    =  GENERIC;
    r.codeLen =  0;
    r.codePos =  0;
    r.loopPos = -1;
    r.wait    =  0;
  return r;
}

void REACTION_setWait(Reaction* r, unsigned long now, unsigned long interval) {
  r->wait = now + interval;
}

// Executes a reaction code and returns whether its alive
boolean REACTION_runCode(Reaction* r) {
  int i;
  
  // Check for an active `wait`. If one exists, check if it's over. If it is, then
  //   reset the wait state and run the code; otherwise, just return;
  unsigned long now = millis();
  if (r.wait > now)
    return true;
  else
    r.wait = 0;

  if (r->codePos < r->codeLen) { 
      char command[5];
      for (i = 0; i < 4; i++)
        command[i] = curCode[(r->codePos)++];
       command[i] = '\0';
       
      if (strcmp(command, "loop") == 0) {
         r->loopPos = r->codePos;
      }
      else if (strcmp(command, "ledr") == 0) { // red led
        digitalWrite(LED_RED, curCode[(r->codePos)++] == '1' ? HIGH : LOW); 
      }
      else if (strcmp(command, "ledb") == 0) { // blue led
        digitalWrite(LED_BLUE, curCode[(r->codePos)++] == '1' ? HIGH : LOW); 
      }
      else if(strcmp(command,"buzz") == 0) { // buzzer
        int timer;
        sscanf(curCode + r->codePos, "%d", &timer);
        r->codePos += floor(log10(abs(timer))) + 1;
        tone(BUZZER, 1500, timer);     
      } 
      else if(strcmp(command,"wait") == 0) { // buzzer
        int timer;
        sscanf(curCode + r->codePos, "%d", &timer);
        r->codePos += floor(log10(abs(timer))) + 1;
        REACTION_setWait(r, now, timer);
      }
      else if(strcmp(command,"move") == 0) { // buzzer
        if(curCode[(r->codePos)++] == '0')
          setServo(-200, -200);
        else
          setServo(200, 200);
      }
      else if(strcmp(command,"turn") == 0) { // buzzer
        if(curCode[(r->codePos)++] == '0') {
          setServo(-200, 200);
          REACTION_setWait(r, now, 1500);
        }
        else {
          setServo(200, -200);
          REACTION_setWait(r, now, 1500);
        }
        setServo(0,0);
        REACTION_setWait(r, now, 20);
      }
      else if(strcmp(command,"stop") == 0) {
        setServo(0,0);
        REACTION_setWait(r, now, 20);
      }
      
      return true;
  }
  // Go to start of the `loop`
  else if (r->loopPos >= 0) {
    r->codePos = r->loopPos; 
    return true;
  }
  
  return false; // program ended
}

#endif
