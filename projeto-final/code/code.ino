#include <SoftwareSerial.h>
#include <string.h>
#include <math.h>
#include "Servo.h"
#include "env_config.h"
#include "car.c"
#include "reaction.c"
 
SoftwareSerial bluetooth(RX_PIN, TX_PIN);
 
// Transmission info
unsigned long last_show, initial;
boolean receivingCode = true;
boolean recvReact = false;

// Reaction info
Reaction reactions[MAX_REACTIONS];
int nReactions  = 1;
int curReaction = 0;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(BLUETOOTH_BAUD_RATE);
  pinMode(SENSOR_ONE_IN, INPUT);  
  pinMode(SENSOR_ONE_OUT, OUTPUT);   
  pinMode(SENSOR_TWO_IN, INPUT);  
  pinMode(SENSOR_TWO_OUT, OUTPUT);    
  pinMode(LED_RED, OUTPUT);  
  pinMode(LED_BLUE, OUTPUT);
  CAR_setup();
  last_show = millis();

  resetState();
}

void resetState() {
  receivingCode = true;
  recvReact     = true;
  curReaction   = 0;
  nReactions    = 1; // by default, only the main "trail"
  reactions[0]  = REACTION_new();
}

int checkReactions() {
  int i, reactionIdx = 0;
  
  for (i = 1; i < nReactions; i++) {
    // if reaction is dead ignore it
    //if (reactions[i].codePos >= reactions[i].codeLen && reactions[i].loopPos == -1)
    //  continue;

    // Check if car is blocked
    ReactionType type = reactions[i].type;
    if (type == CAR_BLOCKED) {
      int sensorLeft  = sensorDistance(SENSOR_ONE_OUT, SENSOR_ONE_IN);            
      int sensorRight = sensorDistance(SENSOR_TWO_OUT, SENSOR_TWO_IN); 

      if (!sensorLeft || !sensorRight)
        reactionIdx = i;
    }
    else if (type = TIMER) {
      // if wait is -1 this is the first time this trail is executing, so
      // execute it for its `wait` to be set.
      if (reactions[i].wait == -1 || millis() > reactions[i].wait)
        reactionIdx = i;
    }

    // If a reaction was found, leave
    if (reactionIdx)
      break;
  }

  return reactionIdx;
}

void loop() {
  char codeChar;

  // Receive code from app, if any available
  if (bluetooth.available()) {
    if(!receivingCode)
      resetState();

    Reaction* recv = &(reactions[nReactions - 1]);
    codeChar = (char)bluetooth.read();
    if (codeChar == '/')
      receivingCode = false;  
    else if (codeChar == '|') {
      nReactions++;
      reactions[nReactions-1] = REACTION_new();
      recvReact = true;
    }
    else {
      // The first char should set the type of the reaction
      if (recvReact) {
        switch (codeChar) {
          case '1': recv->type = CAR_BLOCKED; break;
          case '2': {
            recv->type = TIMER;
            recv->wait = -1;
            break;
          }
          case '3': recv->type = TEMP; break;
          default:  recv->type = GENERIC;
        }
        recvReact = false;
      } 
      else
        recv->code[(recv->codeLen)++] = codeChar;
    }
  }

#ifdef _DEBUG
  if (millis() > last_show + 1000) {
     Serial.print("code: ");
     Serial.println(code);
     Serial.print("react: ");
     Serial.println(reactCode);
     last_show = millis();
  }
#endif
  
  boolean isAlive, timerFirstRun = false;
  if (!receivingCode) {
    // First, check if we need to execute any reaction
    if (curReaction == 0)
      curReaction = checkReactions();

    // If reaction is a "Timer" and it's running for the first time
    // set the flag so control is returned to main trail
    if (reactions[curReaction].type = TIMER && reactions[curReaction].codePos == 0)
      timerFirstRun = true;

    isAlive = REACTION_runCode(&(reactions[curReaction]));
  }
  
  if (timerFirstRun) {
    curReaction = 0;
  }
  // code has finished, so receive new code
  else if (!isAlive) {
    if (curReaction)
      curReaction = 0; // return to main trail
    else
      resetState();
  }
}

