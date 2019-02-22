#include <SoftwareSerial.h>
#include <dht.h>
#include <PulseSensorPlayground.h>

SoftwareSerial wifiSerial(4, 5); // RX, TX
dht dhtData;
PulseSensorPlayground pulseSensor;

//  Variables
const int pulseSensorPin = A0;   // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int pulseSensorLed = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
const int photocellPin = A1;
const int dhtPin = 7;

long photocellDataSum;
long temperatureDataSum;
long humidityDataSum;

int loopCounter;

const char* delim = ",";

// This function incraeses the temperature in case of high light discovered in photo resistor sensor
int getTemperatureByResistor(int temperature, int resistor){
  if (resistor > 500){
      temperature++;
    }

  if(resistor > 800){
      temperature++;
    }

    return temperature;
}

// This function increases the temperature in case of high humidity
// if the humidity is greater than 50, the temperature inceares by one
// if the humidity is greater than 60, the temperature inceares another one and then and then
int getTemperatureByHumidity(int temperature, int humidity){
  int counter;
  
  for(counter = 50; counter < humidity; counter+=10){
      temperature++;
    }

    return temperature;
}

// This function calculate the temperature factor on calculating the final fitness level by pulse.
// While temperature is 18, the value will be 1 and it will grow when the temperature rises or getting down.
float getTemperatureFactor(int temperature){
  return (0.00146 * pow(temperature, 2) - 0.0526 * temperature + 1.4738);  
}

// This function calculates the fitness level from 1 to 100 by the pulse BPM value and with the factor parameter that calculated by the temperature
// When the factor is 1 and the pulse is 120, the value is 100. When the pulse is 30 and less or 210 and high, the value is 0 
int calculateFinalFitness(int pulse, float factor){
  float a = (-0.012 * pow(pulse, 2));
  float b = (2.96 * factor * pulse);
  float c = -(-0.012 * pow(((2.96 * factor / 0.024) - (120-30)),2) + 2.96 * factor * ((2.96 * factor / 0.024) - (120-30)));

  return max((int)(a + b + c), 0);
}

void setup() {
  // Initalize the photo resistor pin
  pinMode(photocellPin, INPUT);

  // Initalize the dht pin
  pinMode(dhtPin, INPUT);

  // Initalize the analog input of pulse sensor pin
  pulseSensor.analogInput(pulseSensorPin);
  
  // Initalize the led of pulse sensor pin led
  pulseSensor.blinkOnPulse(pulseSensorLed);

  // Initalize the threshold
  pulseSensor.setThreshold(Threshold);
  
  Serial.begin(9600);
  wifiSerial.begin(9600);

  photocellDataSum = 0;
  temperatureDataSum = 0;
  humidityDataSum = 0;
  loopCounter = 0;
}

void loop() {

  // Read the new temperature and humidity value
  dhtData.read11(dhtPin);

  // Read the new BPM
  int pulse = pulseSensor.getBeatsPerMinute();

  // Read the new photo resistor value
  int resistor = analogRead(photocellPin);

  // Read the new temperature
  int temperature = (int)dhtData.temperature;

  // Read the new humidity
  int humidity = (int)dhtData.humidity;

  // Initalize the final fitness variable
  int fitness;

  // Update the temperature by resistor value
  temperature = getTemperatureByResistor(temperature, resistor);

  // Update the temperature by humidity value
  temperature = getTemperatureByHumidity(temperature, humidity);

  // Calculate the final fitness by pulse the temperature factor
  fitness = calculateFinalFitness(pulse, getTemperatureFactor(temperature));

  // Build a string of all the parameters seperated by single quote
  String Summary =
     String(pulse, DEC) + ',' +
     String(resistor, DEC) + ',' +
     String(temperature, DEC) + ',' +
     String(humidity, DEC) + ',' + 
     String(fitness, DEC);

  // Print the data to the computer serial (for monitoring)
  Serial.println(Summary);

  // Print the data to the virtual serial that the ESP8266 will read from
  wifiSerial.println(Summary);

  // Feedback from wifi
  while (wifiSerial.available()) {
    String fromWifi = wifiSerial.readString();
    Serial.println(fromWifi);
  }

  // Delay one second
  delay(1000);
}
