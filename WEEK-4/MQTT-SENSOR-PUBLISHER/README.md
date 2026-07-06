# MQTT Sensor Publisher using ESP32, DHT22 and MQTT

## Project Overview

This project was developed as part of the **IIT Jammu Summer School 2026 IoT Assignment** under the topic of MQTT communication and IoT cloud connectivity.

The objective of this project is to demonstrate how an ESP32 can collect environmental data from sensors and communicate with external applications using the MQTT publish-subscribe messaging protocol.

The ESP32 reads temperature and humidity values from a DHT22 sensor and publishes them periodically to a public MQTT broker in JSON format. At the same time, the ESP32 subscribes to a control topic and listens for commands from remote clients such as MQTT Explorer or MQTTX to control an LED connected to the microcontroller.

This project demonstrates two important IoT communication paradigms:

- Device to Cloud communication using MQTT publishing.
- Cloud to Device communication using MQTT subscriptions.

The implementation closely resembles how industrial IoT systems communicate with cloud platforms and remote dashboards.

---

# Objectives

- Learn MQTT publish-subscribe architecture.
- Connect ESP32 to a WiFi network.
- Interface environmental sensors with ESP32.
- Publish sensor data to a cloud broker.
- Receive remote commands from external applications.
- Control hardware devices remotely using MQTT.
- Understand JSON based IoT communication.

---

# Assignment Requirements Covered

| Requirement | Status |
|------------|--------|
| Connect ESP32 to WiFi | ✅ |
| Publish temperature readings | ✅ |
| Publish humidity readings | ✅ |
| Publish data every 5 seconds | ✅ |
| Use JSON payload format | ✅ |
| Subscribe to LED control topic | ✅ |
| Toggle LED using MQTT messages | ✅ |
| Test using MQTT Explorer or MQTTX | ✅ |
| Capture screenshots | ✅ |

---

# Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 Dev Module | 1 | Main microcontroller |
| DHT22 Sensor | 1 | Temperature and humidity sensing |
| LED | 1 | MQTT controlled actuator |
| 1kΩ Resistor | 1 | Current limiting for LED |
| Jumper Wires | As required | Circuit connections |

---

# Software Requirements

- Arduino IDE 2.x
- Wokwi Simulator
- MQTT Explorer or MQTTX
- ESP32 Board Package
- Internet connection

---

# Libraries Used

| Library | Purpose |
|---------|---------|
| WiFi.h | WiFi connectivity |
| PubSubClient | MQTT communication |
| DHT sensor library for ESPx | DHT22 sensor interface |

---

# Why DHT22 Was Used Instead of DHT11

The original assignment specifies the use of a DHT11 sensor.

Since the project was implemented using Wokwi simulation, DHT22 was used because it is the default environmental sensor provided by the simulator and is officially supported by Wokwi's ESP32 examples.

The communication protocol and implementation logic remain exactly the same.

In fact, DHT22 offers:

- Better accuracy
- Wider operating range
- Higher resolution

Therefore, the educational objective of the assignment remains fully satisfied.

---

# MQTT Architecture

This project follows the MQTT publish-subscribe communication model.

```
                     MQTT Broker
                  test.mosquitto.org
                           |
          ---------------------------------
          |                               |
          |                               |
      Publish                        Subscribe
          |                               |
          |                               |
       ESP32                       MQTT Explorer
```

The ESP32 acts both as:

- Publisher
- Subscriber

MQTT Explorer acts as:

- Subscriber
- Publisher

The broker acts as the middleman responsible for forwarding messages between devices.

---

# MQTT Broker Configuration

| Parameter | Value |
|----------|-------|
| Broker | test.mosquitto.org |
| Port | 1883 |
| Username | Not Required |
| Password | Not Required |

---

# MQTT Topics Used

## Temperature Topic

```
iitjammu/summer26/abhishektandon/temperature
```

Purpose:

Used by the ESP32 to publish temperature readings every 5 seconds.

Example message:

```json
{
  "value": 32.00,
  "unit": "C",
  "ts": 18193
}
```

---

## Humidity Topic

```
iitjammu/summer26/abhishektandon/humidity
```

