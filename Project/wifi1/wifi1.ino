#include "UbidotsMicroESP8266.h"

#define TOKEN  "BBFF-EmcoWeUjJABH3U09MPNmVCF69EESB4"
#define WIFISSID "Lior"
#define PASSWORD "lior12341234"

Ubidots client(TOKEN);

char* titles[] = {"Light", "Temperature", "Humidity"};
const char* delim = ",";

void setup() {
  Serial.begin(9600);
  delay(10);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  if (Serial.available()) {
    String inputStr = Serial.readString();
    
    char inputArr[inputStr.length()+1];
    inputStr.toCharArray(inputArr, inputStr.length()+1);
    char* splitInput = strtok(inputArr, delim);

    int index = 0;
    while(splitInput != 0) {
      client.add(titles[index], atoi(splitInput));

      Serial.println(splitInput);
      
      splitInput = strtok(0, delim);
      index++;
    }

    client.sendAll(true);
  }
}
