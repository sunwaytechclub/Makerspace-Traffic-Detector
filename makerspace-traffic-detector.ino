#include "SIGFOX.h"

static const String device = "2C2F3A";
static const bool useEmulator = false;
static const bool echo = true;
static const Country country = COUNTRY_SG;
static UnaShieldV2S transceiver(country, useEmulator, device, echo);

const long transmitCycle = 60;
int triggerCountdownThreshold = 120;
int triggerCountdown = triggerCountdownThreshold;
int triggerCounter = 0;
long countdown = transmitCycle * 1000;
int lastState = 1;
unsigned long lastMillis = 0;
unsigned long currentCycle = 0;

int blockingReset = 2000;
int blockingCounter = blockingReset;

void setup() {
  
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, INPUT);
  Serial.begin(9600);
  if (!transceiver.begin()) stop(F("Unable to init Sigfox module, may be missing"));
  
}

void loop() {

  unsigned long currentMillis = millis();
  currentCycle = currentMillis - lastMillis;
  countdown -= currentCycle;
  lastMillis = currentMillis;

  // Serial.println(countdown);

  // Serial.println(countdown);

  int reading = digitalRead(13);
  digitalWrite(8, reading);
  // Serial.println(reading);

  if(reading == HIGH) {
    triggerCountdown -= currentCycle;
    if(triggerCountdown <= 0) {
      if(lastState) {
        lastState = 0;
        triggerCounter += 1;
        Serial.println("Trigger");
        // Serial.println(triggerCounter);
        digitalWrite(9, HIGH);
      } else {
        blockingCounter -= currentCycle;
        if(blockingCounter <= 0){
          blockingCounter = blockingReset;
          lastState = 1;
          Serial.println("Reset");
          digitalWrite(9, LOW);
        }
      }
    }
  } else {
    triggerCountdown = triggerCountdownThreshold;
    if(!lastState) {
      lastState = 1;
      Serial.println("Untrigger");
      digitalWrite(9, LOW);
    }
  }

  if(countdown <= 0){
//    Message msg(transceiver);
//    msg.addField("ctr", triggerCounter);
//    if(msg.send()) {
//      // Serial.println("Success send");
//      triggerCounter = 0;
//      countdown = transmitCycle * 1000;
//    }
    transceiver.sendString(String(triggerCounter));
    triggerCounter = 0;
    countdown = transmitCycle * 1000;
  }
  
  // delay(clockCycle);
  
}
