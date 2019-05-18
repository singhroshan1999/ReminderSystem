#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include<WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#define DEBUG(x) Serial.println(x);
#define DEBUG2(x) Serial.print(x);

String ssid = "Redmi1";
String pass = "12345678";

unsigned int localPort = 2390;
IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[ NTP_PACKET_SIZE];
int timeArray[] = {0,0,0}; // hour,minuit,second

WiFiUDP udp;

ESP8266WebServer server(80);

//######HEADERS####
void setupWifi();
void setupUDP();
void getNTPTime();

void toggleStopWatch();
long getStopwatch();
long getStopwatchCount();
void togglePauseStopwatch();
bool isPausedStopwatch();
//void unSetStopWatch();

int setAlarm(String hh,String mm,String isAM,String msg,String repeat);
void unSetAlarm(String);

//#################

void setup() {
Serial.begin(115200);
setupWiFi();
setupUDP();
setupWebServer();
SPIFFS.begin();


}

void loop() {
//  getNTPTime();
//  DEBUG2(timeArray[0])
//  DEBUG2(":")
//  DEBUG2(timeArray[1])
//  DEBUG2(":")
//  DEBUG(timeArray[2])
  
  server.handleClient();
  
//  delay(10000);
}

void setupWiFi(){
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    DEBUG2(".")
    delay(500);
  }
//  DEBUG("connected")
}

void sendNTPpacket(IPAddress& address) {
// DEBUG("sending NTP packet...")
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void setupUDP(){
  DEBUG("STARTING UDP")
  udp.begin(localPort);
//  DEBUG("UDP port:")
//  DEBUG2(udp.localPort())
}

void getNTPTime(){
    WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP);
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    ;
//    DEBUG("no packet yet")
  } else {
//    DEBUG("packet received, length=")
//    DEBUG(cb)
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
//    DEBUG2("Seconds since Jan 1 1900 = ")
//    DEBUG(secsSince1900)

//    DEBUG2("Unix time = ")
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
//    DEBUG(epoch)
//    DEBUG2("The UTC time is ")
//    DEBUG2((epoch  % 86400L) / 3600)

    timeArray[0] = (epoch  % 86400L) / 3600; 
    timeArray[1] = (epoch  % 3600) / 60;
    timeArray[2] = epoch % 60;
  }

}

void setupWebServer(){
  server.on("/",handleRequest);
  server.on("/home",[](){
    File f = SPIFFS.open("/index.html","r");
    server.send(200,"text/html",f.readString());
    f.close();
  });
  server.onNotFound([](){
    server.send(404,"404:Not Found");
  });
  server.begin();
}
void setupWifi();
void setupUDP();
void getNTPTime();

void handleRequest(){
//  if(server.hasArg("toggleStopWatch")){
DEBUG("hit /")
  if(server.arg("toggleStopWatch") == "1"){
    DEBUG("hit togglestopwatch")
    toggleStopWatch();
    server.send(200,"text/plain","TSW");
  }
  if (server.arg("getStopwatch") == "1"){
    long count = getStopwatch();
        server.send(200,"text/plain","GSW");

  }
  if (server.arg("getStopwatchCount") == "1"){
    long count = getStopwatchCount();
        server.send(200,"text/plain","GSWC");

  }
  if (server.arg("togglePauseStopwatch") == "1"){
    togglePauseStopwatch();
        server.send(200,"text/plain","TPS");

  }
  if (server.arg("isPausedStopwatch") == "1"){
        server.send(200,"text/plain","IPSW");

  }
  if (server.arg("setAlarm") == "1"){
    int count = setAlarm(server.arg("hh"),server.arg("mm"),server.arg("AM"),server.arg("msg"),server.arg("repeat"));
    Serial.println(server.arg("hh")+server.arg("mm")+server.arg("AM")+server.arg("msg")+server.arg("repeat"));
        server.send(200,"text/plain","SA");

  }
  if (server.arg("unSetAlarm") == "1"){
        server.send(200,"text/plain","USA");

    // do something
  }
//  else {
    
//  }

  //SPIFFS index.html here
  
}

void toggleStopWatch(){}
long getStopwatch(){}
long getStopwatchCount(){}
void togglePauseStopwatch(){}
bool isPausedStopwatch(){}
//void unSetStopWatch(){}

int setAlarm(String hh,String mm,String isAM,String msg,String repeat){return 0;}
void unSetAlarm(String b){}
