#include "UbidotsMicroESP8266.h"

#define TOKEN  "BBFF-EmcoWeUjJABH3U09MPNmVCF69EESB4"
#define WIFISSID "Lior"
#define PASSWORD "lior12341234"

Ubidots client(TOKEN);

// Data variable
float data = 0;

void setup(){
    Serial.begin(115200);
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    // Read data from Serial and add it to Ubidots client
    data = Serial.read();
    client.add("Pulse", data);

    // Send all data to Ubidots client
    client.sendAll(true);
}

