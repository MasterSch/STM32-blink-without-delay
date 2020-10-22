//    STM32F103C8T6
//    Serial Monitor an Serial
/************************************************************************/
/*									*/
/*			Convert 32 bit to date & time			*/
/*			e.g. from RTC chip DS1994			*/
/*									*/
/*		Author: Peter Dannegger					*/
/*     			danni@specs.de					*/
/*									*/
/************************************************************************/

// 4294967295 sec = 0136 y + 1 m + 6 d + 6 h + 28 min + 15 sec

#include <Arduino.h>

#define FIRSTYEAR	1970     // start year
#define FIRSTDAY	4        // 0 = Sunday  4 = Donnerstag

typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;

struct time {
  u8  second;
  u8  minute;
  u8  hour;
  u8  day;
  u8  month;
  u16 year;
  u8  wday;
};


u8 DayOfMonth[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

u32 utcTime = 1591748196 ;
int timeZone = 1;


#define Monitor Serial
#define LED PA1

void gettime( u32 sec, struct time *t);

void setup() {

  Monitor.begin(115200);  // serial USB

  pinMode(LED, OUTPUT);

  int count = 0;
  do {
  digitalWrite(LED, LOW);
  delay(500);  
  digitalWrite(LED, HIGH);
  delay(500);  
  } while (++count < 25);

}


void loop() {

  struct time current_time;

  const char* month[12] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
  const char* wheekday[7] = {"Sonntag","Montag","Dienstag", "Mittwoch","Donnerstag","Freitag","Samstag"};

  utcTime = utcTime + timeZone * 3600;

  for(;;){
    gettime( utcTime, &current_time);
    Monitor.println();
    Monitor.print(" Datum:  ");
    Monitor.print(wheekday[current_time.wday]);
    Monitor.print("  ");
    Monitor.print(current_time.day);
    Monitor.print(". ");
    Monitor.print(month[current_time.month - 1]);
    Monitor.print(" ");
    Monitor.println(current_time.year);
    Monitor.print(" Zeit:   ");
    Monitor.print(current_time.hour);
    Monitor.print(" : ");
    if (current_time.minute < 10) Monitor.print("0"); 
    Monitor.print(current_time.minute);
    Monitor.print(" : ");
    if (current_time.second < 10) Monitor.print("0"); 
    Monitor.print(current_time.second);
    Monitor.println("  Uhr");

    utcTime += 1000U;  
    delay(10000);    // nach 10 Sekunden nochmals
    }
}


void gettime( u32 sec, struct time *t)
{
  u16 day, year, dayofyear;
  u8 hour, wday, month, summertime;

  t->second = sec % 60;
  sec /= 60;
  t->minute = sec % 60;
  sec /= 60;
  hour = sec % 24;
  day = sec / 24;

  wday = (day + FIRSTDAY) % 7;		// weekday 0 = sunday
  year = FIRSTYEAR;               // 1970
  do {
       dayofyear = 365;
     if ((year % 4 == 0  && year % 100 !=0) || (year % 400 == 0))   
       dayofyear = 366;
     if ( day < dayofyear)
      break;
    day -= dayofyear;
    year++;				
  } while(1);

  if (dayofyear & 1 && day > 58)		// no leap year and after 28.2.
    day++;					// skip 29.2.

  for( month = 1; day >= DayOfMonth[month-1]; month++)
    day -= DayOfMonth[month-1];
    day += 1;
    
    summertime = 1;
  if (month < 3 || month > 10)	 		// month 1, 2, 11, 12 winter
       summertime = 0;   else  {
  if (day - wday >= 25 && (wday || hour >= 2)){ // after last Sunday 2:00
    if (month == 10)				// October -> Winter
      summertime = 0;
  }else{					// before last Sunday 2:00
    if (month == 3)				// March -> Winter
      summertime = 0;
    }
  }
  if (summertime == 1) {
  hour++;					// add one hour
  if (hour == 24){				// next day
    hour = 0;
    wday++;					// next weekday
    if (wday == 7)
      wday = 0;
    if (day == DayOfMonth[month-1])  {		// next month
      day = 0;
      month++;
      }
    day++;
    }
  }
  t->year = year;
  t->month = month;
  t->day = day;
  t->wday = wday;
  t->hour = hour;
}
