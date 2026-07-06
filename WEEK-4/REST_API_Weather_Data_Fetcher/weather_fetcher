#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
#include "config.h"

// dht sensor object
DHTesp dhtSensor;

// dht11 data pin
const int DHT_PIN = 15;

void fetchWeatherData();

void setup()
{
    Serial.begin(115200);

    // initialize dht11 sensor
    dhtSensor.setup(DHT_PIN, DHTesp::DHT11);

    // connect to wifi
    Serial.print("Connecting to WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    fetchWeatherData();
}

void loop()
{
    // fetch data every 60 seconds
    delay(60000);

    fetchWeatherData();
}

void fetchWeatherData()
{
    // read local dht11 values
    TempAndHumidity localData = dhtSensor.getTempAndHumidity();

    float localTemp = localData.temperature;
    float localHumidity = localData.humidity;

    HTTPClient http;

    Serial.println("\nFetching weather data from OpenWeatherMap API...");

    // start http connection using url from config.h
    http.begin(WEATHER_API_URL);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
        String payload = http.getString();

        DynamicJsonDocument doc(2048);

        DeserializationError error =
            deserializeJson(doc, payload);

        if (!error)
        {
            // extract api data
            String city = doc["name"];
            float apiTemp = doc["main"]["temp"];
            float apiHumidity = doc["main"]["humidity"];
            String weatherDescription =
                doc["weather"][0]["description"];

            // calculate differences
            float tempDifference =
                localTemp - apiTemp;

            float humidityDifference =
                localHumidity - apiHumidity;

            // display results
            Serial.println("\n====================================");
            Serial.println("WEATHER COMPARISON REPORT");
            Serial.println("====================================");

            Serial.print("City Name             : ");
            Serial.println(city);

            Serial.print("Weather Description   : ");
            Serial.println(weatherDescription);

            Serial.println();

            Serial.print("API Temperature        : ");
            Serial.print(apiTemp);
            Serial.println(" °C");

            Serial.print("Local DHT11 Temp       : ");
            Serial.print(localTemp);
            Serial.println(" °C");

            Serial.print("Temperature Difference : ");
            Serial.print(tempDifference);
            Serial.println(" °C");

            Serial.println();

            Serial.print("API Humidity           : ");
            Serial.print(apiHumidity);
            Serial.println(" %");

            Serial.print("Local DHT11 Humidity   : ");
            Serial.print(localHumidity);
            Serial.println(" %");

            Serial.print("Humidity Difference    : ");
            Serial.print(humidityDifference);
            Serial.println(" %");

            Serial.println("====================================");
        }
        else
        {
            Serial.println("JSON Parsing Failed");
        }
    }
    else
    {
        Serial.print("HTTP Error Code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}
