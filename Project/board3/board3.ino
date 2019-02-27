// used by PulseSensor library
#define USE_ARDUINO_INTERRUPTS false

// Libraries
#include <SoftwareSerial.h>
#include <dht.h>
#include <TinyGPS++.h>
#include <PulseSensorPlayground.h>

// Serials and Objects
SoftwareSerial wifiSerial(4, 3);
SoftwareSerial gpsSerial(6, 5);
dht dhtData;
TinyGPSPlus gpsData;
String gpsValue = "";
PulseSensorPlayground pulsesensorData;

// Input and output phisical pins
const int photocellPin = A1;
const int dhtPin = 7;
const int pulsesensorPin = A0;
const int greenLed1Pin = 10;
const int greenLed2Pin = 11;
const int yellowLedPin = 12;
const int redLedPin = 13;

// Other global variables
int loopCounter;
int measurementCounter;
const char delim = ';';

void setup() {
  // Initialize input ports
  pinMode(photocellPin, INPUT);
  pinMode(dhtPin, INPUT);
  pinMode(greenLed1Pin, OUTPUT);
  pinMode(greenLed2Pin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  // Initialize serial ports
  Serial.begin(9600);
  wifiSerial.begin(9600);
  gpsSerial.begin(9600);

  // Initialize pulsesensor
  pulsesensorData.analogInput(pulsesensorPin);
  pulsesensorData.setThreshold(700);
  pulsesensorData.begin();

  // Zero counters
  loopCounter = 0;
  measurementCounter;

  // Leds signing: Get ready
  digitalWrite(redLedPin, HIGH);
  delay(1000);
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, HIGH);
  delay(1000);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLed2Pin, HIGH);
  delay(1000);
  digitalWrite(greenLed2Pin, LOW);
}

void loop() {
  // Get data from pulse sensor
  pulsesensorData.sawNewSample();
  
  // Get data from gps
  while (gpsSerial.available()) {
    gpsData.encode(gpsSerial.read());
    if (gpsData.location.isUpdated()) {
      gpsValue = "lat=" + String(gpsData.location.lat()) + "$lng=" + String(gpsData.location.lng());
    }
  }
    
  // Send data to wifi once in a two seconds
  if(loopCounter == 2000) {
    // Get data from temperature and humidity sensor
    dhtData.read11(dhtPin);
    

    // Fulfill data variables
    int photocellValue = analogRead(photocellPin);
    int temperatureValue = (int)dhtData.temperature;
    int humidityValue = (int)dhtData.humidity;
    int pulseValue = pulsesensorData.getBeatsPerMinute();

    // Fitness score calculations
    int hotness = temperatureValue;
    hotness = getHotnessByResistor(hotness, photocellValue);
    hotness = getHotnessByHumidity(hotness, humidityValue);
    int fitnessValue = calculateFinalFitness(pulseValue, getHotnessFactor(hotness));

    // Join data from all sources before sending it
    String Summary =
      pair("Light", photocellValue) +
      pair("Temperature", temperatureValue) +
      pair("Humidity", humidityValue) +
      pair("Heartbeat", pulseValue) +
      pair("Fitness", fitnessValue);

    if(gpsValue != "") {
      Summary += pair("Location", gpsValue);
    }

    // Print data for debugging + send data to wifi component
    Serial.println(Summary);
    wifiSerial.print(Summary);

    // Zero loop counter
    loopCounter = 0;
    measurementCounter++;
  }
  
  // Get feedback from wifi (works if debug flag is on)
  while (wifiSerial.available()) {
    String fromWifi = wifiSerial.readString();
    Serial.println(fromWifi);
  }

  // Led signing: Measuring right now
  if((loopCounter+200)%400==0) {
  digitalWrite(greenLed1Pin, HIGH);
  digitalWrite(greenLed2Pin, LOW);
  }
  if(loopCounter%400==0) {
  digitalWrite(greenLed1Pin, LOW);
  digitalWrite(greenLed2Pin, HIGH);
  }

  // Leds signing: Measurment ended
  if(measurementCounter==5) {
    for(int i=0;i<3;i++) {
    digitalWrite(greenLed1Pin, HIGH);
    digitalWrite(greenLed2Pin, HIGH);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    delay(100);
    digitalWrite(greenLed1Pin, LOW);
    digitalWrite(greenLed2Pin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    delay(100);
    }
    delay(5000);
    measurementCounter=0;
  }

  loopCounter++;
  delay(1);
}

// Function for creating a string, readable by wifi component
String pair(String key, int value) {
  return (key + delim + String(value) + delim + '_' + delim);
}
// When value is a String, it will be used as "context" for Ubidots and not as "value"
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
