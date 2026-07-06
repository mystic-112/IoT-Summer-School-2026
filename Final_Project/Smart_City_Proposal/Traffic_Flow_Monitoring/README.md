# Smart Traffic Flow Monitoring System

## Objective

Develop an IoT-based traffic monitoring node capable of measuring traffic density in real time and assisting traffic authorities in implementing adaptive traffic management strategies.

## Features

- Dual lane vehicle monitoring
- Vehicle counting
- Traffic density classification
- Congestion detection
- Dynamic signal timing recommendation
- Vehicle arrival rate estimation
- MQTT cloud integration
- Local congestion alarm
- Dashboard-ready JSON output

## Hardware Components

| Component | Quantity |
|-----------|----------|
| ESP32 Dev Board | 1 |
| IR Vehicle Sensor | 2 |
| Active Buzzer | 1 |
| Jumper Wires | Multiple |
| 5V Supply | 1 |

## System Workflow

IR Sensors detect passing vehicles on both lanes.

The ESP32 continuously counts vehicles and calculates:

- lane traffic count
- total traffic count
- traffic density level
- recommended green signal duration
- estimated vehicle flow rate

The processed data is published to the cloud using MQTT.

## Data Flow

Vehicle Sensor → ESP32 → MQTT → Cloud Server → Traffic Dashboard

## Traffic Classification

| Vehicle Count | Status |
|--------------|--------|
| 0-9 | Low |
| 10-24 | Medium |
| 25-39 | High |
| 40+ | Congested |

## Adaptive Signal Timing

| Traffic Level | Green Time |
|--------------|------------|
| Low | 20 sec |
| Medium | 35 sec |
| High | 50 sec |
| Congested | 70 sec |

## MQTT Topic

jammu/smartcity/traffic

## Example Payload

{
  "lane1_count":12,
  "lane2_count":18,
  "total_vehicles":30,
  "vehicle_rate_per_min":180,
  "traffic_status":"HIGH",
  "recommended_green_time":50
}

## Future Enhancements

- AI camera-based vehicle classification
- Emergency vehicle priority
- Automatic signal control
- Integration with city-wide traffic command center
- Historical traffic analytics
- Route optimization and diversion recommendations
