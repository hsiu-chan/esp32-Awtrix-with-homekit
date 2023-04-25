#include "Btm.h"
#include "Arduino.h"
#include "OneButton.h"





Btm::Btm(uint8_t* pins)
{
    UpBtm=OneButton(*pins,true);
    RightBtm=OneButton(*(pins+1),true);
    DownBtm=OneButton(*(pins+2),true);
    LeftBtm=OneButton(*(pins+3),true);
    OkBtm=OneButton(*(pins+4),true);
    BackBtm=OneButton(*(pins+5),true);
    //led_arr=leds;
}

/*void Canvas::init_Canvas() {
    //btms[0].attachClick(); // attachClick function is passed in
}*/

void Btm::renew() {
    UpBtm.tick();
    DownBtm.tick(); 
    LeftBtm.tick();
    RightBtm.tick();
    OkBtm.tick();
    BackBtm.tick();
}
