#include "Timer.h"
Timer t;
int stopWatchID = -3;
static long stopWatchCount = 0;
bool isPausedStopwatchFlag = false;

int setMemoID = -3;
String memoMsg = "";



void toggleStopWatch();
long getStopwatch();
long getStopwatchCount();
void togglePauseStopwatch();
bool isPausedStopwatch();
//void unSetStopWatch();
void stopWatchTick();

void setMemo(String,String);
void unSetMemo();

void displayNotification(String);

unsigned long DEB_t;
//#################


void setup() {
 Serial.begin(115200);
 Serial.setTimeout(10);
toggleStopWatch();
delay(1000);
Serial.println(getStopwatch());
DEB_t = millis();
//setMemo("1","hello memo!");
}

void loop() {

if(millis()-DEB_t >1000){
DEB_t = millis();
Serial.print("--");
Serial.println(millis());
Serial.println(getStopwatch());
}
t.update();
//delay(500);
}


void toggleStopWatch(){
  if(stopWatchID >=0){
    t.stop(stopWatchID);
    stopWatchID = -3;
    stopWatchCount = 0;
  } else {
//    Serial.println("TSW");
    stopWatchID = t.every(1000,stopWatchTick,(void*)1);
  }
  }
long getStopwatch(){
  return stopWatchCount;
  }
long getStopwatchCount(){  // TODO: redundant
  return stopWatchCount;
  }
void togglePauseStopwatch(){
    if(stopWatchID >=0){
    t.stop(stopWatchID);
    stopWatchID = -3;
    isPausedStopwatchFlag = true;
//    stopWatchCount = 0;  // just store count
  } else {
//    Serial.println("TPS");
    isPausedStopwatchFlag = false;
    stopWatchID = t.every(1000,stopWatchTick,(void*)1);
  }
  }
bool isPausedStopwatch(){
  return isPausedStopwatchFlag;
  }
//void unSetStopWatch(){}

void setMemo(String minuit,String msg){
  // some message
  int mint = minuit.toInt();
  memoMsg = msg;
//  Serial.println(mint*60*1000L);
  setMemoID = t.after(mint*60*1000L,setMemoDo,(void*)(mint*60L));
//  return 0;
  }
void unSetMemo(){
  t.stop(setMemoID);
  setMemoID = -3;
  memoMsg = "";
  }

void stopWatchTick(){
//  Serial.println(stopWatchCount);
  stopWatchCount++;
}

void setMemoDo(){
  Serial.println(memoMsg);
  setMemoID = -3;
  memoMsg = "";
}

void displayNotification(String msg){
  Serial.println(msg);
}

void serialEvent(){
//  char s = (char)Serial.read();
  String st = Serial.readString();
  char s = st[0];
  st.remove(0,1);
//  Serial.println(st);
  if(s == 'A'){
    toggleStopWatch();
  } else if( s == 'B'){
    Serial.println(getStopwatch());
} else if( s == 'C'){
    togglePauseStopwatch();
} else if( s == 'D'){
    Serial.println(isPausedStopwatch());
} else if( s == 'E'){
    String msg = st.substring(st.indexOf("~`")+2);
    st.remove(st.indexOf("~`"));
//    Serial.println(st+String(" ")+msg);
    setMemo(st,msg);
} else if( s == 'F'){
    unSetMemo();
} else if( s == 'G'){
    displayNotification(st);
}

}
