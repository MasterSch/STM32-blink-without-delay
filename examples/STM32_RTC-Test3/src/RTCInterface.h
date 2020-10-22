////////////////////////////////////////////////////////
// (C) 2019 by Oliver Kuhlemann                       //
// Bei Verwendung freue ich mich über Namensnennung,  //
// Quellenangabe und Verlinkung                       //
// Quelle: http://cool-web.de/arduino/                //
////////////////////////////////////////////////////////

// uses https://github.com/rogerclarkmelbourne/Arduino_STM32/tree/master/STM32F1/libraries/RTClock

time_t timetFromDateTime (RTClock rtclock, DateTime dat) {
  time_t tt;
  tm_t mtt;

  mtt.year = dat.year-1970;
  mtt.month = dat.month;
  mtt.day = dat.day;
  mtt.hour = dat.hour;
  mtt.minute = dat.minute;
  mtt.second = dat.second;
  mtt.weekday = dat.weekday == 1 ? 7 : dat.weekday-1;

  tt = rtclock.makeTime(mtt);
  return tt;
}

void setRtcDateTime (RTClock rtclock, DateTime dat) {
  time_t tt;

  tt=timetFromDateTime (rtclock, dat);
  rtclock.setTime(tt);  
}

void setRtcDateTime (RTClock rtclock, time_t tt) {
  rtclock.setTime(tt);  
}


DateTime getRtcDateTime (RTClock rtclock) {
  DateTime dat;
  time_t tt;
  tm_t mtt;
  
  tt=rtc_get_count();
   
  rtclock.breakTime(tt, mtt);
         
  dat.year = mtt.year+1970;
  dat.month = mtt.month;
  dat.day = mtt.day;
  dat.hour = mtt.hour;
  dat.minute = mtt.minute;
  dat.second = mtt.second;
  dat.weekday = mtt.weekday == 7 ? 1 : mtt.weekday+1;
  

  return dat;
}


time_t getRtcTimeT (RTClock rtclock) {
  time_t tt;
  
  tt=rtc_get_count();
  
  return tt;
}