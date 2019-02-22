#include "UbidotsMicroESP8266.h"

// The ubidots token for Barear authorization header
#define TOKEN  "BBFF-EmcoWeUjJABH3U09MPNmVCF69EESB4"

// The wifi username and password to connent
#define WIFISSID "Lior"
#define PASSWORD "lior12341234"

// Initalize the ubidots client with token
Ubidots client(TOKEN);

// Array of titles
char* titles[] = {"BPM", "Light", "Temperature", "Humidity", "Fitness Level"};

// The delmiter of the string of parameters is single quote
const char* delim = ",";

void setup() {
  Serial.begin(9600);
  
  delay(10);

  // Connect to wifi network using the username and the password
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {

  // If the serial is available
  if (Serial.available()) {

    // Read the string from the serial
    String inputStr = Serial.readString();

    // Initalize an input char array with the length of the string
    char inputArr[inputStr.length()+1];

    // Cast the string to the char array
    inputStr.toCharArray(inputArr, inputStr.length()+1);

    // Split the char array by the delmiter 
    char* splitInput = strtok(inputArr, delim);

    int index = 0;

    // While there is still parameters to send 
    while(splitInput != 0) {

      // Add the current parameter from string to the Ubidots client HTTP request and cast it into integer
      client.add(titles[index], atoi(splitInput));

      // Print the value into serial
      Serial.println(splitInput);

      // Get the next parameter
      splitInput = strtok(0, delim);
      index++;
    }

    // Send all the data to Ubidots cloud service
    client.sendAll(true);
  }
}
