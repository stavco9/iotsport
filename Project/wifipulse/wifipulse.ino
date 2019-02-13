#include "UbidotsMicroESP8266.h"
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN
#define WIFISSID "Your_WiFi_SSID"
#define PASSWORD "Your_WiFi_Password"

Ubidots client(TOKEN);

int PulseSensorAnalogPin = 0;
int Led1 = 13;

// Variables
int pulse;

void setup(){
    pinMode(Led1,OUTPUT);
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    // Read data from sensors
    pulse = analogRead(PulseSensorAnalogPin);
    
    // Send data to Serial
    Serial.println(pulse);

    // Turn leds on/off
    if(pulse > 550) {
        digitalWrite(Led1,HIGH);
    } else {
        digitalWrite(Led1,LOW);
    }

    // Send values to Ubidots
    client.add("Pulse", pulse);
    client.sendAll(true);
}
