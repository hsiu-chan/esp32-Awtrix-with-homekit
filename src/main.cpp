#include "Arduino.h"
#include "Btm.h"
#include "GetTime.h"


#include "WiFi.h"
#include "OneButton.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>   // 字体一
#include <Wire.h>
#include "HomeSpan.h"         // HomeSpan sketches always begin by including the HomeSpan library

///////

//////
#define DEVICE_SUFFIX          "AWTRIX"

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
Get_time gettime;


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ZIGZAG + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

uint8_t maxBrightness=MATRIX_MAX_BRIGHTNESS;

uint16_t main_color;

/// 按鈕 //////////////////

  void turnoff(){
    maxBrightness=0;
    Serial.println("off");
  }
  void bUp(){
    maxBrightness=maxBrightness>80?100:maxBrightness+10;
    Serial.println("up");
  }
  void bDown(){
    maxBrightness=maxBrightness<20?0:maxBrightness-10;
    Serial.println("down");
  }

/////////////////////

uint16_t HexColor(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}
struct Matrix_RGB : Service::LightBulb {      // Addressable single-wire RGB LED Strand (e.g. NeoPixel)
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness L{100,true};
  uint8_t* maxBrightness;
  uint16_t* main_color;

  
  Matrix_RGB(uint8_t w, uint8_t h,uint8_t* maxBrightness, uint16_t* main_color
) : Service::LightBulb(){

    L.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    this->maxBrightness=maxBrightness;
    this->main_color=main_color;
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    int p=power.getNewVal();
    //*maxBrightness *= p;
    
    
  float h=H.getNewVal<float>();       
  float s=S.getNewVal<float>()/100;       
  float l=L.getNewVal<float>()/200;       

  int r,g,b;

  // C++ Convert HSL to RGB

  // Implement HSL to RGB conversion here...

  // C++ implementation of HSL to RGB conversion
  // Algorithm source: https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB_alternative

  float c = (1 - abs(2 * l - 1)) * s;
  float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
  float m = l - c / 2;

  if (h < 60) {
      r = (c + m) * 255;
      g = (x + m) * 255;
      b = m * 255;
  } else if (h < 120) {
      r = (x + m) * 255;
      g = (c + m) * 255;
      b = m * 255;
  } else if (h < 180) {
      r = m * 255;
      g = (c + m) * 255;
      b = (x + m) * 255;
  } else if (h < 240) {
      r = m * 255;
      g = (x + m) * 255;
      b = (c + m) * 255;
  } else if (h < 300) {
      r = (x + m) * 255;
      g = m * 255;
      b = (c + m) * 255;
  } else {
      r = (c + m) * 255;
      g = m * 255;
      b = (x + m) * 255;
  }
    
    /////////////////////////////

    *main_color=HexColor(r,g,b);
    return(true);  
  }
};
////////////////

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  
  
  // Init matrix
  matrix.begin();
  matrix.fillScreen(0);//清空 
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setFont(&TomThumb);
  matrix.setTextSize(1);

  Serial.print("Matrix Setup done");

  

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pwd);
  Serial.print("WiFi connecting"); 
  matrix.setCursor(2, 5);
  matrix.print("connecting...");
  //当WiFi连接时会返回WL_CONNECTED，因此跳出循环时代表已成功连接
  for (uint8_t i=0;i<8;i++){
    if (WiFi.status()==WL_CONNECTED){
      Serial.println("/n");
      Serial.print("IP地址:");
      Serial.println(WiFi.localIP()); //读取IP地址
      Serial.print("WiFi RSSI:");
      Serial.println(WiFi.RSSI()); //读取WiFi强度
      gettime.adjust_time();
      break;
    }
    Serial.print(".");
    delay(500);   
  }
  /////////////homeSpan////////
  homeSpan.begin(Category::Lighting,"HomeSpan Lighting");
  SPAN_ACCESSORY();                                             // create Bridge (note this sketch uses the SPAN_ACCESSORY() macro, introduced in v1.5.1 --- see the HomeSpan API Reference for details on this convenience macro)
  SPAN_ACCESSORY("Matrix_RGB");
    new Matrix_RGB(MATRIX_WIDTH,MATRIX_HEIGHT,&maxBrightness,&main_color);
 

  
    

  Serial.println("Setup done");
    //i=0;
}









void loop()
{
  cv.renew();
  cv.OkBtm.attachLongPressStart(turnoff);
  cv.UpBtm.attachClick(bUp);
  cv.DownBtm.attachClick(bDown);
  
  matrix.fillScreen(0);//清空 
  
  homeSpan.poll();
  matrix.setTextColor(main_color);
  matrix.setBrightness(maxBrightness);  



  matrix.setCursor(2, 5);
  matrix.print(gettime.show_time());
  for (uint8_t x=2;x<29;x+=4){
    if (x/4==gettime.show_day_of_week()){ //now.dayOfTheWeek()
      matrix.drawLine(x,7,x+2,7,HexColor(0,255,0));
    }
    else{
    matrix.drawLine(x,7,x+2,7,main_color);
    }
  }


  matrix.show();
  
  



  
}






 
