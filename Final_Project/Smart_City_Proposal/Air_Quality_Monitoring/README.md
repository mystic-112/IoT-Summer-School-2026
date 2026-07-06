# Smart Air Quality Monitoring System

## Objective

Develop an intelligent IoT-based environmental monitoring node capable of continuously monitoring air pollution levels and providing actionable health insights for citizens and municipal authorities.

## Features

* PM2.5 monitoring
* PM10 monitoring
* Harmful gas detection
* Temperature monitoring
* Humidity monitoring
* Air Quality Index estimation
* Pollution severity classification
* Heat index calculation
* Health advisory generation
* MQTT cloud integration
* Real-time alerts for dangerous pollution levels

## Hardware Components

| Component                  | Quantity |
| -------------------------- | -------- |
| ESP32 Development Board    | 1        |
| MQ135 Gas Sensor           | 1        |
| PMS5003 Particulate Sensor | 1        |
| DHT22 Sensor               | 1        |
| Active Buzzer              | 1        |
| Status LED                 | 1        |
| Power Supply               | 1        |

## System Workflow

The MQ135 continuously measures gas concentration while the PMS5003 measures particulate matter concentration in the air. The DHT22 records environmental conditions such as temperature and humidity.

The ESP32 processes the sensor values and calculates:

* Estimated AQI
* Pollution category
* Heat index
* Health advisory level

The processed information is uploaded to the cloud using MQTT where it can be visualized through dashboards and analytics tools.

## Data Flow

MQ135 + PMS5003 + DHT22 → ESP32 → MQTT → Cloud Platform → Smart City Dashboard

## AQI Classification

| AQI Range | Category     |
| --------- | ------------ |
| 0-50      | Good         |
| 51-100    | Satisfactory |
| 101-200   | Moderate     |
| 201-300   | Poor         |
| 301-400   | Very Poor    |
| 401+      | Severe       |

## Health Advisories

| AQI Category | Advisory                               |
| ------------ | -------------------------------------- |
| Good         | Safe for outdoor activities            |
| Moderate     | Sensitive groups should limit exposure |
| Poor         | Reduce prolonged outdoor exposure      |
| Severe       | Avoid outdoor activities               |

## MQTT Topic

`jammu/smartcity/airquality`

## Example MQTT Payload

```json
{
  "temperature": 32.4,
  "humidity": 58.2,
  "gas_level": 1680,
  "pm25": 92,
  "pm10": 146,
  "aqi": 184,
  "aqi_status": "MODERATE",
  "heat_index": 37.1,
  "health_advisory": "LIMIT_PROLONGED_EXPOSURE"
}
```

## Smart City Benefits

* Early detection of pollution hotspots
* Better environmental planning
* Public health protection
* Support for traffic restriction policies
* Historical pollution trend analysis
* Data-driven urban decision making

## Future Enhancements

* GPS tagging for pollution mapping
* AI-based pollution prediction
* Integration with weather forecasts
* Automatic public warning systems
* Integration with city emergency response centers
