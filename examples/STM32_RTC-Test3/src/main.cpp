////////////////////////////////////////////////////////
// (C) 2019 by Oliver Kuhlemann                       //
// Bei Verwendung freue ich mich über Namensnennung,  //
// Quellenangabe und Verlinkung                       //
// Quelle: http://cool-web.de/arduino/                //
////////////////////////////////////////////////////////

// uses https://github.com/rogerclarkmelbourne/Arduino_STM32/tree/master/STM32F1/libraries/RTClock

#include <Arduino.h>
#include <RTClock.h>
#include "DateTime.h"
#include "RTCInterface.h"

//#define internLED PC13
#define internLED PA1

// den genauen 32768 Hertz-Timer für die Uhr benutzen (LSE),
// auch ist nur der LSE durch VBat abgesichert
RTClock rtclock (RTCSEL_LSE); 

void irq_alarm() {
  Serial.println ("\nALARM!!!\n");
}

void irq_alarmzeit() {
  Serial.println ("\nIT'S TEA TIME!!!\n");
}

void setup() {
  char buf[25];

  Serial.begin(115200);

  pinMode(internLED, OUTPUT);
  
  delay(1000); // auf die ser. Schnittstelle warten

  Serial.println (F("Compile-DateTime:"));

  Serial.println (__DATE__);
  Serial.println (__TIME__);

  DateTime compdat;
  DateTime rtcdat;

  compdat = getCompileDateTime();

  storeDateTimeInString (compdat, buf, false); // Compile-Datum ausgeben
  Serial.println (buf);

  storeDateTimeInString (compdat, buf, true); // Compile-Datum mit Wochentag
  Serial.println (buf);

  uint64_t ts64;
  ts64=getDateTimeAsUint64(compdat); // Compile-Datum und Zeit als Zahl
  Serial.println (ts64);

  uint32_t ts32;
  ts32=getDateAsUint32(compdat); // Compile-Datum als Zahl
  Serial.println (ts32);

  ts32=getTimeAsUint32(compdat); // Compile-Zeit als Zahl
  Serial.println (ts32);

  Serial.println (F("\nRTC-DateTime:"));
  rtcdat = getRtcDateTime(rtclock);

  storeDateTimeInString (rtcdat, buf, true); // RTC-Datum ausgeben
  Serial.println (buf);



  if ( getDateAsUint32(rtcdat) < getDateAsUint32(compdat)) { // RTC-Zeit aus der Vergangenheit, kann nicht stimmen
    // Kopilierungszeit in Echtzeituhr speichern
    setRtcDateTime (rtclock, compdat);
    Serial.println (F("RTC-Uhrzeit auf Kompilierungszeit gesetzt."));
    // ansonsten mit der Zeit in der RTC weitermachen
  }


  // Alarm setzen in 30 Sekunden
  rtclock.createAlarm(irq_alarm, (rtclock.getTime() + 30));

  // Alarm setzen zu einer bestimmten Zeit
  DateTime datAlarm;
  datAlarm=getRtcDateTime (rtclock);
  datAlarm.hour=17; datAlarm.minute=0; datAlarm.second=0;
  rtclock.createAlarm(irq_alarmzeit, (timetFromDateTime(rtclock, datAlarm)));
}

void loop() {
  DateTime rtcdat;
  char buf[25];
  
  time_t tt;  // Sekunden seit 01.01.1970
  // tm_t mtt;   // struct (year (+1970), month, day, weekday, weekday, pm, hour, minute, second

  if ( Serial.available() > 14 ) {  // 20190515-091017 // setzen der Uhrzeit per Ser-Schnittstelle

    DateTime serdat;
    for (byte i = 0; i < 15; i++) {
      buf[i] = Serial.read();
    }
    Serial.flush();
    buf [15] =0;
    sscanf (buf, "%4d%2d%2d-%2d%2d%2d", &serdat.year, &serdat.month, &serdat.day, &serdat.hour, &serdat.minute, &serdat.second);
    setRtcDateTime (rtclock, serdat);

  }

  tt = rtclock.getTime();
  Serial.print(tt);
  Serial.print("     ");
    
  rtcdat = getRtcDateTime(rtclock);

  storeDateTimeInString (rtcdat, buf, true); // RTC-Datum mit Wochentag
  Serial.println (buf);

  delay (1000);
  digitalWrite (internLED, !digitalRead(internLED));
}
