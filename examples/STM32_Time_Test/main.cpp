
/* 
 * TimeSerial.pde
 * example code illustrating Time library set through serial port messages.
 *
 * Messages consist of the letter T followed by ten digit time (as seconds since Jan 1 1970)
 * you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
 T1357041600  
 *
 * A Processing example sketch to automatically send the messages is included in the download
 * On Linux, you can use "date +T%s\n > /dev/ttyACM0" (UTC time zone)
 */ 

#include <Arduino.h>
#include <TimeLib.h>

#define Monitor Serial
#define LED PA1
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

void digitalClockDisplay();
void printDigits(int digits);
void processSyncMessage();
time_t requestSync();


void setup()  {

  Monitor.begin(115200);  // serial USB
  
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);
  delay(500);  
  digitalWrite(LED, HIGH);
  delay(500);  
  digitalWrite(LED, LOW);
  delay(500);  
  digitalWrite(LED, HIGH);
  delay(500);  
  digitalWrite(LED, LOW);
  delay(500);  
  digitalWrite(LED, HIGH);
  delay(500); 
  digitalWrite(LED, LOW);
  delay(1000);  

  setSyncProvider(requestSync);  //set function to call when sync required
}

void loop(){    
  if (Monitor.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  if (timeStatus() == timeSet) {
    digitalWrite(LED, LOW); // LED on if synced
  } else {
    digitalWrite(LED, HIGH);  // LED off if needs refresh
  }
  delay(5000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Monitor.print(hour());
  printDigits(minute());
  printDigits(second());
  Monitor.print(" ");
  Monitor.print(day());
  Monitor.print(" ");
  Monitor.print(month());
  Monitor.print(" ");
  Monitor.print(year()); 
  Monitor.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Monitor.print(":");
  if(digits < 10)
    Monitor.print('0');
  Monitor.print(digits);
}


void processSyncMessage() {
  unsigned long pctime = 0;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  int count = 0, inChar = 0;
  do {
    if (Monitor.available() > 0) {
        inChar = Monitor.read();
    if (inChar == 10) break;
    if (inChar > 47 && inChar < 58) {
          inChar -=48;
          Monitor.print(inChar);
          pctime *= 10;
          pctime += inChar;
          count += 1;
      }
    }
    } while (count < 10); 
    // if you get a newline, print the string, then the string's value:
   
      Monitor.print(" PC_Time =  ");
      Monitor.println(pctime);

     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     } else setTime(DEFAULT_TIME);
}

time_t requestSync()
{
  Monitor.println("Waiting for sync message.......->");
  Monitor.print("-->");
  //Monitor.write("-->");  
  return 0; // the time will be sent later in response to serial mesg
}
