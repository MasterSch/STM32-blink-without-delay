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

u32 utcTime = 1591610378UL;
int timeZone = 1;


#define Monitor Serial
#define LED PA1

void gettime( u32 sec, struct time *t );
void summertime( struct time *t );


void setup() {

  Monitor.begin(115200);  // serial USB
  delay(50000);

  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);
  delay(5000);  
  digitalWrite(LED, HIGH);
  delay(5000);  
  digitalWrite(LED, LOW);
  delay(5000);  
  digitalWrite(LED, HIGH);
  delay(5000);  
  digitalWrite(LED, LOW);
  delay(5000);  
  digitalWrite(LED, HIGH);
  delay(500); 

}


void loop() {
  // put your main code here, to run repeatedly:

  struct time current_time;
  struct time summer_time;

  const char* month[13] = {"0", "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
  const char* wheekday[7] = {"Sonntag","Montag", "Dienstag", "Mittwoch","Donnerstag","Freitag","Samstag"};

  utcTime = utcTime + timeZone * 3600;

  for(;;){
    gettime( utcTime, &current_time );
    summer_time = current_time;
    summertime( &summer_time );
    
    Monitor.print(" Datum:  ");
    Monitor.print(wheekday[summer_time.wday]);
    Monitor.print("  ");
    Monitor.print(summer_time.day);
    Monitor.print(". ");
    Monitor.print(month[summer_time.month]);
    Monitor.print(" ");
    Monitor.println(summer_time.year);
    Monitor.print("Zeit:  ");
    Monitor.print(summer_time.hour);
    Monitor.print(" : ");
    if (summer_time.minute < 10) Monitor.print("0"); 
    Monitor.print(summer_time.minute);
    Monitor.print(" : ");
    if (summer_time.second < 10) Monitor.print("0"); 
    Monitor.print(summer_time.second);
    Monitor.println();

    utcTime += 1000U;  
    delay(10000);    // nach 10 Sekunden nochmals
    }
}


void gettime( u32 sec, struct time *t )
{
  u16 day;
  u8 year;
  u16 dayofyear;
  u8 leap400;
  u8 month;

  t->second = sec % 60;
  sec /= 60;
  t->minute = sec % 60;
  sec /= 60;
  t->hour = sec % 24;
  day = sec / 24;

  t->wday = (day + FIRSTDAY) % 7;		// weekday

  year = FIRSTYEAR % 100;			// 0..99
  leap400 = 4 - ((FIRSTYEAR - 1) / 100 & 3);	// 4, 3, 2, 1

  for(;;){
    dayofyear = 365;
    if( (year & 3) == 0 ){
      dayofyear = 366;					// leap year
      if( year == 0 || year == 100 || year == 200 )	// 100 year exception
        if( --leap400 )					// 400 year exception
          dayofyear = 365;
    }
    if( day < dayofyear )
      break;
    day -= dayofyear;
    year++;					// 00..136 / 99..235
  }
  t->year = year + FIRSTYEAR / 100 * 100;	// + century

  if( dayofyear & 1 && day > 58 )		// no leap year and after 28.2.
    day++;					// skip 29.2.

  for( month = 1; day >= DayOfMonth[month-1]; month++ )
    day -= DayOfMonth[month-1];

  t->month = month;				// 1..12
  t->day = day + 1;				// 1..31
}


void summertime( struct time *t )
{
  u8 hour, day, wday, month;			// locals for faster access

  hour = t->hour;
  day = t->day;
  wday = t->wday;
  month = t->month;

  if( month < 3 || month > 10 )			// month 1, 2, 11, 12
    return;					// -> Winter

  if( day - wday >= 25 && (wday || hour >= 2) ){ // after last Sunday 2:00
    if( month == 10 )				// October -> Winter
      return;
  }else{					// before last Sunday 2:00
    if( month == 3 )				// March -> Winter
      return;
  }
  hour++;					// add one hour
  if( hour == 24 ){				// next day
    hour = 0;
    wday++;					// next weekday
    if( wday == 7 )
      wday = 0;
    if( day == DayOfMonth[month-1] ){		// next month
      day = 0;
      month++;
    }
    day++;
  }
  t->month = month;
  t->hour = hour;
  t->day = day;
  t->wday = wday;
}

