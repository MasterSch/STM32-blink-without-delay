#include <Arduino.h>
#include <time.h>

// These variables store the flash pattern
// and the current state of the LED

int ledPin =  PA1;                  // PC13 onboard LED
int ledState = LOW;               // ledState used to set the LED
unsigned long previousMillis = 0; // will store last time LED was updated

unsigned long OnTime = 200;                 // milliseconds of on-time
unsigned long OffTime = 200;               // milliseconds of off-time




void setup() 
{
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  //Serial.begin(115200);
  //delay(3000);   
}

void loop()
{
  // check to see if it's time to change the state of the LED
  unsigned long currentMillis = millis();
 
  if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
  {
    previousMillis += OnTime;          // on time is over, set new start
    ledState = LOW;                    // Turn it off
    digitalWrite(ledPin, ledState);    // Update the actual LED
    //Serial.println(previousMillis);
  }
  else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
  {
    previousMillis += OffTime;           // off time is over, set new start
    ledState = HIGH;                     // turn it on
    digitalWrite(ledPin, ledState);	     // Update the actual LED
    //Serial.println(previousMillis);
  }
}
