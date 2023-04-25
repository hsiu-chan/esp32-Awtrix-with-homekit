
/*
 此程式為 Arduino 內建程式，用來掃描並顯示您所處環境附近的Wi-Fi。
 每5秒鐘掃描一次，並把掃描的結果，印在序列埠螢幕上。
 設定序列堡螢幕的baud rate 為 115200 ，即可看到掃描的結果。
 */
#include "Arduino.h"
#include "WiFi.h"
#include "OneButton.h"
#include "Btm.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>   // 字體一
#include "RTClib.h"
#include <Wire.h>


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

const char ssid[]="WI-FI_2.4G"; //修改為你家的WiFi網路名稱
const char pwd[]="hsiu112358"; //修改為你家的WiFi密碼


uint8_t btnPins[BASE_BTN_COUNT] = {25,26,12,14,27,13};
Btm cv(btnPins);
RTC_Micros rtc;


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ZIGZAG + NEO_MATRIX_PROGRESSIVE,
  NEO_GBR            + NEO_KHZ800);





//int i;

/// @brief  adjust Time ///
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;//格林威治時間,一格3600,GMT+8就是8*3600=28800
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
  //當WiFi連線時會回傳WL_CONNECTED，因此跳出迴圈時代表已成功連線
  for (uint8_t i=0;i<8;i++){
    if (WiFi.status()==WL_CONNECTED){
      Serial.println("/n");
      Serial.print("IP位址:");
      Serial.println(WiFi.localIP()); //讀取IP位址
      Serial.print("WiFi RSSI:");
      Serial.println(WiFi.RSSI()); //讀取WiFi強度
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
  show_time();
  
  
  


  // Wait a bit before scanning again
    //delay(10);
  delay(200);


  
}






 
