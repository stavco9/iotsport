#include <SoftwareSerial.h>
#include <dht.h>

SoftwareSerial wifiSerial(4, 5);
dht dhtData;

const int photocellPin = A1;
const int dhtPin = 7;

int loopCounter;
const char delim = ',';
const char* delim1 = ",";
char k[20];
int v;

void setup() {
  // Initialize input ports
  pinMode(photocellPin, INPUT);
  pinMode(dhtPin, INPUT);

  // Initialize serial ports
  Serial.begin(9600);
  wifiSerial.begin(9600);

  // Initialize data structure (will be transferred to ESP8266)
  loopCounter = 0;
}

void loop() {
  dhtData.read11(dhtPin);

  /*data["Light"] = analogRead(photocellPin);
  data["Temperature"] = (int)dhtData.temperature;
  data["Humidity"] = (int)dhtData.humidity;

  dataString = "";
  for(map<char*,int>::iterator it = data.begin(); it != data.end(); ++it) {
    dataString += it->first + elementDelim + data[it->first] + arrayDelim;
  }

  
  Serial.println(dataString);*/
  
    String Summary =
      createItem("Light", analogRead(photocellPin)) +
      createItem("Temperature", (int)dhtData.temperature) +
      createItem("Humidity", (int)dhtData.humidity);
      
      
  
    /*String Summary =
      "Light," + String(analogRead(photocellPin), DEC) +
      ",Temperature," + String((int)dhtData.temperature, DEC) +
      ",Humidity," + String((int)dhtData.humidity, DEC);*/

    Serial.println(Summary);

    String dataString = Summary;

    char dataArr[dataString.length()];
    dataString.toCharArray(dataArr, dataString.length());
    char* item = strtok(dataArr, delim1);
    while(item != 0) {
      strcpy(k,item);
      item = strtok(0, delim1);
      v = atoi(item);
      item = strtok(0, delim1);

      Serial.println(k);
      Serial.println(v);
    }

    delay(5000);
}

String createItem(String key, int value) {
  return (key + delim + String(value) + delim);
}
