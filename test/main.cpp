#include<Arduino.h>

uint8_t arr[8]={3,4,5,6,7,8,9,10};
uint8_t i=0;

void setup() {
  Serial.begin(9600);
  for(uint8_t* e=arr;e;e++){
    pinMode(*e,OUTPUT);
  }
}

void loop() {
  //int sensorValue = analogRead(A0);

  digitalWrite(i, HIGH);
  Serial.println(i);

  delay(50000);


}
