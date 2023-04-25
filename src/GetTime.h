#include "Arduino.h"
#include "RTClib.h"


class Get_time
{
private:
    uint8_t now_month;
    const char* ntpServer = "pool.ntp.org"; 
    long  gmtOffset_sec;
    int   daylightOffset_sec;
    RTC_Micros rtc;

public:
    char timeChar[9];

    Get_time() {
        gmtOffset_sec = 28800L; 
        daylightOffset_sec =0;
    }

    void adjust_time();
    char* show_time();
    uint8_t show_day_of_week(){return rtc.now().dayOfTheWeek();};
};
