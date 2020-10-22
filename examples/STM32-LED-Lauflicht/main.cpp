#include <Arduino.h>

int LED[] = {0,1,2,3,4,5,6,7,16,17}; // PA0-PA7,PB0,PB1

int delayOn = 100, delayOff = 1;

void setup() {
  // put your setup code here, to run once:

Serial.begin(115200);
delay(3000);
Serial.println("Start....");
int i = 0;
do {
pinMode(LED[i], OUTPUT);
digitalWrite(LED[i], 1);  // LED aus
Serial.print(" i = ");
Serial.print(i);
Serial.print(" LED[i] = ");
Serial.println(LED[i]);
i++;
} while (i < 10);

}

void loop() {
  // put your main code here, to run repeatedly:
int i = 0;

do {
digitalWrite(LED[i], 0);
Serial.print(" i = ");
Serial.print(i);
Serial.print(" LED[i] = ");
Serial.println(LED[i]);
delay(delayOn);
digitalWrite(LED[i], 1);
delay(delayOff);
i++;
} while (i < 10);
i--;
i--;
do {
digitalWrite(LED[i], 0);
Serial.print(" i = ");
Serial.print(i);
Serial.print(" LED[i] = ");
Serial.println(LED[i]);
delay(delayOn);
digitalWrite(LED[i], 1);
delay(delayOff);
i--;
} while (i >0);


}
