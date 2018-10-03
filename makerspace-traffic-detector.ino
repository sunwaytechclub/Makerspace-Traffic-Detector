#include "SIGFOX.h"
#define INPUT_PIN 12

//#define MESSAGE_LENGTH 32
//#define TIMEOUT 50000 + 10000 //timeout length in us, 10ms buffer time

static const String device = "2C2F3A";
static const bool useEmulator = false;
static const bool echo = true;
static const Country country = COUNTRY_SG;
static UnaShieldV2S transceiver(country, useEmulator, device, echo);

const long transmitCycle = 600;
unsigned long lastMillis = 0;
unsigned long currentCycle = 0;
long countdown = transmitCycle * 1000;

//int data[MESSAGE_LENGTH];
unsigned int counter = 0;
unsigned long duration_high = 0;
unsigned long duration_low = 0;
bool blocking_flag =  false;

void setup()
{
  pinMode(INPUT_PIN, INPUT);
  Serial.begin(9600);
  if (!transceiver.begin()) stop(F("Unable to init Sigfox module, may be missing"));
}

unsigned int count_when_blocking()
{
  unsigned long reading;
  int count = 0;
  int blocking_control = 10;

  unsigned int blocking_time_m = 0;
  
  while(blocking_flag)
  {
    while(blocking_control <= 0){
      reading = pulseIn(INPUT_PIN, LOW, 100000);

      if(reading > 0) {
        blocking_control -= 1;
        if(blocking_control <= 0) { 
          blocking_flag = false; 
        }
      } else {
        blocking_control = 10;
        blocking_time_m += 100;
        Serial.println(blocking_time_m);
        if(blocking_time_m >= 2000)
        {
          count += 1;
          blocking_time_m = 0;
        }
      }
    }
    
//    if(reading > 0)
//    {
//      blocking_flag = false;
//    }
//    else
//    {
//      blocking_time_m += 120;
//      Serial.println(blocking_time_m);
//      if(blocking_time_m >= 2000)
//      {
//        count += 1;
//        blocking_time_m = 0;
//      }
//    }
    
  }
  // delay(1000);
  // Serial.println(count);
  
  return count;
}

void loop()
{
  Serial.println("Print");
  unsigned long currentMillis = millis();
  currentCycle = currentMillis - lastMillis;
  countdown -= currentCycle;
  lastMillis = currentMillis;
  
  duration_low = pulseIn(INPUT_PIN, LOW, 100000);
  if(duration_low == 0)
  {
      //detected person, add one to counter
      counter += 1;
      //person blocking signal, set blcking flag
      blocking_flag = true;
      counter += count_when_blocking();
  }

  if(countdown <= 0){
    Serial.println("Sending");
    transceiver.sendString(String(counter));
    counter = 0;
    countdown = transmitCycle * 1000;
  }
  Serial.println(counter);
  Serial.println(countdown);
}
