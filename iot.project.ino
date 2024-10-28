#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DPIN 4        // Pin to connect DHT sensor (GPIO number)
#define DTYPE DHT11   // Define DHT 11 or DHT22 sensor type

const char* ssid = "SLTC_STAFF";            // Replace with your WiFi SSID
const char* password = "CrossRoads@2022";    // Replace with your WiFi password
const char* serverName = "https://weather.ceylonwebinnovators.xyz/submit_data.php";  // Replace with your server address

DHT dht(DPIN, DTYPE);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  dht.begin();
}

void loop() {
  delay(2000);

  float tc = dht.readTemperature(false);  // Read temperature in C
  float hu = dht.readHumidity();          // Read Humidity

  if (isnan(tc) || isnan(hu)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temp: ");
    Serial.print(tc);
    Serial.print(" C, Hum: ");
    Serial.print(hu);
    Serial.println("%");

    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = String(serverName) + "?temperature=" + String(tc) + "&humidity=" + String(hu);

      http.begin(serverPath.c_str());

      int httpResponseCode = http.GET();

      if (httpResponseCode>0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      }
      else {
        Serial.print("Error on sending GET: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}