#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "iott-46d55.firebaseio.com"
#define FIREBASE_AUTH "Z2UQb80fKTU28ThgZ9Iny0srqH8ikMAg9ReS7y0u"
#define WIFI_SSID "Redmi"
#define WIFI_PASSWORD "12345678"

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
 Firebase.pushString("/msg","hello");
 Serial.println(Firebase.success());
 Firebase.stream("/msg");
}

void loop() {
  delay(500);
  FirebaseObject f = Firebase.readEvent();
  Serial.println("-------------------------------");
  Serial.println(f.getString("type"));
  Serial.println(f.getString("data"));
  Serial.println(f.getString("path"));
}
