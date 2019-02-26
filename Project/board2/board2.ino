#include <SoftwareSerial.h>
#include <dht.h>
#include <TinyGPS++.h>

SoftwareSerial wifiSerial(4, 3);
SoftwareSerial gpsSerial(6, 5);
dht dhtData;
TinyGPSPlus gpsData;
String gpsValue = "";

const int photocellPin = A1;
const int dhtPin = 7;
const int ledPin = 13;

int loopCounter;
const char delim = ';';

void setup() {
  // Initialize input ports
  pinMode(photocellPin, INPUT);
  pinMode(dhtPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Initialize serial ports
  Serial.begin(9600);
  wifiSerial.begin(9600);
  gpsSerial.begin(9600);
  
  loopCounter = 0;
}

void loop() {
  while (gpsSerial.available()) {
    gpsData.encode(gpsSerial.read());
    if (gpsData.location.isUpdated()) {
      gpsValue = "lat=" + String(gpsData.location.lat()) + "$lng=" + String(gpsData.location.lng());
    }
  }
    
  // Send data to wifi once in a second
  if(loopCounter == 200) {
    dhtData.read11(dhtPin);

    int photocellValue = analogRead(photocellPin);
    int temperatureValue = (int)dhtData.temperature;
    int humidityValue = (int)dhtData.humidity;
    int pulseValue = 70; // REPLACE THIS WITH REAL VALUE LATER

    
    int hotness = temperatureValue;
    // Update the hotness by photoresistor value
    hotness = getHotnessByResistor(hotness, photocellValue);
    // Update the hotness by humidity value
    hotness = getHotnessByHumidity(hotness, humidityValue);
    // Calculate the final fitness by pulse the hotness factor
    int fitnessValue = calculateFinalFitness(pulseValue, getHotnessFactor(hotness));
    
    String Summary =
      pair("Light", photocellValue) +
      pair("Temperature", temperatureValue) +
      pair("Humidity", humidityValue) +
      pair("Fitness", fitnessValue);

    if(gpsValue != "") {
      Summary += pair("Location", gpsValue);
    }

    Serial.println(Summary);
    wifiSerial.print(Summary);
    loopCounter = 0;

    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
  }
  
  // Feedback from wifi
  while (wifiSerial.available()) {
    String fromWifi = wifiSerial.readString();
    Serial.println(fromWifi);
  }

  loopCounter++;
  delay(10);
}

String pair(String key, int value) {
  return (key + delim + String(value) + delim + '_' + delim);
}
// When value is a String, it will be used as "context" for Ubidots
String pair(String key, String value) {
  return (key + delim + '1' + delim + value + delim);
}



// This function incraeses the temperature in case of high light discovered in photo resistor sensor
int getHotnessByResistor(int hotness, int resistor){
  if (resistor > 500){
      hotness++;
    }
  if(resistor > 800){
      hotness++;
    }
    return hotness;
}

// This function increases the temperature in case of high humidity
// if the humidity is greater than 50, the temperature inceares by one
// if the humidity is greater than 60, the temperature inceares another one and then and then
int getHotnessByHumidity(int hotness, int humidity){
  int counter;
  for(counter = 50; counter < humidity; counter+=10){
      hotness++;
    }
    return hotness;
}

// This function calculate the temperature factor on calculating the final fitness level by pulse.
// While temperature is 18, the value will be 1 and it will grow when the temperature rises or getting down.
float getHotnessFactor(int hotness){
  return (0.00146 * pow(hotness, 2) - 0.0526 * hotness + 1.4738);  
}

// This function calculates the fitness level from 1 to 100 by the pulse BPM value and with the factor parameter that calculated by the temperature
// When the factor is 1 and the pulse is 120, the value is 100. When the pulse is 30 and less or 210 and high, the value is 0 
int calculateFinalFitness(int pulse, float factor){
  float a = (-0.012 * pow(pulse, 2));
  float b = (2.96 * factor * pulse);
  float c = -(-0.012 * pow(((2.96 * factor / 0.024) - (120-30)),2) + 2.96 * factor * ((2.96 * factor / 0.024) - (120-30)));

  return max((int)(a + b + c), 0);
}
