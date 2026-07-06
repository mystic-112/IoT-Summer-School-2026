#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
#include "config.h"

// dht sensor object
DHTesp dhtSensor;

// dht11 data pin
const int DHT_PIN = 15;

// function prototype
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
    Serial.println("\nWiFi Connected Successfully");

    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    fetchWeatherData();
}

void loop()
{
    // fetch weather data every minute
    delay(60000);

    fetchWeatherData();
}

void fetchWeatherData()
{
    // read local dht11 values
    TempAndHumidity localData =
        dhtSensor.getTempAndHumidity();

    float localTemp =
        localData.temperature;

    float localHumidity =
        localData.humidity;

    // secure client for https requests
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    Serial.println(
        "\nFetching weather data from Open-Meteo API...");

    http.begin(client, WEATHER_API_URL);

    int httpResponseCode =
        http.GET();

    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);

    // default values
    String city =
        "Jagti, Jammu";

    float apiTemp = 0;
    float apiHumidity = 0;

    if (httpResponseCode == 200)
    {
        String payload =
            http.getString();

        Serial.println("\nRAW API RESPONSE:");
        Serial.println(payload);

        DynamicJsonDocument doc(4096);

        DeserializationError error =
            deserializeJson(doc, payload);

        if (!error)
        {
            // extract open-meteo values
            apiTemp =
                doc["current"]["temperature_2m"] | 0.0;

            apiHumidity =
                doc["current"]["relative_humidity_2m"] | 0.0;
        }
        else
        {
            Serial.println("JSON Parsing Failed");
            Serial.println(error.c_str());
        }
    }
    else
    {
        Serial.println(
            "Failed to fetch weather data.");

        if (httpResponseCode == -1)
        {
            Serial.println(
                "Reason: Connection failed.");
        }

        Serial.println("\nSERVER RESPONSE:");
        Serial.println(http.getString());
    }

    // calculate differences
    float tempDifference =
        localTemp - apiTemp;

    float humidityDifference =
        localHumidity - apiHumidity;

    // report section
    Serial.println(
        "\n========================================");
    Serial.println(
        "       WEATHER COMPARISON REPORT");
    Serial.println(
        "========================================");

    Serial.print(
        "Location               : ");
    Serial.println(city);

    Serial.println();

    Serial.println(
        "TEMPERATURE COMPARISON");

    Serial.print(
        "Outside Temperature    : ");
    Serial.print(apiTemp);
    Serial.println(" °C");

    Serial.print(
        "Room Temperature       : ");
    Serial.print(localTemp);
    Serial.println(" °C");

    Serial.print(
        "Temperature Difference : ");
    Serial.print(tempDifference);
    Serial.println(" °C");

    Serial.println();

    Serial.println(
        "HUMIDITY COMPARISON");

    Serial.print(
        "Outside Humidity       : ");
    Serial.print(apiHumidity);
    Serial.println(" %");

    Serial.print(
        "Room Humidity          : ");
    Serial.print(localHumidity);
    Serial.println(" %");

    Serial.print(
        "Humidity Difference    : ");
    Serial.print(humidityDifference);
    Serial.println(" %");

    Serial.println();

    Serial.println(
        "WEATHER INSIGHTS");
    Serial.println(
        "----------------------------------------");

    // outside weather description
    Serial.print(
        "Outside Conditions     : ");

    if (apiTemp < 10)
    {
        Serial.println(
            "Very cold weather outside.");
    }
    else if (apiTemp < 20)
    {
        Serial.println(
            "Cool and pleasant weather outside.");
    }
    else if (apiTemp < 30)
    {
        Serial.println(
            "Pleasant outdoor conditions.");
    }
    else if (apiTemp < 38)
    {
        Serial.println(
            "Hot weather outside.");
    }
    else
    {
        Serial.println(
            "Extremely hot weather outside.");
    }

    // outdoor humidity analysis
    Serial.print(
        "Outdoor Atmosphere     : ");

    if (apiHumidity < 30)
    {
        Serial.println(
            "Dry outdoor air conditions.");
    }
    else if (apiHumidity < 60)
    {
        Serial.println(
            "Comfortable outdoor humidity.");
    }
    else if (apiHumidity < 80)
    {
        Serial.println(
            "Humid outdoor conditions.");
    }
    else
    {
        Serial.println(
            "Very humid outdoor atmosphere.");
    }

    // room analysis
    Serial.print(
        "Room Conditions        : ");

    if (localTemp < 20)
    {
        Serial.println(
            "Room feels cool.");
    }
    else if (localTemp < 28)
    {
        Serial.println(
            "Room temperature is comfortable.");
    }
    else if (localTemp < 33)
    {
        Serial.println(
            "Room feels warm.");
    }
    else
    {
        Serial.println(
            "Room feels hot.");
    }

    // room humidity analysis
    Serial.print(
        "Indoor Air Quality     : ");

    if (localHumidity < 30)
    {
        Serial.println(
            "Indoor air is dry.");
    }
    else if (localHumidity < 60)
    {
        Serial.println(
            "Indoor humidity is comfortable.");
    }
    else if (localHumidity < 80)
    {
        Serial.println(
            "Indoor environment is humid.");
    }
    else
    {
        Serial.println(
            "Indoor humidity is very high.");
    }

    // comparison
    Serial.print(
        "Indoor vs Outdoor      : ");

    if (localTemp > apiTemp + 2)
    {
        Serial.println(
            "Room is warmer than outside.");
    }
    else if (localTemp < apiTemp - 2)
    {
        Serial.println(
            "Room is cooler than outside.");
    }
    else
    {
        Serial.println(
            "Room temperature is similar to outside.");
    }

    // recommendation
    Serial.print(
        "Recommendation         : ");

    if (localTemp > 32 && localHumidity > 70)
    {
        Serial.println(
            "Use fan or ventilation for improved comfort.");
    }
    else if (localTemp > 35)
    {
        Serial.println(
            "Stay hydrated and keep the room ventilated.");
    }
    else if (localHumidity < 30)
    {
        Serial.println(
            "Air is dry. Hydration is recommended.");
    }
    else if (localHumidity > 80)
    {
        Serial.println(
            "High humidity detected. Improve ventilation if possible.");
    }
    else
    {
        Serial.println(
            "Indoor conditions are comfortable.");
    }

    Serial.println(
        "Observation            : Open-Meteo values represent outdoor atmospheric conditions in Jagti, Jammu while DHT11 measures indoor room conditions.");

    Serial.println(
        "========================================");

    http.end();
}
