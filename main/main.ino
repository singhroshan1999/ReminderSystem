#include <ESP8266WiFi.h>
#include<WiFiUDP.h>
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

//######HEADERS####
void setupWifi();
void setupUDP();
void getNTPTime();
//#################

void setup() {
Serial.begin(115200);
setupWiFi();
setupUDP();

}

void loop() {
  getNTPTime();
  DEBUG2(timeArray[0])
  DEBUG2(":")
  DEBUG2(timeArray[1])
  DEBUG2(":")
  DEBUG(timeArray[2])
  delay(10000);
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
