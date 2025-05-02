// ------- INCLUDES ------- //
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"
#include "WiFi.h"
#include "PubSubClient.h"

// ------- GLOBAL VARS ------- //


/* Adjust these variables appropriately for your own network and MQTT Server
  Notable to mention that if the MQTT Server and Collector is not on the same network.
  Some form of portforwarding is required. Also possible to use some form of DNS */
const char* ssid = "YOUR INTERNET SSID HERE";  
const char* password ="YOUR NETWORK PASSWORD HERE"; 

const char* mqtt_server = "YOUR MOSQUITTO SERVER IP HERE"

// !!!IMPORTANT!!!
// Adjust accordingly for individual ESP
const char* collectorID = "01";                 // Collector ID for multiple ESP32's

Adafruit_AM2320 am2320 = Adafruit_AM2320();     // Stores AM2320 as am2320 for ease of usage.

// variables that store MQTT paths
char humDest[28];
char tempDest[28];
char nomTempVal[32];
char upperNomHumVal[32];
char lowerNomHumVal[32];
char statusStr[28];
char buzzStr[42];
char clientStr[42];

// All pins
const int redLEDPin = 16;                       // Red LED Pin
const int blueLEDPin = 4;                       // Blue LED Pin
const int greenLEDPin = 2;                      // Green LED Pin
const int FanPin = 23;                          // Fan Pin
const int buzzPin = 26;                         // Buzzer Pin

// MQTT relevant variables
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


// Global MQTT Signal relevant variables
float temperature = 0;
float humidity = 0;

float lowerNominalHum = NULL;
float upperNominalHum = NULL;
float NomTemperature = NULL;

int StatusSignal = 0;
int BuzzStatus = 0;


// Time related variables
static unsigned long previousMillis = 0;
unsigned long currentMillis = 0;


// ------- INITS -------- //

//Initialize Temp & Humidity Sensor
void initAm2320() {
  while (!Serial) {                             // Wait for the serial.
    delay(10); 
  }


  Serial.println("Adafruit AM2320 Start");      //Signifies a connection has been found
  am2320.begin();                               //Start the sensor

}

//Initialize WiFi Client 
void initWiFi() {
  if (WiFi.status() != WL_CONNECTED){           //Ensures connection only runs if it is not connected
    WiFi.mode(WIFI_STA);                        // Set ESP to Station Mode. Mostly useful if it has been in AP mode earlier
    WiFi.begin(ssid, password);                 // Connect to the wifi
    Serial.print("Connecting to Wifi...");      
    while (WiFi.status() != WL_CONNECTED){      // If it does not manage to connect. Prints '.' in output
      Serial.print('.');
      delay(1000);
    }
    Serial.println(WiFi.localIP());             // Post local IP
    Serial.println("Wifi Setup Done");
  }

}


// Start the MQTT Client
void initMqttClient(){
  client.setServer(mqtt_server, 1883);         // Set the server to the provided IP, listening at port 1883 (Mosquitto Default)
  client.setCallback(callback);
}

