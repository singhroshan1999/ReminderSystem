#include <LiquidCrystal.h>
#include <ArduinoJson.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int l = 10;
int flag = 0;
StaticJsonDocument<256> doc;
void setup() {

Serial.begin(4800);

analogWrite(9,100);
analogWrite(10,100);
  lcd.begin(16, 2);

lcd.setCursor(0,0);
//lcd.autoscroll();
}

void loop() {
lcd.setCursor(0,0);
  lcd.print(l);
// Serial.println(l);
delay(1000);
}


void serialEvent(){
 if(Serial.find("$$")){

 while(Serial.available()>0){
  
     deserializeJson(doc,Serial.readString());
int i = doc["tok"];
l=i;
 }


 }

}
