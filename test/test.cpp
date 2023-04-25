#include "Arduino.h"
#include "Btm.h"
#include "GetTime.h"


#include "WiFi.h"
#include "OneButton.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>   // 字体一
#include "RTClib.h"
#include <Wire.h>

///////

//////

#define BASE_BTN_COUNT        6

#define MATRIX_WIDTH          32
#define MATRIX_HEIGHT         8

#define NUM_LEDS              256////点阵屏驱动引脚

#define MATRIX_LED_PIN        32

#define MATRIX_MAX_BRIGHTNESS 48//屏幕最大亮度

#define BLACK    0x0000
#define BLUE     0xF800
#define RED      0x001F
#define GREEN    0x07E0
#define CYAN     0xFFE0
#define MAGENTA  0xF81F
#define YELLOW   0x07FF
#define WHITE    0xFFFF

const char ssid[]="WI-FI_2.4G"; //修改为你家的WiFi网络名称
const char pwd[]="hsiu112358"; //修改为你家的WiFi密码


uint8_t btnPins[BASE_BTN_COUNT] = {25,26,12,14,27,13};
Btm cv(btnPins); //按钮们
//RTC_Micros rtc;
Get_time gettime;


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ZIGZAG + NEO_MATRIX_PROGRESSIVE,
  NEO_GBR            + NEO_KHZ800);

RTC_Micros rtc;




//int i;

/// @brief  adjust Time ///
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;//格林威治时间,一格3600,GMT+8就是8*3600=28800
const int   daylightOffset_sec = 0;
uint8_t now_month;

void adjust_time(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Fail to obtain time from ntp.");
    return;
  }
  rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
  now_month=timeinfo.tm_mon + 1;
}






/// @brief //////////////////

void show_time(){
  DateTime now = rtc.now();
  char timeChar[9];
  if (now_month!=now.month()){
    adjust_time();
  }

  
  //sprintf(timeChar, "%02d", i%32);
  sprintf(timeChar, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  
  
  matrix.setTextColor(matrix.Color(255, 0, 0));
  matrix.setCursor(2, 5);
  matrix.print(timeChar);
  for (uint8_t x=2;x<29;x+=4){
    if (x/4==now.dayOfTheWeek()){
      matrix.drawLine(x,7,x+2,7,GREEN);
    }
    else{
    matrix.drawLine(x,7,x+2,7,matrix.Color(150, 0, 0));
    }
  }
  
  matrix.show();
}



/////////////////////
uint8_t maxBrightness=40;
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pwd);
  Serial.print("WiFi connecting"); 
  //当WiFi连接时会返回WL_CONNECTED，因此跳出循环时代表已成功连接
  for (uint8_t i=0;i<8;i++){
    if (WiFi.status()==WL_CONNECTED){
      Serial.println("/n");
      Serial.print("IP地址:");
      Serial.println(WiFi.localIP()); //读取IP地址
      Serial.print("WiFi RSSI:");
      Serial.println(WiFi.RSSI()); //读取WiFi强度
      adjust_time();
      break;
    }
    Serial.print(".");
    delay(500);   
  }
  
  
  
    

    
  // Init matrix
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(maxBrightness);
  matrix.setFont(&TomThumb);
  matrix.setTextSize(1);





  Serial.println("Setup done");
    //i=0;
}


void turnoff(){
  matrix.setBrightness(0);
  Serial.println("off");
}
void bUp(){
  maxBrightness=maxBrightness>60?100:maxBrightness+20;
  
  matrix.setBrightness(maxBrightness);
  Serial.println("up");
}
void bDown(){
  maxBrightness=maxBrightness<40?0:maxBrightness-20;
  matrix.setBrightness(maxBrightness);
  Serial.println("down");
}

void loop()
{
  cv.renew();
  cv.OkBtm.attachLongPressStart(turnoff);
  cv.UpBtm.attachClick(bUp);
  cv.DownBtm.attachClick(bDown);
  matrix.fillScreen(0);
  /*
  matrix.setTextColor(matrix.Color(255, 0, 0));
  matrix.setCursor(2, 5);
  matrix.print(clock.show_time());
  for (uint8_t x=2;x<29;x+=4){
    if (x/4==1){ //now.dayOfTheWeek()
      matrix.drawLine(x,7,x+2,7,GREEN);
    }
    else{
    matrix.drawLine(x,7,x+2,7,matrix.Color(150, 0, 0));
    }
  }
  
  matrix.show();*/
  
  
  show_time();


  // Wait a bit before scanning again
    //delay(10);
  delay(200);


  
}






 
