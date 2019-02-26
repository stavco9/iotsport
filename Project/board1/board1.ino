#include <SoftwareSerial.h>
#include <dht.h>

SoftwareSerial wifiSerial(4, 5);
dht dhtData;

const int photocellPin = A1;
const int dhtPin = 7;

long photocellDataSum;
long temperatureDataSum;
long humidityDataSum;

int loopCounter;


const char* delim = ',';


void setup() {
  pinMode(photocellPin, INPUT);
  pinMode(dhtPin, INPUT);
  
  Serial.begin(9600);
  wifiSerial.begin(9600);

  photocellDataSum = 0;
  temperatureDataSum = 0;
  humidityDataSum = 0;
  loopCounter = 0;
}

void loop() {
  /*Serial.println("light: ");
  Serial.println(analogRead(photocellPin));
  if(dhtData.read11(dhtPin) == DHTLIB_OK) {
    Serial.println("temperature: ");
    Serial.println(dhtData.temperature);
    Serial.println("humidity: ");
    Serial.println((int)dhtData.humidity);
  }*/
/*
  if(loopCounter == 500) {
    
    dhtData.read11(dhtPin);
  
    String Summary =
      String(analogRead(photocellPin), DEC) + ',' +
      String((int)dhtData.temperature, DEC) + ',' +
      String((int)dhtData.humidity, DEC);

    Serial.println(Summary);
    wifiSerial.println(Summary);
    
  }*/
  
  /*photocellDataSum += analogRead(photocellPin);
  if(dhtData.read11(dhtPin) == DHTLIB_OK) {
    temperatureDataSum += (long)dhtData.temperature;
    humidityDataSum += (long)dhtData.humidity;
  }*/
  
  //Serial.println(String(dhtData.temperature)+"   -   "+String(temperatureDataSum));
  
  /*if(loopCounter == 150) {
    String Summary =
      String(photocellDataSum / loopCounter, DEC) + ',' +
      String(temperatureDataSum / loopCounter, DEC) + ',' +
      String(humidityDataSum / loopCounter, DEC);

    //wifiSerial.print(realdata);
    Serial.println(Summary);
    
    photocellDataSum = 0;
    temperatureDataSum = 0;
    humidityDataSum = 0;
    loopCounter=0;
  }*/

/*
    char inputArr[Summary.length()+1];
    Summary.toCharArray(inputArr, Summary.length()+1);
    char* splitInput = strtok(inputArr, delim);

    int index = 0;
    while(splitInput != 0) {

      Serial.println(splitInput);
      
      splitInput = strtok(0, delim);
      index++;
    }*/

  // Feedback from wifi
  while (wifiSerial.available()) {
    String fromWifi = wifiSerial.readString();
    Serial.println(fromWifi);
  }

  loopCounter++;
  delay(10);
}
