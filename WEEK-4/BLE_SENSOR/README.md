# Bluetooth Sensor Dashboard (ESP32 BLE)

## Overview

This project demonstrates how to use the ESP32 as a **Bluetooth Low Energy (BLE) GATT Server** to broadcast real-time environmental data collected from a DHT11 temperature and humidity sensor. The ESP32 advertises itself as a BLE peripheral and updates sensor readings every **3 seconds**. Any BLE scanning application such as **nRF Connect** or **LightBlue** can connect to the device and read the latest values.

The project implements the **Bluetooth SIG Environmental Sensing Service (ESS)** along with standard Temperature and Humidity characteristics. A custom Timestamp characteristic is also provided to indicate when the sensor values were last updated.

---

# Objectives

* Read temperature and humidity from a DHT11 sensor.
* Broadcast sensor data using Bluetooth Low Energy (BLE).
* Update sensor values every 3 seconds.
* Implement a BLE GATT Server with characteristics and descriptors.
* Display live sensor values using a BLE scanning application.
* Demonstrate BLE advertising, GATT services, characteristics, notifications, and descriptors.

---

# Features

* ESP32 configured as a BLE GATT Server.
* Uses the standard Bluetooth Environmental Sensing Service.
* Reads DHT11 sensor values every 3 seconds.
* Broadcasts:

  * Temperature
  * Humidity
  * Timestamp
* Supports BLE notifications.
* Automatically resumes advertising after client disconnection.
* Includes Characteristic User Description (BLE2901) descriptors.
* Includes Client Characteristic Configuration (BLE2902) descriptors.
* Displays live data using nRF Connect or LightBlue.

---

# Hardware Requirements

| Component                           |    Quantity |
| ----------------------------------- | ----------: |
| ESP32 Development Board             |           1 |
| DHT11 Temperature & Humidity Sensor |           1 |
| Breadboard                          |           1 |
| Jumper Wires                        | As required |
| USB Cable                           |           1 |

---

# Software Requirements

* Arduino IDE 2.x or later
* ESP32 Board Package
* DHT Sensor Library for ESPx (DHTesp)
* nRF Connect (Android/iOS) or LightBlue

---

# Circuit Connections

| DHT11 Pin | ESP32 Pin |
| --------- | --------- |
| VCC       | 3.3V      |
| DATA      | GPIO 15   |
| GND       | GND       |

---

# Working Principle

1. The ESP32 initializes the DHT11 sensor.
2. BLE is initialized and the ESP32 starts advertising.
3. A BLE client (phone) scans and connects to the ESP32.
4. Every 3 seconds:

   * Temperature is measured.
   * Humidity is measured.
   * A timestamp is generated using the ESP32 uptime.
5. The three characteristic values are updated.
6. Connected BLE clients receive the latest values through notifications.

---

# BLE GATT Structure

```text
ESP32 Environmental Sensor
│
└── Environmental Sensing Service (0x181A)
    │
    ├── Temperature Characteristic (0x2A6E)
    │     ├── BLE2901 User Description
    │     └── BLE2902 Client Configuration
    │
    ├── Humidity Characteristic (0x2A6F)
    │     ├── BLE2901 User Description
    │     └── BLE2902 Client Configuration
    │
    └── Timestamp Characteristic (Custom UUID)
          ├── BLE2901 User Description
          └── BLE2902 Client Configuration
```

---

# BLE Service UUID

| Service                       | UUID                                   |
| ----------------------------- | -------------------------------------- |
| Environmental Sensing Service | `0000181A-0000-1000-8000-00805F9B34FB` |

---

# Characteristic UUIDs

| Characteristic | UUID                                   | Type                   |
| -------------- | -------------------------------------- | ---------------------- |
| Temperature    | `00002A6E-0000-1000-8000-00805F9B34FB` | Bluetooth SIG Standard |
| Humidity       | `00002A6F-0000-1000-8000-00805F9B34FB` | Bluetooth SIG Standard |
| Timestamp      | `12345678-1234-5678-1234-56789ABCDEF1` | Custom UUID            |

---

# Characteristic Properties

| Characteristic | Read | Notify |
| -------------- | :--: | :----: |
| Temperature    |   ✅  |    ✅   |
| Humidity       |   ✅  |    ✅   |
| Timestamp      |   ✅  |    ✅   |

---

# Implemented GATT Descriptors

## BLE2901 – Characteristic User Description

Provides a human-readable description for each characteristic.

| Characteristic | Description         |
| -------------- | ------------------- |
| Temperature    | Temperature (°C)    |
| Humidity       | Humidity (%)        |
| Timestamp      | Timestamp (seconds) |

---

## BLE2902 – Client Characteristic Configuration Descriptor (CCCD)

Allows BLE clients to enable notifications and receive updated sensor values automatically every 3 seconds.

---

# Data Update Interval

Sensor readings are refreshed every:

**3 seconds**

Each update includes:

* Temperature
* Humidity
* Timestamp

---

# Testing Procedure

1. Upload the sketch to the ESP32.
2. Connect the DHT11 sensor according to the wiring diagram.
3. Open the Serial Monitor (115200 baud).
4. Verify that temperature and humidity values are printed every 3 seconds.
5. Enable Bluetooth on a smartphone.
6. Open nRF Connect or LightBlue.
7. Scan for:

```
ESP32 Environmental Sensor
```

8. Connect to the device.
9. Open the **Environmental Sensing Service**.
10. Read or enable notifications for:

    * Temperature
    * Humidity
    * Timestamp

---

# Expected Output

## Serial Monitor

```text
BLE Environmental Sensor Started

Temperature : 29.3 C
Humidity    : 62.0 %
Timestamp   : 3

Temperature : 29.4 C
Humidity    : 62.0 %
Timestamp   : 6
```

## nRF Connect / LightBlue

The application displays:

* Temperature
* Humidity
* Timestamp

The Temperature and Humidity characteristics appear as standard Bluetooth SIG characteristics. The Timestamp characteristic uses a custom UUID and may appear as **Unknown Characteristic** in some BLE scanner applications. Expanding the characteristic displays its **BLE2901 User Description** as **"Timestamp (seconds)"**.

---

# Project Folder Structure

```text
iot-summer-school-2026/
└── week4/
    └── ble_sensor/
        ├── ble_sensor.ino
        ├── README.md
        └── images/
            ├── circuit_diagram.png
            ├── serial_output.png
            └── nrf_connect_output.png
```

---

# Learning Outcomes

Through this project, the following concepts were implemented and understood:

* Bluetooth Low Energy (BLE)
* ESP32 BLE GATT Server
* Environmental Sensing Service
* GATT Characteristics
* BLE Advertising
* BLE Notifications
* BLE2901 User Description Descriptor
* BLE2902 Client Characteristic Configuration Descriptor
* DHT11 Sensor Interfacing
* Real-time Wireless Sensor Monitoring

---

# Conclusion

This project successfully implements a BLE-based wireless environmental monitoring system using the ESP32 and DHT11 sensor. The ESP32 acts as a BLE GATT Server, broadcasting temperature, humidity, and timestamp values every 3 seconds. Standard Bluetooth SIG services and characteristics are used wherever applicable, while GATT descriptors improve interoperability and readability. The system demonstrates practical BLE communication and can serve as the foundation for larger IoT and smart monitoring applications.
