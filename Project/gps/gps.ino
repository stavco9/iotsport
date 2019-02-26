#include <TinyGPS++.h>
#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(4, 3);
SoftwareSerial gpsSerial(6, 5);
TinyGPSPlus gpsData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //wifiSerial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gpsSerial.available()) {
    Serial.write(gpsSerial.read());
  }
  
  /*while (gpsSerial.available()) {
    gpsData.encode(gpsSerial.read());
    if (gpsData.location.isUpdated()) {
      Serial.print("Latitude= "); 
      Serial.print(gpsData.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gpsData.location.lng(), 6);
    }
  }*/
}
