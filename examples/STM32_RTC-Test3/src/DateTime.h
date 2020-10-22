////////////////////////////////////////////////////////
// (C) 2019 by Oliver Kuhlemann                       //
// Bei Verwendung freue ich mich über Namensnennung,  //
// Quellenangabe und Verlinkung                       //
// Quelle: http://cool-web.de/arduino/                //
////////////////////////////////////////////////////////

/*
  struct DateTime {
  uint16_t year=0;
  uint8_t month=0;
  uint8_t day=0;
  uint8_t hour=0;
  uint8_t minute=0;
  uint8_t second=0;
  uint8_t weekday=0;
};
*/

struct DateTime {
  int year=0;
  int month=0;
  int day=0;
  int hour=0;
  int minute=0;
  int second=0;
  int weekday=0;
};


static const char *weekDays[7] = {
  "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

static const char *monthNames[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

uint8_t calcWeekday (int d, int m, int y) {
  uint8_t w;

  if (m < 3) y -= 1;
  w = ((d + (int) (2.6 * ((m + 9) % 12 + 1) - 0.2) + y % 100 + (int) (y % 100 / 4) + (int) (y / 400) - 2 * (int) (y / 100) - 1) % 7 + 7) % 7 + 1;

  return w;
}


DateTime getCompileDateTime () {
  // __DATE__ = May  9 2019

  DateTime dat;
  char monthName[20];
  int day = 0;
  int month = 0;
  int year = 0;
  char compDate[13];

  strncpy(compDate, __DATE__, 12);
  sscanf (compDate, "%s %2d %4d", monthName, &day, &year);

  for (int i = 0; i < 12; i++) {
    if (strncmp (monthNames[i], monthName, 3) == 0) {
      month = i + 1;
      break;
    }
  }

  dat.year = year;
  dat.month = month;
  dat.day = day;


  // __TIME__ = 20:56:25
  int hour = 0;
  int minute = 0;
  int second = 0;

  sscanf (__TIME__, "%2d:%2d:%2d", &hour, &minute, &second);

  dat.hour = hour;
  dat.minute = minute;
  dat.second = second;

  dat.weekday = calcWeekday (dat.day, dat.month, dat.year);

  return dat;
}


void storeDateTimeInString (DateTime dat, char *buf, boolean withWeekday) { // buf min. 20 bytes, mit Weekday mit 25
  if (withWeekday) {
    snprintf (buf, 25, "%s, %d-%02d-%02d %02d:%02d:%02d",
              (dat.weekday <1 || dat.weekday >7) ? "???" : weekDays[dat.weekday - 1],
              dat.year, dat.month, dat.day,
              dat.hour, dat.minute, dat.second);
  } else {
    snprintf (buf, 20, "%d-%02d-%02d %02d:%02d:%02d",
              dat.year, dat.month, dat.day,
              dat.hour, dat.minute, dat.second);
  }

}

uint32_t getDateAsUint32 (DateTime dat) { // gibt z. B. 20190510 zurück, alles Ziffern YMD
  uint32_t ts;

  ts=dat.year*10000;
  ts+=dat.month*100;
  ts+=dat.day;

  return ts;
}


uint32_t getTimeAsUint32 (DateTime dat) { // gibt z. B. 20190510155906 zurück, alles Ziffern YMDhms
  uint32_t ts;

  ts=dat.hour*10000;
  ts+=dat.minute*100;
  ts+=dat.second;

  return ts;
}


uint64_t getDateTimeAsUint64 (DateTime dat) { // gibt z. B. 20190510155906 zurück, alles Ziffern YMDhms
  uint64_t ts;

  ts=dat.year*10000000000;
  ts+=dat.month*100000000;
  ts+=dat.day*1000000;
  ts+=dat.hour*10000;
  ts+=dat.minute*100;
  ts+=dat.second;

  return ts;
}
