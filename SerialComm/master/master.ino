//#include<SoftwareSerial.h> //Included SoftwareSerial Library
//SoftwareSerial s(3,1);
//#define CODE_A 0xA0
//#define CODE_B 0xB0
//int a = 0;
//int b = 0;
String s[] = {"roshan","singh","IT4"};

void setup() {
  Serial.begin(4800);

if(Serial.availableForWrite()>0){

    Serial.print("$$");
for (int i = 0;sizeof(s)/sizeof(s[0])>i;i++){
    Serial.print(s[i]);
    Serial.print("#");
}

Serial.flush();
}
pinMode(0,INPUT_PULLUP);
}

void loop() {

delay(1000);
}
