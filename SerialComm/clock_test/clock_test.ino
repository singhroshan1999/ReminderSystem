#include <LiquidCrystal.h>
#include "Timer.h"
Timer t;
int i = 0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  t.every(5000,call,5);
  analogWrite(9,100);
analogWrite(10,100);

}

void loop() {
  // put your main code here, to run repeatedly:
t.update();
}

void call(){
  lcd.setCursor(0,0);
  lcd.print(i++);
}