Purpose:

Used by the ESP32 to publish humidity readings every 5 seconds.

Example message:

```json
{
  "value": 50.00,
  "unit": "%",
  "ts": 18193
}
```

---

## LED Control Topic

```
iitjammu/summer26/abhishektandon/led_control
```

Purpose:

Used by MQTT Explorer or MQTTX to send remote commands to ESP32.

Supported payloads:

```
ON
OFF
```

Payload values are case-sensitive.

| Payload | Action |
|---------|--------|
| ON | Turn LED ON |
| OFF | Turn LED OFF |

---

# Understanding MQTT Payloads

In MQTT, the payload is the actual message content being transmitted over a topic.

For example:

Topic:

```
iitjammu/summer26/abhishektandon/temperature
```

Payload:

```json
{
  "value": 32.00,
  "unit": "C",
  "ts": 18193
}
```

Similarly:

Topic:

```
iitjammu/summer26/abhishektandon/led_control
```

Payload:

```
ON
```

The ESP32 receives the payload and processes it to perform the desired action.

---

# Circuit Connections

## DHT22 Connections

| DHT22 Pin | ESP32 Pin |
|----------|-----------|
| VCC | 3.3V |
| DATA | GPIO15 |
| GND | GND |

---

## LED Connections

| LED Pin | ESP32 Pin |
|--------|-----------|
| Anode | GPIO2 |
| Cathode | GND through 1kΩ resistor |

---

# Program Workflow

## Step 1

ESP32 connects to the Wokwi WiFi network.

```
SSID     : Wokwi-GUEST
Password : None
```

---

## Step 2

ESP32 establishes a connection with the MQTT broker.

---

## Step 3

The DHT22 sensor measures:

- Temperature
- Humidity

---

## Step 4

The sensor values are converted into JSON format.

Example:

```json
{
  "value": 32,
  "unit": "C",
  "ts": 18193
}
```

---

## Step 5

ESP32 publishes the sensor data every 5 seconds.

---

## Step 6

ESP32 subscribes to the LED control topic.

---

## Step 7

MQTT Explorer publishes a command:

```
ON
```

---

## Step 8

The broker forwards the message to ESP32.

---

## Step 9

ESP32 receives the message and turns the LED ON.

---

## Step 10

If MQTT Explorer sends:

```
OFF
```

the LED turns OFF.

---

# Sample Serial Monitor Output

```text
connecting to wifi...
wifi connected
ip address: 10.13.37.2

connecting to mqtt broker connected
subscribed to led control topic

temperature published:
{"value": 32.00, "unit": "C", "ts": 18193}

humidity published:
{"value": 50.00, "unit": "%", "ts": 18193}
```

---

# Screenshots

The following screenshots should be included:

```
screenshots/
│
├── mqtt_connection.png
├── mqtt_messages.png
├── temperature_topic.png
├── humidity_topic.png
├── led_control_on.png
└── led_control_off.png
```

---

# Learning Outcomes

After completing this project, the following concepts were understood:

- MQTT protocol fundamentals
- Publish-subscribe communication
- Public MQTT brokers
- JSON message formatting
- Sensor integration with ESP32
- Remote hardware control
- IoT cloud communication
- Real-time telemetry systems

---

# Possible Future Improvements

- Add OLED display support.
- Integrate ThingSpeak dashboard.
- Store historical sensor data.
- Add multiple sensor nodes.
- Add threshold based alerts.
- Integrate Blynk mobile dashboard.
- Add email notifications.
- Use secure MQTT over TLS.

---

# Folder Structure

```text
week4/
└── mqtt_sensor_publisher/
    ├── mqtt_sensor_publisher.ino
    ├── libraries.txt
    ├── README.md
    └── screenshots/
        ├── mqtt_connection.png
        ├── mqtt_messages.png
        ├── temperature_topic.png
        ├── humidity_topic.png
        ├── led_control_on.png
        └── led_control_off.png
```

---

# Author

**Abhishek Tandon**

B.Tech Electronics and Communication Engineering  
Veermata Jijabai Technological Institute (VJTI), Mumbai

Summer Intern  
IIT Jammu Summer School 2026

---
