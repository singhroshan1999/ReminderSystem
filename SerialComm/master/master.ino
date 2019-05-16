//#include<SoftwareSerial.h> //Included SoftwareSerial Library
//SoftwareSerial s(3,1);
//#define CODE_A 0xA0
//#define CODE_B 0xB0
//int a = 0;
//int b = 0;
String s;
int count = 0;

void setup() {
  Serial.begin(4800);
delay(2000);
s = String("$${\"tok\":")+String(45)+String("}");
pinMode(0,INPUT_PULLUP);
}

void loop() {

if(Serial.availableForWrite() > 62 && count< s.length()){
  Serial.print(s[count]);
  count++;
  delay(10);
}
if(count>= s.length()){
  s = String("$${\"tok\":")+String(45+count)+String("}");
  count = 0;
  
}
}
