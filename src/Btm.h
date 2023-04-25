#include "Arduino.h"
#include "OneButton.h"

class Btm
{
private:
    //CRGB* led_arr;


public:
    Btm(uint8_t* pins);
    void renew();
    //void clear();
    OneButton RightBtm, LeftBtm, UpBtm, DownBtm, BackBtm,OkBtm;
    //void fill();
};
