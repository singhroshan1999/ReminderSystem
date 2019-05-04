#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {

Serial.begin(4800);

analogWrite(9,100);
analogWrite(10,100);
  lcd.begin(16, 2);

lcd.setCursor(0,0);
//lcd.autoscroll();
}

void loop() {

delay(1000);
}


void serialEvent(){
  lcd.print("                ");
  lcd.setCursor(0,0);
 if(Serial.find("$$")){
 while(Serial.available()>0){
  lcd.print(Serial.readStringUntil("#"));
 }
 }

//  Serial.print(s);
}
