
#include <Arduino.h>

USBSerial USB;

void setup() {
  // put your setup code here, to run once:
    // initialize digital pin PC13 as an output.

USB.begin(115200);            //   über USB!!
Serial1.begin(115200);     

delay(3000);

pinMode(PC13, OUTPUT);

USB.println("...start....enter   end for exit.....");
Serial1.println("ATE0");   // Echo off

}

void loop() { // run over and over
byte a = ('\n');

while(1) {
   if (Serial1.available())  {
     a = Serial1.read();
  // USB.print(" ");  
   if (a == 13) USB.print("\\r");
   if (a == 10) USB.println("\\n");
   if (a > 13)  USB.write(a);
  // USB.println(" ");
  }
  if (USB.available()) {
    a = (USB.read());

     USB.print(" send  ");
     USB.print(a);     // wert dezimal
     USB.print("   ");
    if (a == 13) USB.print("\\r");
    if (a == 10) USB.print("\\n");
    if (a >13) USB.write(a);     // wert als zeichen
     USB.println("  ");
     Serial1.write(a);
 //  }
  }
 }
}

