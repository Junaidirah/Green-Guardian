#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
// Declaration
#define DHT_SENSOR_PIN 4
#define DHT_SENSOR_TYPE DHT11

int  Buzzer = D2;
int Fire_analog = A0;
int Fire_digital =D1;

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// LCD Configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Insert your network credentials
#define WIFI_SSID "awik"
#define WIFI_PASSWORD "banyak12345"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAmrPo2-DLauHYLQl7qBrjxb5w8bfaCvsE"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://green-guardian-14c4a-default-rtdb.asia-southeast1.firebasedatabase.app/" 

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  dht_sensor.begin();
  pinMode(Buzzer, OUTPUT);      
  pinMode(Fire_digital, INPUT);
  Serial.begin(115200);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  // Sign up
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Sign-up successful");
    signupOK = true;
  } else {
    Serial.println("Sign-up failed");
    Serial.println(config.signer.signupError.message.c_str());
  }

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Temperature and humidity measured should be stored in variables so the user
  // can use it later in the database
  float temperature = dht_sensor.readTemperature();
  float humidity = dht_sensor.readHumidity();
  int firesensorAnalog = analogRead(Fire_analog);
  int firesensorDigital = digitalRead(Fire_digital);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");



  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    // Since we want the data to be updated every second
    sendDataPrevMillis = millis();

    // Enter Temperature into the DHT_11 Table
    if (Firebase.RTDB.setInt(&fbdo, "DHT_11/Temperature", temperature)) {
      Serial.print("Temperature : ");
      Serial.println(temperature);
    } else {
      Serial.println("Failed to read from the sensor");
      Serial.println("Reason: " + fbdo.errorReason());
    }

    // Enter Humidity into the DHT_11 Table
    if (Firebase.RTDB.setFloat(&fbdo, "DHT_11/Humidity", humidity)) {
      Serial.print("Humidity : ");
      Serial.println(humidity);
    } else {
      Serial.println("Failed to read from the sensor");
      Serial.println("Reason: " + fbdo.errorReason());
    }
  }
  Serial.print("Fire Sensor: ");
  Serial.print(firesensorAnalog);
  Serial.print("\t");
  Serial.print("Fire Class: ");
  Serial.print(firesensorDigital);
  Serial.print("\t");
  Serial.print("\t");
  if (firesensorAnalog < 1000) {
    Serial.println("Fire detected");
    digitalWrite(Buzzer, HIGH); // Send tone
    delay(1000);
    digitalWrite(Buzzer, LOW);  // No tone

    // Send fire status to database
    if (Firebase.ready() && signupOK) {
      if (Firebase.RTDB.setString(&fbdo, "FireDetection/Status", "Fire")) {
        Serial.println("Status 'Fire' sent to database");
      } else {
        Serial.println("Failed to send 'Fire' status to database");
        Serial.println("Reason: " + fbdo.errorReason());
      }
    }
  } else {
    Serial.println("No fire detected");

    // Send no fire status to database
    if (Firebase.ready() && signupOK) {
      if (Firebase.RTDB.setString(&fbdo, "FireDetection/Status", "No Fire")) {
        Serial.println("Status 'No Fire' sent to database");
      } else {
        Serial.println("Failed to send 'No Fire' status to database");
        Serial.println("Reason: " + fbdo.errorReason());
      }
    }
  }
  delay(100);
}