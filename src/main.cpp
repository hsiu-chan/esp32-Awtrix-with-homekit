#include "Arduino.h"
#include "Btm.h"
#include "GetTime.h"


#include "WiFi.h"
//#include <WebServer.h>
#include "OneButton.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>   // 字体一
#include <Wire.h>
#include "HomeSpan.h"         // HomeSpan sketches always begin by including the HomeSpan library
#include "ColorConverter.h"
///////

//////
#define DEVICE_SUFFIX          "AWTRIX"

#define BASE_BTN_COUNT        6

#define MATRIX_WIDTH          32
#define MATRIX_HEIGHT         8

#define NUM_LEDS              256////点阵屏驱动引脚

#define MATRIX_LED_PIN        32

#define MATRIX_MAX_BRIGHTNESS 70//屏幕最大亮度

#define BLACK    0x0000
#define BLUE     0xF800
#define RED      0x001F
#define GREEN    0x07E0
#define CYAN     0xFFE0
#define MAGENTA  0xF81F
#define YELLOW   0x07FF
#define WHITE    0xFFFF
const char* ssid = "WI-FI_2.4G";
const char* password = "hsiu112358";


/// ICONS //////
const uint8_t home_icon[] ={
0x04, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x6c, 0xa0, 0x00, 0x80, 0xc6, 0xae, 0xe6, 0xaa, 0x54, 0xea, 0xea, 0xa3, 0x44, 0xaa, 0xac, 0xca, 0x7c, 0xae, 0xa6, 0xab, 0x00, 0x00, 0x00, 0x00
  };



//////////////////
uint8_t btnPins[BASE_BTN_COUNT] = {25,26,12,14,27,13};
Btm cv(btnPins); //按钮们
Get_time gettime;


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_LED_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ZIGZAG + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

uint8_t maxBrightness=MATRIX_MAX_BRIGHTNESS;

uint16_t main_color;
uint16_t week_color;



//WebServer server(80);


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
















//////////////
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
  float l=L.getNewVal<float>()/100;       

  /////////////////////////////

    *main_color=hsv2hex(h,s,l);
    return(true);  
  }
};


struct Week_day : Service::LightBulb {      // Addressable single-wire RGB LED Strand (e.g. NeoPixel)
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness L{100,true};
  uint8_t* maxBrightness;
  uint16_t* main_color;

  
  Week_day(uint8_t w, uint8_t h,uint8_t* maxBrightness, uint16_t* main_color
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
  float l=L.getNewVal<float>()/100;       

  /////////////////////////////

    *main_color=hsv2hex(h,s,l);
    return(true);  
  }
};
////////////////





/*String write_ssid = "iot";
String write_password = "chosemaker";
int statusCode;
String content;
String room_id = "";*/

/*void createWebServer(){

    server.on("/uid", []() {
        content = "<!DOCTYPE HTML>\r\n<html>";
        content += "<p>";
        content += "<ol>";
        content += room_id;
        content += "</ol>";
        content += "</p><form method='get' action='setuid'><label>UID</label><input name='uid' length=64><input type='submit'></form>";
        content += "</html>";
        server.send(200, "text/html", content);
    });
 
    server.on("/setuid", []() {
        room_id = server.arg("uid");    
        if (room_id.length() > 0) {
          Serial.println("html uid:");
          Serial.println(room_id);
          content = "{\"Success\"}";
          statusCode = 200;
        } else {
          content = "{\"Error\":\"404 not found\"}";
          statusCode = 404;
          Serial.println("Sending 404");
        }
        server.send(statusCode, "application/json", content);
    });
 
}*/






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

  
  /////////////homeSpan////////
  matrix.setCursor(2, 6);
  matrix.fill(0);
  //matrix.print("HomeSpan");
  
  matrix.drawBitmap(0,0,home_icon,MATRIX_WIDTH,MATRIX_HEIGHT,HexColor(255,90,2),HexColor(0,0,0));

  matrix.show();
  //matrix.drawPixel()

  homeSpan.begin(Category::Lighting,"陳時鐘");
  SPAN_ACCESSORY();                                             // create Bridge (note this sketch uses the SPAN_ACCESSORY() macro, introduced in v1.5.1 --- see the HomeSpan API Reference for details on this convenience macro)
  SPAN_ACCESSORY("MainColor");
    new Matrix_RGB(MATRIX_WIDTH,MATRIX_HEIGHT,&maxBrightness,&main_color);
  SPAN_ACCESSORY("WeekDay");
    new Matrix_RGB(MATRIX_WIDTH,MATRIX_HEIGHT,&maxBrightness,&week_color);

  homeSpan.setWifiCredentials(ssid,password);


  ////////////////////////////////

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

  //createWebServer();


 

  
    

  Serial.println("Setup done");
    //i=0;
}







void loop()
{


  
  homeSpan.poll();
  matrix.setTextColor(main_color);
  matrix.setBrightness(maxBrightness);  

  matrix.fillScreen(0);//清空

  matrix.setCursor(2, 6);
  matrix.print(gettime.show_time());
  

  for (uint8_t x=2;x<29;x+=4){
    if (x/4==gettime.show_day_of_week()){ //now.dayOfTheWeek()
      matrix.drawLine(x,7,x+2,7,week_color);
    }
    else{
    matrix.drawLine(x,7,x+2,7,main_color);
    }
  }
  
  


  matrix.show();

  //server.handleClient();
  //sleep(200);
  
  



  
}






 
