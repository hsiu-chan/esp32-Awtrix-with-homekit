#include "Arduino.h"
#include "RTClib.h"
#include "GetTime.h"


#define GREEN    0x07E0




void Get_time::adjust_time(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Fail to obtain time from ntp.");
    return;
  }
  rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
  now_month=timeinfo.tm_mon + 1;
}

char* Get_time::show_time(){
  DateTime now = rtc.now();
  
  if (now_month!=now.month()){
    adjust_time();
  }

  
  //sprintf(timeChar, "%02d", i%32);
  
  /*time
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();*/


  /*Screen test
  matrix.drawPixel(i%32,(i/32)%8,CYAN);
  i++;*/
  sprintf(timeChar, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  
  return timeChar;
}
