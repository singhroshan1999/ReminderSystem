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
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Serial.println(Firebase.pushInt("/",1234));
  Serial.println(Firebase.pushFloat("/float",1234));
  Serial.println(Firebase.pushBool("/bool",1234));
  Serial.println(Firebase.pushString("/s/string","eqweqweqwe"));

  Firebase.setInt("/roshan",45646);
  Firebase.setFloat("/singh",456.46);
  Firebase.setBool("/it",false);
  Firebase.setString("/fourth","fsdfsd");

  Serial.println(Firebase.getInt("/roshan"));
  Serial.println(Firebase.getFloat("/singh"));
  Serial.println(Firebase.getBool("/it"));
  Serial.println(Firebase.getString("/fourth"));
//  delay(5000);
  Firebase.remove("/s");
  Firebase.stream("/float");
}

void loop() {
  delay(5000);
  FirebaseObject f = Firebase.readEvent();
  Serial.println("-------------------------------");
  Serial.println(f.getString("type"));
  Serial.println(f.getString("data"));
  Serial.println(f.getString("path"));
}
