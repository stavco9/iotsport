int PulseSensorAnalogPin = 0;
int Led1 = 13;

// Data variables
int pulse;

void setup() {
  pinMode(Led1,OUTPUT);
  Serial.begin(115200);
}

void loop() {
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
    
    delay(10);
}
