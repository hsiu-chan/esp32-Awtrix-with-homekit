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
uint8_t arr[8]={3,4,5,6,7,8,9,10};
uint8_t i=0;

void setup() {
  Serial.begin(9600);

  
  /*pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(arr[3],INPUT);
  pinMode(arr[4],INPUT);
  pinMode(arr[5],INPUT);
  pinMode(arr[6],INPUT);
  pinMode(arr[7],INPUT);*/
  //pinMode(arr[8],OUTPUT);
  Serial.println("setting done");


}

void loop() {
  //int sensorValue = analogRead(A0);
  Serial.println(arr[i]);
  Serial.println(analogRead(arr[i]));
    

  digitalWrite(arr[i], HIGH);
  i+=1;
  i%=8;

  delay(50000);


}
