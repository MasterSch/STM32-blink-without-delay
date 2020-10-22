//    STM32F103C8T6
//    Serial Monitor an Serial

#include <Arduino.h>

#define debug true
#define Monitor Serial
#define LED PA1


unsigned long secsSince1900, secsSince1970;

void PrintunixTime(unsigned long *secsSince1970);
void ConvertNTPtoUnix(unsigned long *secsSince1900, unsigned long *secsSince1970);


void setup() {

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


}


void loop() {

  const unsigned long timestamps[] = {236546ul, 13456754ul, 243256ul};

  for (byte i = 0; i < (sizeof(timestamps) / sizeof(timestamps[0])); i++) 
  {
    Serial.print(i, DEC);
    Serial.println("");
    secsSince1900 = timestamps[i];
    Serial.println();
    PrintNtpTime(&secsSince1900);
  }

// wait ten seconds before asking for the time again

delay(30000);

}     //  loop end-----------------------------------------------------


void PrintunixTime(unsigned long *secsSince1970) {
    unsigned long days, year, days4years;
    int leapyear = 0, Tag, Monat, Jahr, Wochentag;
    const int monat[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const char* month[13] = {"0", "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
    const char* wheekday[7] = {"Donnerstag", "Freitag", "Samstag", "Sonntag", "Montag", "Dienstag", "Mittwoch"};

    // adjust time zone
    secsSince1970 += 7200;  // + 2 Stunden MESZ+1
    Monitor.print("Unix with corrected time zone = ");
    Monitor.println(*secsSince1970);
    // print the hour, minute and second:
    Monitor.print("The UTC + 2 time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Monitor.print((*secsSince1970  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Monitor.print(':');
    if (((*secsSince1970 % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Monitor.print('0');
    }
    Monitor.print((*secsSince1970  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Monitor.print(':');
    if ((*secsSince1970 % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Monitor.print('0');
    }
    Monitor.println(*secsSince1970 % 60); // print the second
    Monitor.println();
    Monitor.println();
    
    days = *secsSince1970 / 86400L;
    Wochentag = days % 7;
    //Monitor.println(days); // print the second
    Monitor.print("Tage seit 1.1.1970: ");
    Monitor.println(days);
    Monitor.println();
    // 1.1.1970
    year = 1970;
    leapyear = 0;    
    days4years = 1460;
    
    do {
       leapyear = 0;
       if (((year+2) % 4 == 0  && (year+2) % 100 !=0) || ((year+2) % 400 == 0 ))   
       leapyear = 1;
       if (days >= (days4years + leapyear))
       {
         days = (days - days4years - leapyear);
         year += 4;
       } else break;
      } while(1);
       leapyear = 0; 
    if (days >= 365) 
      { days -= 365;
        year +=1;
      } 
    if (days >= 365) 
      { days -= 365;
        year +=1; 
      } 
    // Resttage Jahr + 2 Schaltjahr?
     if ((year % 4 == 0  && year % 100 !=0) || (year % 400 == 0 ))   
       leapyear = 1;
     if (days > (364 + leapyear))
       { days = (days - 365 - leapyear);
         year+=1;
       }
    Jahr = year;
    uint temp = 1;
    Monat = 1; 
    do {
    temp = monat[Monat];
    if ((Monat == 2) && (leapyear == 1)) temp++; 
    if (days >= temp) days -= temp;
    else break; 
    Monat++;
    } while (Monat < 12);
    
    Monitor.print("Schaltjahr: ");
    Monitor.println(leapyear);
    Monitor.print("Jahr: ");
    Monitor.println(Jahr);
    Monitor.print("Monat: ");
    Monitor.println(month[Monat]);
    Monitor.print("Tag: ");
    Monitor.println(days+1);
    Monitor.print("Wochentag: ");
    Monitor.println(wheekday[Wochentag]);
    Monitor.println();
}

void ConvertNTPtoUnix(unsigned long *secsSince1900, unsigned long *secsSince1970)
{
    // const unsigned long seventyYears = 2208988800UL;
    *secsSince1970 = *secsSince1900 - 2208988800UL;
}

/*
vorher
void PrintunixTime(unsigned long *secsSince1900) {
    unsigned long days, year, days4years;
    int leapyear = 0, Tag, Monat, Jahr, Wochentag;
    const int monat[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const char* month[13] = {"0", "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
    const char* wheekday[7] = {"Donnerstag", "Freitag", "Samstag", "Sonntag", "Montag", "Dienstag", "Mittwoch"};

    Monitor.print("Seconds since Jan 1 1900 = ");
    Monitor.println(*secsSince1900);

    // now convert NTP time into everyday time:
    Monitor.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = *secsSince1900 - seventyYears;
    // print Unix time:
    Monitor.println(epoch);
    // adjust time zone
    epoch += 7200;  // + 2 Stunden MESZ+1
    Monitor.print("Unix with corrected time zone = ");
    Monitor.println(epoch);
    // print the hour, minute and second:
    Monitor.print("The UTC + 2 time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Monitor.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Monitor.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Monitor.print('0');
    }
    Monitor.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Monitor.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Monitor.print('0');
    }
    Monitor.println(epoch % 60); // print the second
    Monitor.println();
    Monitor.println();
    
    days = epoch / 86400L;
    Wochentag = days % 7;
    //Monitor.println(days); // print the second
    Monitor.print("Tage seit 1.1.1970: ");
    Monitor.println(days);
    Monitor.println();
    // 1.1.1970
    year = 1970;
    leapyear = 0;    
    days4years = 1460;
    
    do {
       leapyear = 0;
       if (((year+2) % 4 == 0  && (year+2) % 100 !=0) || ((year+2) % 400 == 0 ))   
       leapyear = 1;
       if (days >= (days4years + leapyear))
       {
         days = (days - days4years - leapyear);
         year += 4;
       } else break;
      } while(1);
       leapyear = 0; 
    if (days >= 365) 
      { days -= 365;
        year +=1;
      } 
    if (days >= 365) 
      { days -= 365;
        year +=1; 
      } 
    // Resttage Jahr + 2 Schaltjahr?
     if ((year % 4 == 0  && year % 100 !=0) || (year % 400 == 0 ))   
       leapyear = 1;
     if (days >= (365 + leapyear))
       { days = (days - 365 - leapyear);
         year+=1;
       }
    Jahr = year;
    int temp = 1;
    Monat = 1; 
    do {
    temp = monat[Monat];
    if ((Monat == 2) && (leapyear == 1)) temp++; 
    if (days >= temp) days -= temp;
    else break; 
    Monat++;
    } while (Monat < 12);
    
    Monitor.print("Schaltjahr: ");
    Monitor.println(leapyear);
    Monitor.print("Jahr: ");
    Monitor.println(Jahr);
    Monitor.print("Monat: ");
    Monitor.println(month[Monat]);
    Monitor.print("Tag: ");
    Monitor.println(days+1);
    Monitor.print("Wochentag: ");
    Monitor.println(wheekday[Wochentag]);
    Monitor.println();
}





/*
void PrintNtpTime(unsigned long *secsSince1900) {
    unsigned long days, year, days4years;
    int leapyear = 0, Tag, Monat, Jahr, Wochentag;
    const int monat[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const char* month[13] = {"0", "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
    const char* wheekday[7] = {"Donnerstag", "Freitag", "Samstag", "Sonntag", "Montag", "Dienstag", "Mittwoch"};

    Monitor.print("Seconds since Jan 1 1900 = ");
    Monitor.println(*secsSince1900);

    // now convert NTP time into everyday time:
    Monitor.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = *secsSince1900 - seventyYears;
    // print Unix time:
    Monitor.println(epoch);
    // adjust time zone
    epoch += 7200;  // + 2 Stunden MESZ+1
    Monitor.print("Unix with corrected time zone = ");
    Monitor.println(epoch);
    // print the hour, minute and second:
    Monitor.print("The UTC + 2 time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Monitor.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Monitor.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Monitor.print('0');
    }
    Monitor.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Monitor.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Monitor.print('0');
    }
    Monitor.println(epoch % 60); // print the second
    Monitor.println();
    Monitor.println();
    
    days = epoch / 86400L;
    Wochentag = days % 7;
    //Monitor.println(days); // print the second
    Monitor.print("Tage seit 1.1.1970: ");
    Monitor.println(days);
    Monitor.println();
    // 1.1.1970
    year = 1970;
    leapyear = 0;
    if (days > 364) 
      { days -= 365;
        year +=1; 
      }
    if (days > 364) 
      { days -= 365;
        year +=1;
      } 
    // 1.1.1972      
    days4years = 1460;
    
    do {
       leapyear = 0;
       if ((year % 4 == 0  && year % 100 !=0) || (year % 400 == 0 ))   
       leapyear = 1;
       if (days >= (days4years + leapyear))
       {
         days = (days - days4years - leapyear);
         year += 4;
       } else break;
      } while(1);
    // Resttage
    if (days >= (365 + leapyear)) 
      { days -= (365 - leapyear);
        year +=1; 
      }
    if (days >= 365) 
      { days -= 365;
        year +=1;
      } 
    if (days >= 365) 
      { days -= 365;
        year +=1; 
      }  
    Jahr = year;
    int temp = 1;
    Monat = 1; 
    do {
    temp = monat[Monat];
    if ((Monat == 2) && (leapyear == 1)) temp++; 
    if (days >= temp) days -= temp;
    else break; 
    Monat++;
    } while (Monat < 12);
    
    Monitor.print("Schaltjahr: ");
    Monitor.println(leapyear);
    Monitor.print("Jahr: ");
    Monitor.println(Jahr);
    Monitor.print("Monat: ");
    Monitor.println(month[Monat]);
    Monitor.print("Tag: ");
    Monitor.println(days+1);
    Monitor.print("Wochentag: ");
    Monitor.println(wheekday[Wochentag]);
    Monitor.println();
}
*/