//Initialize all pins
void pinInit(){
  pinMode(greenLEDPin, OUTPUT);                 
  pinMode(blueLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  digitalWrite(buzzPin, HIGH);
  pinMode(FanPin, OUTPUT);
}

// ------- FUNCTIONS -------- //

/* 
 Ensures we are subscribed to the appropriate topics as well as
 that we have an established MQTT connection between the ESP
 and the MQTT Server.
*/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    //Attempt to connect
    if (client.connect(clientStr)) {
      Serial.println(clientStr);
      Serial.println("Connected");
      
      // Topics Subscribed to
      client.subscribe("esp32/output");
      client.subscribe(nomTempVal);         // Nominal Temperature Value
      client.subscribe(upperNomHumVal);     // Nominal Upper Humidity Value
      client.subscribe(lowerNomHumVal);     // Nominal Lower Humidity Value
      client.subscribe(statusStr);          // Status
      client.subscribe(buzzStr);            // Buzzer
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Callback function

/*
This function is from  https://github.com/knolleary/pubsubclient documentation.
*/

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if (messageTemp == "on"){
      Serial.println("on");
      digitalWrite(greenLEDPin, HIGH);
    } 
    else if(messageTemp == "off") {
      Serial.println("off");
      digitalWrite(greenLEDPin, LOW);
    }
  }

// Managing subscribed to topics
  if (String(topic) == lowerNomHumVal) {  
    lowerNominalHum = messageTemp.toFloat(); 
  }

  if(String(topic) == upperNomHumVal) {
    upperNominalHum = messageTemp.toFloat();
  }

  if (String(topic) == nomTempVal) {
    NomTemperature = messageTemp.toFloat();
  }
  if (String(topic) == statusStr) {
    StatusSignal = messageTemp.toInt();
  }
  if (String(topic) == buzzStr) {
    BuzzStatus = messageTemp.toInt();
  }

}

//Publish function
/*
This function is from  https://github.com/knolleary/pubsubclient documentation.
With the addition of publishing the correct strings of information.
*/
void MQTTFunc(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    temperature = am2320.readTemperature();
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");          //Exists for Debug
    Serial.println(tempString);             //Exists for Debug
    client.publish(tempDest, tempString);
    humidity = am2320.readHumidity();
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");            //Exists for Debug
    Serial.println(humString);             //Exists for Debug
    client.publish(humDest, humString);
    
  }
}

/* Function creates MQTT paths with corresponding
  unit number. Unit number is defined at the top of code */
void MQTTPathDefinition(){
  strcpy(humDest,"esp32/unit_");
  strcat(humDest, collectorID);
  strcat(humDest, "/humidity");

  strcpy(tempDest,"esp32/unit_");
  strcat(tempDest, collectorID);
  strcat(tempDest, "/temperature");

  strcpy(nomTempVal, "esp32/unit_");
  strcat(nomTempVal, collectorID);
  strcat(nomTempVal, "/nominal_temp");

  strcpy(lowerNomHumVal, "esp32");
  strcat(lowerNomHumVal, "/humidityLower");

  strcpy(upperNomHumVal, "esp32");
  strcat(upperNomHumVal, "/humidityUpper");

  strcpy(statusStr, "esp32/unit_");
  strcat(statusStr, collectorID);
  strcat(statusStr, "/status");

  strcpy(buzzStr, "esp32/unit_");
  strcat(buzzStr, collectorID);
  strcat(buzzStr, "/buzzerStatus");

  strcpy(clientStr, "ESP8266Client/unit_");
  strcat(clientStr, collectorID);

}

//Sounds the buzzer if the appropriate MQTT signal is sent.
void HumidityBuzz(){
    switch (BuzzStatus) {   
    case 0:
      digitalWrite(buzzPin, HIGH);
    break;

    case 1:
      digitalWrite(buzzPin, LOW);
      delay(1000);
      digitalWrite(buzzPin, HIGH);
    break;
    
    default:
      digitalWrite(buzzPin, HIGH);
    break;
  }
}

/*
Fan control function. Logic handeled locally on ESP32.
Activating the fan if sensor temperature is above 30% of the Nominal Values
*/
void FanTempControl(){
if (am2320.readTemperature() > (NomTemperature * 1.30) ) {
    digitalWrite(FanPin, HIGH);
  } else if (am2320.readTemperature() <= (NomTemperature * 1.10)) {
    digitalWrite(FanPin, LOW);
  } else {
    digitalWrite(FanPin, LOW);
  }
}

