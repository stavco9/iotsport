#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(4, 5);

const int yellow = 12;

void setup() {
  pinMode(yellow, OUTPUT);
  // Same code as in ubidots client wifiConnection function,
  // but here with led indication

  Serial.begin(9600);
  wifiSerial.begin(9600);

  Serial.println("started");
  char wifiStatus = '.';
  while (wifiStatus == '.') {
    if (wifiSerial.available()) {
      wifiStatus = wifiSerial.read();
      Serial.println(wifiStatus);
    }
    delay(500);
  }
  Serial.println("connected");
  Serial.println(wifiSerial.readString());
  /*bool isYellowLedOn = false;
  while (WiFi.status() != WL_CONNECTED) {
    if(isYellowLedOn) {
      digitalWrite(yellow, LOW);
    }
    else {
      digitalWrite(yellow, HIGH);
    }
    delay(500);
  }
  digitalWrite(yellow, LOW);
  WiFi.setAutoReconnect(true);*/
}

void loop() {
  // put your main code here, to run repeatedly:

}
