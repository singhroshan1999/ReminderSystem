#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include<WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#define DEBUG(x) //Serial.println(x);
#define DEBUG2(x) //Serial.print(x);

String ssid = "Redmi1";
String pass = "12345678";

unsigned int localPort = 2390;
IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[ NTP_PACKET_SIZE];
int timeArray[] = {0,0,0}; // hour,minuit,second

WiFiUDP udp;

unsigned long MILLIS,MILLIS2;

//long alarms[10];
//int alarmTaken = 0;
//long alarmsR[10];
//int alarmTakenR = 0;
long alarm = 0;

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

int setAlarm(String hh,String mm,String isAM,String msg,String repeat); // hhmm --> sec --> sort
void unSetAlarm(String);
void selectionSort(long[],int);
void alarmPoll();
void printArray(long arr[], int size) ;

//#################

void setup() {
Serial.begin(115200);
Serial.setTimeout(40);
pinMode(LED_BUILTIN,OUTPUT);
setupWiFi();
setupUDP();
setupWebServer();
SPIFFS.begin();
MILLIS = millis();
MILLIS2 = millis();
getNTPTime();

}

void loop() {
//  getNTPTime();
//  DEBUG2(timeArray[0])
//  DEBUG2(":")
//  DEBUG2(timeArray[1])
//  DEBUG2(":")
//  DEBUG(timeArray[2])
  if(millis() - MILLIS > 50 && Serial.available()){
    serialEvent();
    MILLIS = millis();
  }
  if(millis() - MILLIS2 > 5000){
//    alarmPoll();
    getNTPTime();
////    for(int i = 0;i<alarmTakenR;i++) Serial.println(alarmsR[i]);
//Serial.println(alarmsR[0]);
////    Serial.println(alarmTakenR);
////    Serial.println(sizeof(alarmsR)/sizeof(unsigned long));
//    Serial.println("---");
    Serial.println(timeArray[0]);
    Serial.println(timeArray[1]);
    Serial.println(timeArray[2]);
    MILLIS2 = millis();
    
    
  }
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
    getStopwatch();
//        server.send(200,"text/plain","GSW");

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
      isPausedStopwatch();
//        server.send(200,"text/plain","IPSW");

  }
  if (server.arg("setAlarm") == "1"){
    int count = setAlarm(server.arg("hh"),server.arg("mm"),server.arg("AM"),server.arg("msg"),server.arg("repeat"));
//    Serial.println(server.arg("hh")+server.arg("mm")+server.arg("AM")+server.arg("msg")+server.arg("repeat"));
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

void toggleStopWatch(){
  Serial.print("A");
  Serial.flush();
  }

long getStopwatch(){
  Serial.print("B");
  Serial.flush();
  return 0L;
  }

long getStopwatchCount(){}  // redudant

void togglePauseStopwatch(){
  Serial.print("C");
  Serial.flush();
  }

bool isPausedStopwatch(){
  Serial.print("D");
  Serial.flush();
  return true;
  }
//
//int setAlarm(String hh,String mm,String isAM,String msg,String repeat){
//  int hr = hh.toInt();
//  int mn = mm.toInt();
//  int totalMn = hr*60+mn;
//  Serial.println("setAlarm0");
//  if(repeat == "true"){
//    Serial.println("setAlarm1");
//    if(alarmTakenR >10) return 1;
//    Serial.println("setAlarm11");
//    alarmsR[alarmTakenR] = totalMn*60;
//    alarmTakenR++;
//    selectionSort(alarmsR,alarmTakenR);
//    printArray(alarmsR,alarmTakenR);
//  }else if(totalMn > timeArray[0]*60+timeArray[1]){
//    if(repeat == "true"){
//    if(alarmTakenR >10) return 1;
//    Serial.println("setAlarm2");
//    alarmsR[alarmTakenR] = totalMn*60;
//    alarmTakenR++;      
//    selectionSort(alarmsR,alarmTakenR);
//    printArray(alarmsR,alarmTakenR);
//    }else {
//    if(alarmTaken >10) return 1;
//    Serial.println("setAlarm3");
//    alarms[alarmTaken] = totalMn*60;
//    alarmTaken++;
//    selectionSort(alarms,alarmTaken);
//    printArray(alarms,alarmTaken);
//    }
//  }
//  return 0;}
void unSetAlarm(String b){}

void serialEvent(){
  if(Serial.available()>0){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  String st = Serial.readString();
  char s = st[0];
  st.remove(0,1);
  if(s == 'X'){
//    Serial.println("dfgdfgdfgdfgdfgdfg");
    server.send(200,"text/plain",st);
  } else if(s == 'Z'){
    server.send(200,"text/plain",st);
  }
  }
}

// selectionSorting

void swap(long *xp, long *yp) 
{ 
    long temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
void selectionSort(long arr[], int n) 
{ 
    int i, j, min_idx; 
  
    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n-1; i++) 
    { 
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i+1; j < n; j++) 
          if (arr[j] < arr[min_idx]) 
            min_idx = j; 
  
        // Swap the found minimum element with the first element 
        swap(&arr[min_idx], &arr[i]); 
    } 
} 
//
//void alarmPoll(){
//  if(alarmTaken == 0 && alarmTakenR == 0){
//    return;
//  }else{
//    if(alarmTaken == 0 && alarmsR[alarmTakenR-1] - (timeArray[0]*60+timeArray[1])*60 > 10){
//      Serial.print(String("E")+String(alarmsR[alarmTakenR-1] - (timeArray[0]*60+timeArray[1])*60)+String("~`")+String("NoMsg"));
//      alarmsR[alarmTakenR] = 99999999;
//      selectionSort(alarmsR,alarmTakenR);
//      printArray(alarmsR,alarmTakenR);
//      alarmTakenR--;
//      return;
//        }
//    if(alarmTakenR == 0 && alarms[alarmTaken-1] - (timeArray[0]*60+timeArray[1])*60 > 10){
//      Serial.print(String("E")+String(alarms[alarmTaken-1] - (timeArray[0]*60+timeArray[1])*60)+String("~`")+String("NoMsg"));
//      alarms[alarmTaken] = 99999999;
//      selectionSort(alarms,alarmTaken);
//      printArray(alarms,alarmTaken);      
//      alarmTaken--;
//      return;
//        }     
//  }
//}

void printArray(long arr[], int sizee) 
{ 
    int i; 
    for (i=0; i < sizee; i++) 
        Serial.print(arr[i]); 
    Serial.println("\n"); 
} 

int setAlarm(String hh,String mm,String isAM,String msg,String repeat){
  int hr = hh.toInt();
  int mn = mm.toInt();
  long totalMn = hr*60L+mn;
  alarm = totalMn;
  Serial.println(alarm);
  Serial.println(timeArray[0]*60+timeArray[1]);
 return 0;
}
