#include "SIGFOX.h"

static const String device = "2C2F3A";
static const bool useEmulator = false;
static const bool echo = true;
static const Country country = COUNTRY_SG;
static UnaShieldV2S transceiver(country, useEmulator, device, echo);

const long transmitCycle = 60;
int triggerCountdownThreshold = 50;
int triggerCountdown = triggerCountdownThreshold;
int triggerCounter = 0;
long countdown = transmitCycle * 1000;
int lastState = 1;
unsigned long lastMillis = 0;
long currentCycle = 0;

void setup() {
  
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  Serial.begin(9600);
  if (!transceiver.begin()) stop(F("Unable to init Sigfox module, may be missing"));
  
}

void loop() {

  int currentMillis = millis();
  currentCycle = currentMillis - lastMillis;
  countdown = countdown - currentCycle;
  lastMillis = currentMillis;

  // Serial.println(countdown);

  digitalWrite(12, HIGH);
  int reading = digitalRead(13);
  digitalWrite(12, LOW);
  digitalWrite(8, reading);

  if(reading == LOW) {
    triggerCountdown -= currentCycle;
    if(triggerCountdown <= 0) {
      if(lastState) {
        lastState = 0;
        triggerCounter += 1;
        Serial.println("Trigger");
      }
    }
  } else {
    triggerCountdown = triggerCountdownThreshold;
    if(!lastState) {
      lastState = 1;
      Serial.println("Untrigger");
    }
  }

  if(countdown <= 0){
    Message msg(transceiver);
    msg.addField("ctr", triggerCounter);
    if(msg.send()) {
      Serial.println("Success send");
      triggerCounter = 0;
      countdown = transmitCycle * 1000;
    }
    // transceiver.sendString(String("{\"ctr\":\"") + triggerCounter + "\"}");
    
  }
  
  // delay(clockCycle);
  
}
