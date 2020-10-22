#include <Arduino.h>
#include <LiquidCrystal.h>
#include "WiFiEsp.h"
#include "WiFiEspUdp.h"
#include <TimeLib.h>

HardwareSerial Serial2(USART2);   // für StM32 !!!!

void sendNTPpacket(char *ntpSrv);
time_t getNtpTime();
time_t localTimeFromUTC(time_t utc);
void digitalLCDClockDisplayklein();
void digitalLCDClockDisplaygross();
void printNumber(int value, int col);
void digitalClockDisplay();
void printDigits(int digits);

const char* monat[12] = {"Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"};
const char* wheekday[7] = {"Son","Mon","Die", "Mit","Don","Fre","Sam"};

char ssid[] = "zuhause";            // your network SSID (name)
char pass[] = "schmitt.obw@web.de";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char timeServer[] = "0.de.pool.ntp.org";  // NTP server
unsigned int localPort = 2390;        // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48;  // NTP timestamp is in the first 48 bytes of the message
const int UDP_TIMEOUT = 2000;    // timeout in miliseconds to wait for an UDP packet to arrive
const int timeZone = 1;     // Central European Time
byte sekAlt = 60;

byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiEspUDP Udp;

const int rs=PB0, en=PB1, db4=PB10, db5=PB11, db6=PB12, db7=PB13;    

LiquidCrystal lcd(rs, en, db4, db5, db6, db7);

// minature digits
// To be used for the Custom characters
PROGMEM unsigned char minidigit[8][8] = {
	{0x18,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x18}, // minature 0
	{0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x03}, // minature 1
	{0x1f,0x1f,0x00,0x00,0x00,0x00,0x1f,0x1f}, // minature 2
	{0x18,0x1c,0x00,0x00,0x00,0x00,0x18,0x1c}, // minature 3
	{0x03,0x07,0x00,0x00,0x00,0x00,0x00,0x00}, // minature 4
	{0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x1f}, // minature 5
	{0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x03}, // minature 6
	{0x1f,0x1f,0x00,0x00,0x00,0x00,0x00,0x00}, // minature 7
};


void setup()
{
 
   // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial2.begin(115200);
  // initialize the lcd 
  lcd.begin(16,2);               
  // Fill in the 8 custom characters with miniture digits
	// so they can be seen in the character set
	for(uint8_t i=0; i < 8; i++)
	lcd.createChar(i, minidigit[i]); // mini digit matching each codepoint
  lcd.home ();                   // go home

  // initialize ESP module
  WiFi.init(&Serial2);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  Udp.begin(localPort);

  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(600);       // 600sek = 10 Min

}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      //digitalClockDisplay();
      digitalLCDClockDisplayklein();
      //digitalLCDClockDisplaygross();
    }
  }
}

void digitalLCDClockDisplayklein()
{
   if (sekAlt > second())
  {  
  lcd.setCursor(0, 0);
  lcd.print(wheekday[weekday()]);
  lcd.print(" ");
  if (day() < 10) lcd.print(" ");
  lcd.print(day());
  lcd.print(". ");
  lcd.print(monat[month()-1]);
  lcd.print(" ");
  lcd.print (year());
  lcd.setCursor(0,1);
  lcd.print("  Zeit: "); 
  if (hour()<10) lcd.print(" "); 
  lcd.print(hour());
  lcd.print(":");
  if(minute()<10) lcd.print("0");
  lcd.print(minute());
  lcd.print(":");
  }
  lcd.setCursor(14,1);
  if(second()<10) lcd.print("0");
  lcd.print(second());
  sekAlt = second();
}

void digitalLCDClockDisplaygross()
{
int stdE, stdZ, minZ, minE;
   if (sekAlt > second())
   {
  if (hour() < 10) {
   stdE = hour();
   lcd.setCursor(0, 0);
   lcd.print("   ");
   lcd.setCursor(0, 1);
   lcd.print("   ");
     }  else  {
    stdE = hour() % 10; 
    stdZ = (hour() - stdE) / 10;
    printNumber( stdZ, 0);
   } 
    printNumber( stdE,3);
if (minute() < 10) {
   minZ = 0;
   minE = minute();
  }  else  {
    minE = minute() % 10; 
    minZ = (minute() - minE) / 10;
   } 
    printNumber( minZ, 7);
    printNumber( minE, 10);

     lcd.setCursor(6,0);
     lcd.print((char)165 );
     lcd.setCursor(6,1);
     lcd.print((char)165);
  }
  lcd.setCursor(14,1);
  if(second()<10) lcd.print("0");
  lcd.print(second());
  sekAlt = second();
}




