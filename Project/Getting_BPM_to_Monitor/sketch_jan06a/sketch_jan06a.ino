/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
namespace {
  const char * WIFISSID = "AndroidAP"; // Assign your WiFi SSID
  const char * PASSWORD = "srcr1928"; // Assign your WiFi password
  const char * TOKEN = "BBFF-bed0d8cec66fa492bc2091e9ecf37d13834"; // Assign your Ubidots TOKEN
}

Ubidots client(TOKEN);

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  client.readData(); // Reads the command from the logger
  delay(1000);
}