void StatusLED(unsigned long currentMillis){
  switch (StatusSignal) {
    case 0:                                                    // All LED's OFF
      digitalWrite(redLEDPin, LOW);
      digitalWrite(blueLEDPin, LOW);
      digitalWrite(greenLEDPin, LOW);
    break;

    case 1:                                                    // Solid green
      digitalWrite(redLEDPin, LOW);
      digitalWrite(blueLEDPin, LOW);
      digitalWrite(greenLEDPin, HIGH);
    break;

    case 2:                                                    // Blinking yellow 2x per second
      digitalWrite(blueLEDPin, LOW);                           // Reset blue LED
      digitalWrite(greenLEDPin, LOW);                          // Reset green LED
      digitalWrite(redLEDPin, LOW);                            // Reset red LED


      if (currentMillis - previousMillis >= 250) {             //Sets the interval to 250ms
        previousMillis = currentMillis;                        //Sorts out the Differential

        static bool redLEDState = false;
        static bool greenLEDState = false;

        greenLEDState = !greenLEDState;
        redLEDState = !redLEDState;
        digitalWrite(redLEDPin, redLEDState ? HIGH : LOW);      // if LedState is true set HIGH if False set LOW
        digitalWrite(greenLEDPin, greenLEDState ? HIGH : LOW);  // if LedState is true set HIGH if False set LOW
      }
    break;

    case 3:                                                     // Solid yellow
      digitalWrite(redLEDPin, HIGH);
      digitalWrite(blueLEDPin, LOW);
      digitalWrite(greenLEDPin, HIGH);
    break;

    case 4:                                                     // Blinking red 4x per second 
      digitalWrite(blueLEDPin, LOW);
      digitalWrite(greenLEDPin, LOW);
      static bool redLEDState = false;                          // Tracks the internal state of the red LED
      if (currentMillis - previousMillis >= 125) {
          previousMillis = currentMillis;                    
          redLEDState = !redLEDState;                           // Toggles Boolean Value
          digitalWrite(redLEDPin, redLEDState ? HIGH : LOW);    // Sets the RED LED correct depending on the boolean value
      }                                                         
    break;                                                      

    case 5:                                                     // Solid red
      digitalWrite(redLEDPin, HIGH);
      digitalWrite(blueLEDPin, LOW);
      digitalWrite(greenLEDPin, LOW);
    break;

    case 6:                                                     // Solid blue
      digitalWrite(redLEDPin, LOW);
      digitalWrite(blueLEDPin, HIGH);
      digitalWrite(greenLEDPin, LOW);
    break;

    default:                                                    // Default State
      digitalWrite(redLEDPin, LOW);
      digitalWrite(blueLEDPin, LOW);
      digitalWrite(greenLEDPin, LOW);
    break;
  }
}



// ------- RUN ON BOOT -------- //
void setup() {
  Serial.begin(115200);                                          // Initializes Serial Monitoring 
  pinInit();                                                     // Initializes Pins
  initAm2320();                                                  // Initializes AM2320 Sensor
  initWiFi();                                                    // Initializes WiFi
  initMqttClient();                                              // Initializes the MQTT Client
  Serial.print("RSSI: ");                                        // Prints WiFi RSSI
  Serial.println(WiFi.RSSI());
  MQTTPathDefinition();                                          // Loads PATHS for MQTT
}



// ------- RUN WHILE ON ------- //
void loop() {
  currentMillis = millis();                                      // Begins count of current Milli's
  switch (StatusSignal) {
    case 6:                                                      // Activates in case of Emergency Stop Button
      digitalWrite(greenLEDPin, LOW);
      digitalWrite(redLEDPin, LOW);
      digitalWrite(blueLEDPin, HIGH);
      client.loop();                                             // Ensures the client is still able to recieve MQTT Signals
    break;

    default:                                                     // Default Behaviour
      MQTTFunc();                                                // Runs MQTT Function
      StatusLED(currentMillis);                                  // Checks StatusLED, sending the current Millis
      HumidityBuzz();                                            // Manages the internal buzzer
      FanTempControl();                                          // Ensures Fan Control
    break;
  }
}