void printNumber(int value, int col) {

  switch(value) {  

  case 0:
    {
  lcd.setCursor(col, 0); 
  lcd.write(1);  
  lcd.write(7); 
  lcd.write(byte(0));
  lcd.setCursor(col, 1); 
  lcd.write(1);  
  lcd.write(5);  
  lcd.write(byte(0));      
    }
    break;

  case 1:
{
  lcd.setCursor(col,0);
  lcd.write(32);
  lcd.write(byte(0));
  lcd.write(32);
  lcd.setCursor(col,1);
  lcd.write(32);
  lcd.write(byte(0));
  lcd.write(32);
}
    break;

  case 2:
{
  lcd.setCursor(col,0);
  lcd.write(4);
  lcd.write(2);
  lcd.write(byte(0));
  lcd.setCursor(col, 1);
  lcd.write(1);
  lcd.write(5);
  lcd.write(5);
}
    break;

  case 3:
{
  lcd.setCursor(col,0);
  lcd.write(4);
  lcd.write(2);
  lcd.write(byte(0));
  lcd.setCursor(col, 1);
  lcd.write(6);
  lcd.write(5);
  lcd.write(byte(0)); 
}
    break;

  case 4:
{
  lcd.setCursor(col,0);
  lcd.write(1);
  lcd.write(5);
  lcd.write(byte(0));
  lcd.setCursor(col, 1);
  lcd.write(32);
  lcd.write(32);
  lcd.write(byte(0));
}
    break;

  case 5:
{
  lcd.setCursor(col,0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.setCursor(col, 1);
  lcd.write(6);
  lcd.write(5);
  lcd.write(byte(0));
}
    break;

  case 6:
{
  lcd.setCursor(col,0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.setCursor(col, 1);
  lcd.write(1);
  lcd.write(5);
  lcd.write(byte(0));
}
    break;

  case 7:
{
  lcd.setCursor(col,0);
  lcd.write(1);
  lcd.write(7);
  lcd.write(byte(0));
  lcd.setCursor(col, 1);
  lcd.write(32);
  lcd.write(32);
  lcd.write(byte(0));
}
    break;

  case 8:
{
  lcd.setCursor(col, 0); 
  lcd.write(1);  
  lcd.write(2); 
  lcd.write(byte(0));
  lcd.setCursor(col, 1); 
  lcd.write(1);  
  lcd.write(5);  
  lcd.write(byte(0));
}
    break;
    
  case 9:
{
  lcd.setCursor(col, 0); 
  lcd.write(1);  
  lcd.write(2); 
  lcd.write(byte(0));
  lcd.setCursor(col, 1); 
  lcd.write(6);  
  lcd.write(5);  
  lcd.write(byte(0));
      }
    break;        
  }      
}  

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(" Heute ist ");
  Serial.print(wheekday[weekday()]);
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.print(" ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(" Uhr ");
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

// wird nur bei sync mit syncprovider aufgerunfen, hier 1x pro Stunde
time_t getNtpTime()   
{

 sendNTPpacket(timeServer); // send an NTP packet to a time server
  
  // wait for a reply for UDP_TIMEOUT miliseconds
  unsigned long startMs = millis();
  while (!Udp.available() && (millis() - startMs) < UDP_TIMEOUT) {}

  Serial.println(Udp.parsePacket());
  if (Udp.parsePacket()) {
    Serial.println("packet received");
    // We've received a packet, read the data from it into the buffer
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = ");
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);

// nach European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// https://forum.arduino.cc/index.php?topic=172044.msg1278536#msg1278536
// Param: UTC-Unixtime
// Return local-Unixtime  
time_t utc = epoch;
const uint8_t tz_utc = 0;
uint16_t y = year(utc);
uint8_t m,h,d;
  m = month(utc);
  h = hour(utc);
  d = day(utc);
  if (m < 3 || m > 10) return utc + 3600; // +1h
  if (m > 3 && m < 10) return utc + 7200; // +2h
  if (((m == 3) && ((h +24 * d) >= (1+ tz_utc + 24 * (31 - (5 * y / 4 +4) % 7))))
    || ((m == 10) && ((h +24 * d) < (1+ tz_utc + 24 * (31 - (5 * y / 4 +1) % 7))))) return utc + 7200;
  else return utc + 3600; 

  }
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(char *ntpSrv)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)

  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(ntpSrv, 123); //NTP requests are to port 123

  Udp.write(packetBuffer, NTP_PACKET_SIZE);

  Udp.endPacket();
}

