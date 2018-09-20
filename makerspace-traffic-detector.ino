#include "SIGFOX.h"

static const String device = "2C2F3A";
static const bool useEmulator = false;
static const bool echo = true;
static const Country country = COUNTRY_SG;
static UnaShieldV2S transceiver(country, useEmulator, device, echo);

const long transmitCycle = 60;
const int clockCycle = 25;
int triggerCounter = 0;
long countdown = transmitCycle * 1000;
int lastState = 0;
unsigned long lastMillis = 0;

void setup() {
  
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  if (!transceiver.begin()) stop(F("Unable to init Sigfox module, may be missing"));
  
}

void loop() {

  int currentMillis = millis();
  countdown = countdown - clockCycle - (currentMillis - lastMillis);
  lastMillis = currentMillis;

  // Serial.println(countdown);
  
  int reading = analogRead(0);
  // Serial.println(reading);
  analogWrite(9, reading / 4);

  // Check trigger
  if(reading <= 900) {
    if(!lastState){
      lastState = 1;
      triggerCounter += 1;
      Serial.println("Trigger");
    }
  } else {
    if(lastState){
      lastState = 0;
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
  
  delay(clockCycle);
  
}
