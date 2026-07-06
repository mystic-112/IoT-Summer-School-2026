## Q35 - REST API Weather Data Fetcher

This project demonstrates how an ESP32 can retrieve real-time weather information from the OpenWeatherMap REST API and compare it with local environmental measurements obtained from a DHT11 sensor.

The ESP32 connects to a Wi-Fi network and sends an HTTP GET request to the OpenWeatherMap API. The JSON response is parsed using the ArduinoJson library to extract useful information such as city name, temperature, humidity, and weather description.

At the same time, the ESP32 reads local temperature and humidity values from the DHT11 sensor. The program compares the API data with the locally measured values and calculates the difference between them.

The Serial Monitor displays:

* City name
* Weather description
* Temperature from OpenWeatherMap
* Temperature from DHT11
* Temperature difference
* Humidity from OpenWeatherMap
* Humidity from DHT11
* Humidity difference

Sensitive information such as Wi-Fi credentials and the OpenWeatherMap API configuration is stored in a separate `config.h` file. This file is excluded from version control using the repository `.gitignore` file to prevent accidental exposure of credentials.

### Technologies Used

* ESP32
* OpenWeatherMap REST API
* HTTP GET requests
* JSON parsing using ArduinoJson
* DHT11 temperature and humidity sensor
* Wi-Fi networking

### Learning Outcomes

* Working with REST APIs on embedded devices
* Sending HTTP requests from ESP32
* Parsing JSON responses

config.h

The config.h file stores all sensitive and user-specific configuration values required by the project, including Wi-Fi credentials and OpenWeatherMap API information.

This file is intentionally excluded from the Git repository through the .gitignore file to prevent accidental exposure of credentials.

Example contents include:

Wi-Fi SSID
Wi-Fi password
OpenWeatherMap API key or API URL
City and country configuration (if applicable)

Users cloning the repository should create their own config.h file locally before compiling the project.
* Integrating cloud data with local sensor measurements
* Secure management of API credentials using configuration files
