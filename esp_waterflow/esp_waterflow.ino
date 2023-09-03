/*
  Rui Santos modified by Arif Nur Rizqi
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  github repositories https://github.com/arifnurrizqi/esp-waterflow-monitoring
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#define YF_B1_PIN 19 // pin of water flow sensor in pin 19

// Replace with your network credentials
const char* ssid     = "ARNUR"; //your wifi ssid
const char* password = "takonmama"; //your wifi password

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.0.15/esp-waterflow-monitoring/post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

volatile unsigned long pulseCount = 0;
volatile float flowRate;
float flowLiterPerMin;
float totalVolume = 0;
unsigned long prevTime = 0;
int Status = 1;
float indicator = 80;

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
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(flowLiterPerMin)
                           + "&value2=" + String(totalVolume) + "&value3=" + String(Status) + "&value4=" + String(indicator) + "";
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
  // while for Status it still uses random number generation, later it will be connected to the solenoid valve to open or close the valve
  Status = random (2); // Generate random numbers from 0 or 1
  // while the indicators still use random number generation, which will later be connected to the battery voltage sensor
  indicator = random(75, 101); // Generate random numbers from 75 to 100
  //Send an HTTP POST request every 5 seconds
  delay(5000);
}
