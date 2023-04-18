
/*
 此程式為 Arduino 內建程式，用來掃描並顯示您所處環境附近的Wi-Fi。
 每5秒鐘掃描一次，並把掃描的結果，印在序列埠螢幕上。
 設定序列堡螢幕的baud rate 為 115200 ，即可看到掃描的結果。
 */
#include "Arduino.h"
#include "WiFi.h"
//#include "Button.h"
#include "OneButton.h"
#include <FastLED.h>
#define BASE_BTN_COUNT        6

//LED矩阵宽
#define MATRIX_WIDTH          32
//LED矩阵高
#define MATRIX_HEIGHT         8
#define NUM_LEDS              256
//点阵屏驱动引脚
#define MATRIX_LED_PIN        32

//屏幕最大亮度
#define MATRIX_MAX_BRIGHTNESS 48

uint8_t btnPins[BASE_BTN_COUNT] = {25,26,12,14,27,13};

//Button* buttons[BASE_BTN_COUNT];



// Setup a new OneButton on pin A1.  
OneButton button1(25, true);
// Setup a new OneButton on pin A2.  
OneButton button2(26, true);

void click1() {
  Serial.println("Button 1 click.");
} // click1


// This function will be called when the button1 was pressed 2 times in a short timeframe.
void doubleclick1() {
  Serial.println("Button 1 doubleclick.");
} // doubleclick1


// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1() {
  Serial.println("Button 1 longPress start");
} // longPressStart1


// This function will be called often, while the button1 is pressed for a long time.
void longPress1() {
  Serial.println("Button 1 longPress...");
} // longPress1


// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1() {
  Serial.println("Button 1 longPress stop");
} // longPressStop1


// ... and the same for button 2:

void click2() {
  Serial.println("Button 2 click.");
} // click2


void doubleclick2() {
  Serial.println("Button 2 doubleclick.");
} // doubleclick2


void longPressStart2() {
  Serial.println("Button 2 longPress start");
} // longPressStart2


void longPress2() {
  Serial.println("Button 2 longPress...");
} // longPress2

void longPressStop2() {
  Serial.println("Button 2 longPress stop");
} // longPressStop2





CRGB leds[MATRIX_WIDTH*MATRIX_HEIGHT];



void setup()
{
    Serial.begin(9600);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Starting TwoButtons...");

    // link the button 1 functions.
    button1.attachClick(click1);
    button1.attachDoubleClick(doubleclick1);
    button1.attachLongPressStart(longPressStart1);
    button1.attachLongPressStop(longPressStop1);
    button1.attachDuringLongPress(longPress1);

    // link the button 2 functions.
    button2.attachClick(click2);
    button2.attachDoubleClick(doubleclick2);
    button2.attachLongPressStart(longPressStart2);
    button2.attachLongPressStop(longPressStop2);
    button2.attachDuringLongPress(longPress2);

    FastLED.addLeds<WS2812Controller800Khz,MATRIX_LED_PIN, GRB>((CRGB* )leds,MATRIX_WIDTH*MATRIX_HEIGHT);
     FastLED.setBrightness(MATRIX_MAX_BRIGHTNESS);


    Serial.println("Setup done");
}

void loop()
{
  button1.tick();
  button2.tick();
  fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0)); // 设置所有LED灯为绿色
  FastLED.show(); // 显示所有LED灯


  // Wait a bit before scanning again
    delay(10);
}
