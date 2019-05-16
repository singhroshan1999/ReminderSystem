#include <ArduinoJson.h>

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//String l = "{",r = "},tok = "\"time\":";
delay(2000);
String input = String("{\"tok\":")+String(Serial.readString())+String("}");

StaticJsonDocument<256> doc;
DeserializationError err = deserializeJson(doc,input);
if(err){
  Serial.println("error");
  Serial.println(err.c_str());
  return;
}

const char* nm = doc["name"];
const char* ln = doc["last"];
int a = doc["tok"];
int b = doc["arr"][0];
int c = doc["arr"][1];
Serial.println(nm);
Serial.println(ln);
Serial.println(a);
Serial.println(b);
Serial.println(c);
}

void loop() {
  // put your main code here, to run repeatedly:

}
