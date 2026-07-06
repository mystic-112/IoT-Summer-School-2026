## Q35 - REST API Weather Data Fetcher using ESP32

### Objective

The objective of this experiment is to demonstrate how an ESP32 can communicate with a cloud-based weather service using REST APIs and compare outdoor weather conditions with local indoor sensor measurements.

The ESP32 connects to a WiFi network and periodically fetches real-time weather information from the Open-Meteo weather API using an HTTPS GET request. At the same time, a DHT11 sensor connected to the ESP32 measures the local room temperature and humidity.

The fetched outdoor weather data and local sensor measurements are compared and displayed on the Serial Monitor along with automatically generated environmental insights and recommendations.

---

### Features

* Connects ESP32 to WiFi network.
* Fetches real-time outdoor weather data using REST API calls.
* Retrieves:

  * Outdoor temperature
  * Outdoor humidity
* Reads local indoor temperature and humidity using DHT11.
* Calculates:

  * Temperature difference between indoor and outdoor conditions.
  * Humidity difference between indoor and outdoor conditions.
* Generates human-readable environmental insights.
* Provides comfort recommendations based on room conditions.
* Uses HTTPS communication for secure API access.
* Stores sensitive credentials in a separate configuration file.

---

### Hardware Components

| Component                             | Quantity |
| ------------------------------------- | -------- |
| ESP32 Development Board               | 1        |
| DHT11 Temperature and Humidity Sensor | 1        |
| Breadboard                            | 1        |
| Jumper Wires                          | Several  |
| Micro USB Cable                       | 1        |

---

### Software Requirements

* Arduino IDE
* ESP32 Board Package
* ArduinoJson Library
* DHT Sensor Library for ESPx
* WiFi Library
* HTTPClient Library

---

### API Used

Outdoor weather information is obtained using the Open-Meteo REST API.

API Endpoint:

```text
https://api.open-meteo.com/v1/forecast?latitude=32.8083&longitude=74.8958&current=temperature_2m,relative_humidity_2m
```

The coordinates correspond to Jagti, Jammu, India.

The API provides:

* Outdoor temperature
* Outdoor relative humidity

Unlike OpenWeatherMap, Open-Meteo does not require API keys or account registration, making it highly suitable for educational and IoT prototyping applications.

---

### Project Structure

```text
REST_API/
│
├── REST_API.ino
├── config.h
└── .gitignore
```

---

### Configuration File

The `config.h` file stores all sensitive credentials and user-specific settings.

Example:

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define WEATHER_API_URL "https://api.open-meteo.com/v1/forecast?latitude=32.8083&longitude=74.8958&current=temperature_2m,relative_humidity_2m"

#endif
```

This file is excluded from version control using `.gitignore`.

---

### Working Principle

1. ESP32 connects to the WiFi network.
2. ESP32 sends an HTTPS GET request to the weather API.
3. The server returns weather data in JSON format.
4. ArduinoJson parses the JSON response.
5. ESP32 reads local sensor values from DHT11.
6. Indoor and outdoor values are compared.
7. Environmental insights and recommendations are generated.
8. Results are displayed on the Serial Monitor every 60 seconds.

---

### Sample Output

```text
========================================
WEATHER COMPARISON REPORT
========================================

Location               : Jagti, Jammu

Outside Temperature    : 34.2 °C
Room Temperature       : 31.0 °C
Temperature Difference : -3.2 °C

Outside Humidity       : 68 %
Room Humidity          : 61 %
Humidity Difference    : -7 %

WEATHER INSIGHTS
----------------------------------------
Outside Conditions     : Hot weather outside.
Outdoor Atmosphere     : Humid outdoor conditions.
Room Conditions        : Room feels warm.
Indoor Air Quality     : Indoor environment is humid.
Indoor vs Outdoor      : Room is cooler than outside.
Recommendation         : Indoor conditions are comfortable.

Observation            : Outdoor API values represent atmospheric conditions while DHT11 measures the immediate room environment.
========================================
```

---

### Learning Outcomes

After completing this experiment, the following concepts are understood:

* REST API communication using ESP32
* HTTPS communication in embedded systems
* HTTP GET requests
* JSON parsing using ArduinoJson
* Cloud-to-edge data integration
* IoT sensor interfacing
* Indoor and outdoor environmental comparison
* Secure credential management using configuration files
* Real-time weather monitoring applications

---

### Applications

* Smart homes
* Environmental monitoring systems
* Weather stations
* Smart classrooms
* Smart agriculture
* HVAC monitoring systems
* Building automation systems
* IoT-based comfort analysis systems

---

### Conclusion

This project successfully demonstrates the integration of cloud services with IoT sensors using ESP32. By combining real-time outdoor weather data with local environmental measurements, the system provides meaningful insights into indoor comfort conditions and demonstrates practical REST API usage in embedded systems.
