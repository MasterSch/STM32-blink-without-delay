#include <Arduino.h>


HardwareSerial Serial2(USART2);

void setup() {
  // put your setup code here, to run once:
    // initialize digital pin PA1 as an output.

Serial.begin(115200);            //   Ã¼ber USB!!
Serial2.begin(115200);     

delay(5000);
pinMode(PA1, OUTPUT);

Serial2.println("ATE0");   // Echo off
delay(1000);

Serial.println("...start....    enter \"end\" for exit.....");

}

void loop() { // run over and over
byte a = ('\n');

while(1) {
   if (Serial2.available())  {
     a = Serial2.read();
  // Serial.print(" ");  
   if (a == 13) Serial.print("\\r");
   if (a == 10) Serial.println("\\n");
   if (a > 13)  Serial.write(a);
  // Serial.println(" ");
  }
  if (Serial.available()) {
    a = (Serial.read());

     Serial.print(" sende an Serial2:  ");
     Serial.print(a);     // wert dezimal
     Serial.print("   ");
    if (a == 13) Serial.print("\\r");
    if (a == 10) Serial.print("\\n");
    if (a >13) Serial.write(a);     // wert als zeichen
     Serial.println("  ");
     Serial2.write(a);
 //  }
  }
 }
}

