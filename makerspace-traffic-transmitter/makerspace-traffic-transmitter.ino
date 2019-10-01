#define CARRIER_PERIOD 26 // 26 us per cycle @ 38kHZ
#define IR_LED_PIN 8
#define DURATION_HIGH 364 //14cycles * 26 us = 364us
#define DURATION_LOW 2*364
#define MESSAGE_LENGTH 32 //maximum of 31bits per message, last bit is stop bit.
#define TIMEOUT_LENGTH 50 //50ms between each message

unsigned long IRCode = 0xFFFFFFFF;

void setup()
{
  
}

void IRSetup()
{
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, LOW);
}


//sends 38kHz carrier signal for a specified duration in milliseconds

void IRCarrier(unsigned int duration)
{
  for(int i=0; i < (convertMicrosToCycles(duration)); i++)
  {
    digitalWrite(IR_LED_PIN, HIGH);
    digitalWrite(IR_LED_PIN, LOW);
    delayMicroseconds(18);
  }  
}


//Converts duration in microseconds to number of cycles with frequency defined by CARRIER_PERIOD
unsigned int convertMicrosToCycles(unsigned int duration)
{
  return(duration/CARRIER_PERIOD);
}

void IRSendMessage(unsigned long message)
{
    for(int i=0; i < MESSAGE_LENGTH; i++)
    {
      IRCarrier(DURATION_HIGH);
      if(message & 0x80000000)
      {
        delayMicroseconds(DURATION_HIGH);
      }
      else
      {
        delayMicroseconds(DURATION_LOW);  
      }
      message = message<<1;
    }
}

void loop()
{
  IRSetup();
  while(1)
  {
    IRSendMessage(IRCode);
    delay(TIMEOUT_LENGTH);
  }
}








