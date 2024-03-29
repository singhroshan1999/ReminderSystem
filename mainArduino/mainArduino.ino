#include "Timer.h"
Timer t;
int stopWatchID = -3;
static long stopWatchCount = 0;
int isPausedStopwatchFlag = 0;

int setMemoID = -3;
String memoMsg = "";



void toggleStopWatch();
void getStopwatch();
long getStopwatchCount();
void togglePauseStopwatch();
void isPausedStopwatch();
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
//Serial.println(getStopwatch());
DEB_t = millis();
pinMode(LED_BUILTIN,OUTPUT);
//setMemo("1","hello memo!");
}

void loop() {

//if(millis()-DEB_t >1000){
//DEB_t = millis();
//Serial.print("--");
//Serial.println(millis());
//Serial.println(getStopwatch());
//}
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

void getStopwatch(){
  Serial.print(String("Z")+String(stopWatchCount));
  Serial.flush();
//  return stopWatchCount;
  }
long getStopwatchCount(){  // TODO: redundant
  return stopWatchCount;
  }
void togglePauseStopwatch(){
    if(stopWatchID >=0){
    t.stop(stopWatchID);
    stopWatchID = -3;
    isPausedStopwatchFlag = 1;
//    stopWatchCount = 0;  // just store count
  } else {
//    Serial.println("TPS");
    isPausedStopwatchFlag = 0;
    stopWatchID = t.every(1000,stopWatchTick,(void*)1);
  }
  }
void isPausedStopwatch(){
  Serial.print(String("X")+String(isPausedStopwatchFlag));
  Serial.flush();
//  return isPausedStopwatchFlag;
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
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));

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
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
//  char s = (char)Serial.read();
  String st = Serial.readString();
//  Serial.println(st);
  char s = st[0];
  st.remove(0,1);
  
//  Serial.println(st);
  if(s == 'A'){
    toggleStopWatch();
  } else if( s == 'B'){
    getStopwatch(); // mark
} else if( s == 'C'){
    togglePauseStopwatch();
} else if( s == 'D'){
    isPausedStopwatch();  // mark
} else if( s == 'E'){
    String msg = st.substring(st.indexOf("~`")+2);
    st.remove(st.indexOf("~`"));
//    Serial.println(st+String(" ")+msg);
    setMemo(st,msg);
} else if( s == 'F'){
    unSetMemo();
} else if( s == 'G'){
    displayNotification(st);  //mark2
}

}
