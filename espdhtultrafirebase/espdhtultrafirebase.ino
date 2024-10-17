#include <Adafruit_Sensor.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <DHT.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Students"
#define WIFI_PASSWORD "JT@wbm61s"

// Insert Firebase project API Key
#define API_KEY "AIzaSyC1gujNfFh6QtjYIwKVf9YbC9OzOS0BzHk"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "cck01092005@gmail.com"
#define USER_PASSWORD "Kanha#2005"

// Insert RTDB URL
#define DATABASE_URL "https://esp-32-iot-f1178-default-rtdb.firebaseio.com"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String tempPath;
String humPath;
String distancePath;

// DHT11 sensor configuration
#define DHTPIN 4  // Define the GPIO pin connected to the DHT11 data pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temperature;
float humidity;

// Ultrasonic sensor configuration
#define TRIG_PIN 26  // GPIO pin for the trigger pin of the ultrasonic sensor
#define ECHO_PIN 25  // GPIO pin for the echo pin of the ultrasonic sensor
long duration;
float distance;

// Timer variables (send new readings every 10 seconds)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 10000; // 10 seconds

// Initialize DHT11
void initDHT() {
  dht.begin();
  Serial.println("DHT11 sensor initialized.");
}

// Initialize Ultrasonic Sensor
void initUltrasonic() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("Ultrasonic sensor initialized.");
}

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());
}

// Write float values to the database
void sendFloat(String path, float value) {
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)) {
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print(" to path: ");
    Serial.println(path);
    Serial.println("Write successful!");
  } else {
    Serial.print("Write failed: ");
    Serial.println(fbdo.errorReason());
  }
}

// Get distance from the ultrasonic sensor
float getDistance() {
  // Clear the trigPin by setting it LOW
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculating the distance in centimeters
  distance = (duration * 0.034) / 2;

  return distance;
}

void setup() {
  Serial.begin(115200);
  
  // Initialize sensors and WiFi
  initDHT();
  initUltrasonic();
  initWiFi();

  // Assign the API key (required)
  config.api_key = API_KEY;

  // Assign the user sign-in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long-running token generation task
  config.token_status_callback = tokenStatusCallback; // See addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase auth and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database paths for sensor readings
  tempPath = databasePath + "/temperature"; // --> UsersData/<user_uid>/temperature
  humPath = databasePath + "/humidity";     // --> UsersData/<user_uid>/humidity
  distancePath = databasePath + "/distance"; // --> UsersData/<user_uid>/distance
}

void loop() {
  // Send new readings to database every 10 seconds
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Get latest sensor readings
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    distance = getDistance();

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Display readings on Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Send readings to database
    sendFloat(tempPath, temperature);
    sendFloat(humPath, humidity);
    sendFloat(distancePath, distance);
  }
}

