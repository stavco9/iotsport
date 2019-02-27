#include "UbidotsMicroESP8266.h"

// Ubidots
#define TOKEN  "BBFF-EmcoWeUjJABH3U09MPNmVCF69EESB4"
Ubidots client(TOKEN);

// Wifi credentials
#define WIFISSID "Lior"
#define PASSWORD "lior12341234"

// Global parameters and objects
#define MAX_OBJECTS 10
const char* delim = ";";
char objectKey[MAX_OBJECTS][20];
int objectValue[MAX_OBJECTS];
char objectContext[MAX_OBJECTS][40];

void setup() {
  // Initialize wifi component and connect to wifi endpoint
  Serial.begin(9600);
  delay(10);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  // When receiving data from Arduino board,
  // split the data into data structurs that fit to Ubidots client
  // and send it to Ubidots
  
  if (Serial.available()) {
    String dataString = Serial.readString();
    char dataArr[dataString.length()];
    dataString.toCharArray(dataArr, dataString.length());
    char* item = strtok(dataArr, delim);

    int objectIndex = 0;
    while(item != 0 && objectIndex < MAX_OBJECTS) {
      strcpy(objectKey[objectIndex],item);
      item = strtok(0, delim);
      objectValue[objectIndex] = atoi(item);
      item = strtok(0, delim);
      sprintf(objectContext[objectIndex],item);
      item = strtok(0, delim);

      client.add(objectKey[objectIndex], objectValue[objectIndex], objectContext[objectIndex]);
      objectIndex++;
    }
    client.sendAll(true);
  }
}
