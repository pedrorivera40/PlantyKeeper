/*
 * PlantyKeeper - A smart garden monitoring system 
 * for Android devices.
 * 
 * This program allows to periodically stream the status 
 * of up to four sensors to a Firebase Realtime database.
 * 
 * This project was developed as an application of some
 * concepts discussed in the Digital Electronics laboratory.
 * 
 * SPRING19
 * INEL4225-060
 * Instructor - Carlos Condori
 * 
 * @author Pedro Luis Rivera (pedrorivera40)
 * @author Lianne Sanchez (liannesr)
 */

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <MCP3008.h>

// SPI Pins for MCP3008 IC.
#define MISO 12
#define MOSI 13
#define CLK 14
#define CS 15

// MPC3008 Analog Read Pins.
#define LIGHT 0
#define MOIST 1
#define RAIN 2
#define TEMP 3


// Firebase Paths.
#define USER "lianne"
#define PLANT_NAME "/plant_one"
#define LIGHT_PATH "/light"
#define MOIST_PATH "/moist"
#define RAIN_PATH "/rain"
#define TEMP_PATH "/temp"

// Firebase & Wifi Setup.
#define FIREBASE_HOST "plantykeeper.firebaseio.com"
#define FIREBASE_AUTH ""
#define WIFI_SSID "RUMNET"
#define WIFI_PASSWORD ""
#define UPDATE_DELAY 60000 // Update Firebase each min.

int light, moist, rain, temp;
String light_path, moist_path, rain_path, temp_path;

MCP3008 adc(CLK, MOSI, MISO, CS);
 
void setup() 
{
  // Begin Serial Communication.
  Serial.begin(9600); 
  // Connect to WIFI.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting: ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
  // Initialize Firebase.
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  light_path = USER;
  light_path.concat(PLANT_NAME);
  light_path.concat(LIGHT_PATH);
  moist_path = USER;
  moist_path.concat(PLANT_NAME);
  moist_path.concat(MOIST_PATH);
  rain_path = USER;
  rain_path.concat(PLANT_NAME);
  rain_path.concat(RAIN_PATH);
  temp_path = USER;
  temp_path.concat(PLANT_NAME);
  temp_path.concat(TEMP_PATH);
}
 
void loop() 
{
 // Read values from ADC.
 light = 100 - (int)((((float)adc.readADC(LIGHT)) / (float)1023) * 100);
 moist = 100 - (int)((((float)adc.readADC(MOIST)) / (float)1023) * 100);
 rain = 100 - (int)((((float)adc.readADC(RAIN)) / (float)1023) * 100);
 temp = 100 - (int)((((float)adc.readADC(TEMP)) / (float)1023) * 100);;

 // Upload data to Firebase.
 Firebase.setInt(light_path, light);
 Firebase.setInt(moist_path, moist);
 Firebase.setInt(rain_path, rain);
 Firebase.setInt(temp_path, temp);
 delay(UPDATE_DELAY); 
}
