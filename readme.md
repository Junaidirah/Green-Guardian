# IoT Temperature, Humidity, and Fire Detection System

This project is an Internet of Things (IoT) system designed to monitor temperature, humidity, and fire detection using an ESP8266 microcontroller, DHT11 sensor, flame sensor, and Firebase Realtime Database. It periodically measures temperature and humidity, sends the data to the Firebase database, and detects fire based on analog sensor readings.

## Features

- Measures temperature and humidity using DHT11 sensor.
- Detects fire using a flame sensor.
- Sends temperature and humidity data to Firebase Realtime Database.
- Sends fire detection status to Firebase Realtime Database.
- Wi-Fi connection status indication through serial monitor.
- Error handling for Firebase operations.

## Hardware Requirements

- ESP8266 microcontroller (e.g., NodeMCU)
- DHT11 temperature and humidity sensor
- Flame sensor
- Buzzer (optional, for fire detection alert)
- Jumper wires

## Dependencies

- [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
- [Firebase_ESP_Client](https://github.com/mobizt/Firebase-ESP8266)
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)

## Setup

1. Connect the hardware components as per the pin definitions in the code.
2. Install the necessary libraries mentioned in the dependencies section.
3. Replace placeholders with your Wi-Fi credentials, Firebase API key, and Realtime Database URL.
4. Upload the code to your ESP8266 board.
5. Open the serial monitor to view the system status and debug messages.

## Usage

- Upon successful connection to Wi-Fi, the system will sign up with Firebase and establish a connection.
- Temperature and humidity data will be periodically read from the DHT11 sensor and sent to the Firebase Realtime Database.
- Fire detection status will be determined based on the analog reading from the flame sensor. If a fire is detected, the status will be sent to the database along with an optional alert through the buzzer.
- You can monitor the system's operation and view the data in real-time through the Firebase console.

## Notes

- Ensure the correct pin connections and proper power supply to the components.
- Make sure the Firebase Realtime Database security rules are appropriately configured to allow read and write access.
