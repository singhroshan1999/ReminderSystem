#include<FS.h>
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
ESP8266WebServer serv(80);
void code();
void web();
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
WiFi.begin("Redmi1","12345678");
SPIFFS.begin();
serv.on("/CODE",code);
serv.on("/WEB",web);
serv.onNotFound([](){
  serv.send(404,"NOT FOUND");
});
serv.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
serv.handleClient();
}
void code(){
  File f = SPIFFS.open("/networking2.ino","r");
  serv.send(200,"text/plain",f.readString());
  f.close();
}
void web(){
  File f = SPIFFS.open("/main.html","r");
  serv.send(200,"text/html",f.readString());
  f.close();
}
