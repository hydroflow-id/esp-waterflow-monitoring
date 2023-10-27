/*
  Rui Santos modified by Arif Nur Rizqi
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  github repositories https://github.com/arifnurrizqi/esp-waterflow-monitoring
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define YF_B1_PIN 19 // pin of water flow sensor in pin 19

Adafruit_SSD1306 display(128, 64, &Wire, -1); // OLED display object

// Replace with your network credentials
const char* ssid     = "ARNUR"; //your wifi ssid
const char* password = "takonmama"; //your wifi password

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.0.2/esp-waterflow-monitoring/post-data.php"; //example http://192.168.0.12/esp-waterflow-monitoring/post-data.php

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

volatile unsigned long pulseCount = 0;
volatile float flowRate;
float flowLiterPerMin;
float totalVolume = 0;
unsigned long prevTime = 0;
int scrollPosition = 128;
float prevFlowLiterPerMin = -1; // Variable to store previous flowLiterPerMin value
unsigned long prevSendTime = 0; // Variable to store previous send time
const int sendInterval = 30000; // Interval to send data in milliseconds (30 seconds)
unsigned long prevScrollTime = 0;
const int scrollInterval = 10; // Interval waktu perubahan posisi teks (10 ms)

void IRAM_ATTR countPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(YF_B1_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(YF_B1_PIN), countPulse, RISING);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for 128x64)
  display.display();
  delay(3000);
  display.clearDisplay();
}

void loop() {
  
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
//    WiFiClientSecure *client = new WiFiClientSecure;
//    client->setInsecure(); //don't use SSL certificate
//    HTTPClient https;  // if serverName is using https:// replace all "http" syntax with "https"
    
    WiFiClient client;
    HTTPClient http; 
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Check if flowLiterPerMin has changed or it's time to send data
    if (flowLiterPerMin != prevFlowLiterPerMin || (millis() - prevSendTime) >= sendInterval) {
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(flowLiterPerMin)
                            + "&value2=" + String(totalVolume) + "";
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
      
      // You can comment the httpRequestData variable above
      // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = https.POST("Hello, World!");
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = https.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
          
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Update previous flowLiterPerMin value and send time
      prevFlowLiterPerMin = flowLiterPerMin;
      prevSendTime = millis();
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  
  unsigned long currentTime = millis();
  unsigned long timeDifference = (currentTime - prevTime);
  
  if (timeDifference > 1000) { // Update flow rate every 1 second
    detachInterrupt(digitalPinToInterrupt(YF_B1_PIN));
    flowRate = pulseCount / (timeDifference / 1000.0);
    flowLiterPerMin = flowRate * 60 / 550; // 550 is the number of pulses per liter for YF-B1
    totalVolume += flowLiterPerMin / 60;
    pulseCount = 0;
    prevTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(YF_B1_PIN), countPulse, RISING);
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("WATER FLOW MONITORING");

  display.setCursor(0, 24);
  display.print("Debit    : ");
  display.setCursor(0, 34);
  display.print("Bulan ini: ");

  display.setCursor(scrollPosition, 56);
  display.print("Buka hydroflow.id pada Browser Anda untuk sumber kode dari project ini");

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(65, 24);
  display.print(flowLiterPerMin, 2);
  display.print(" L/m  ");

  display.setCursor(65, 34);
  display.print(totalVolume, 2);
  display.print(" L    ");
  
  display.display();  
  display.clearDisplay();

  // Scrolling effect
    scrollPosition--;
    if (scrollPosition < -430) { // -430 sesuai dengan panjang tulisan "Buka iot.bem-unwiku.com pada browser Anda untuk online monitoring"
      scrollPosition = 128; // Reset scroll position ke awal jika sudah selesai scrolling
    }

  delay(5); // speed scrolling effect
}
