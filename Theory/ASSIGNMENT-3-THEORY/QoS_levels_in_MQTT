# Q40. Quality of Service (QoS) Levels in MQTT

**Quality of Service (QoS)** in MQTT defines the **guarantee of message delivery between the publisher, MQTT broker, and subscriber**. Since IoT networks often operate over unreliable wireless links such as Wi-Fi, cellular, or LoRa, messages may occasionally be lost, duplicated, or delayed. MQTT addresses this problem by providing three QoS levels that allow developers to balance **reliability, latency, bandwidth usage, and power consumption** according to application requirements.

---

## 1. QoS 0 — "At Most Once"

### Definition

QoS 0 is the simplest and fastest delivery mechanism in MQTT. The publisher sends the message only once and does not wait for any acknowledgment from the broker.

This approach is often called **"fire and forget."**

### Communication Flow

```text
Publisher  -------- PUBLISH -------->  Broker
```

No confirmation packet is returned.

---

### Characteristics

* Message may be delivered once or not at all.
* No retransmission occurs if the message is lost.
* Lowest bandwidth consumption.
* Lowest latency.
* Minimal processing overhead.
* Best for non-critical data.

---

### Advantages

* Very fast communication.
* Conserves battery power.
* Reduces network traffic.
* Suitable for high-frequency sensor updates.

---

### Disadvantages

* No guarantee of delivery.
* Messages can be lost during network interruptions.

---

### Example Applications

* Temperature monitoring
* Humidity monitoring
* Ambient light sensing
* Weather station telemetry
* GPS tracking updates

If one temperature reading is lost, the next reading arrives shortly afterward, making occasional packet loss acceptable.

---

## 2. QoS 1 — "At Least Once"

### Definition

QoS 1 guarantees that the subscriber receives the message **at least once**. The publisher sends the message and waits for an acknowledgment (`PUBACK`) from the broker.

If the acknowledgment is not received within a timeout period, the publisher retransmits the message.

### Communication Flow

```text
Publisher  ---- PUBLISH ----> Broker
Publisher  <-- PUBACK ------ Broker
```

---

### Characteristics

* Delivery is guaranteed.
* Duplicate messages are possible.
* Retransmissions occur when acknowledgments are missing.
* Moderate bandwidth consumption.
* Moderate latency.

---

### Advantages

* Reliable message delivery.
* Suitable for important sensor data.
* Handles temporary network failures.

---

### Disadvantages

* Duplicate messages may occur.
* Additional network overhead due to acknowledgments.

Applications using QoS 1 should therefore be capable of detecting and ignoring duplicate messages.

---

### Example Applications

* Water level monitoring
* Smart agriculture irrigation systems
* Energy meter readings
* Industrial telemetry
* Security alarms

If a water tank reaches a critical level, losing that message could cause overflow or pump failure. Receiving the same message twice is usually less harmful than missing it entirely.

---

## 3. QoS 2 — "Exactly Once"

### Definition

QoS 2 provides the highest level of reliability by ensuring that a message is received **exactly once and only once**.

This is achieved using a four-step handshake mechanism.

### Communication Flow

```text
Publisher ---- PUBLISH ----> Broker
Publisher <-- PUBREC ------- Broker
Publisher ---- PUBREL -----> Broker
Publisher <-- PUBCOMP ------ Broker
```

---

### Characteristics

* Guaranteed delivery without duplicates.
* Highest reliability.
* Highest latency.
* Highest bandwidth usage.
* Most computational overhead.

---

### Advantages

* Prevents duplicate actions.
* Suitable for mission-critical operations.

---

### Disadvantages

* More network traffic.
* Increased latency.
* Greater power consumption.

---

### Example Applications

* Financial transactions
* Medical devices
* Billing systems
* Remote machinery control
* Chemical dosing systems

Executing a financial transaction twice or dispensing chemicals twice could have serious consequences, making QoS 2 necessary.

---

## Comparison of MQTT QoS Levels

| Feature                 | QoS 0            | QoS 1                 | QoS 2                 |
| ----------------------- | ---------------- | --------------------- | --------------------- |
| Delivery Guarantee      | No               | Yes                   | Yes                   |
| Duplicate Messages      | No               | Possible              | No                    |
| Acknowledgment Required | No               | PUBACK                | PUBREC/PUBREL/PUBCOMP |
| Bandwidth Usage         | Low              | Medium                | High                  |
| Latency                 | Low              | Medium                | High                  |
| Power Consumption       | Low              | Medium                | High                  |
| Reliability             | Low              | Medium                | Very High             |
| Typical Use Case        | Sensor Telemetry | Alerts and Monitoring | Critical Transactions |

---

## QoS Selection for an IoT Water Level Monitoring System

For an **IoT water level monitoring system**, the most appropriate choice is **QoS 1 (At Least Once)**.

### Reasoning

A water level monitoring system may be used for:

* Overhead water tanks
* Underground reservoirs
* Industrial storage tanks
* Irrigation systems
* Flood monitoring systems

Suppose the water level reaches **95% capacity** and an alert message is generated to stop the pump.

If **QoS 0** is used and that message is lost due to temporary Wi-Fi interference, the pump may continue running and cause:

* Tank overflow
* Water wastage
* Equipment damage
* Increased electricity costs

Therefore, QoS 0 is not sufficiently reliable.

Using **QoS 2** would guarantee exactly one delivery, but it introduces additional packet exchanges and higher power consumption. In water level monitoring, receiving the same alert twice is generally not harmful because the application can simply ignore duplicate readings.

QoS 1 offers the best balance:

* Reliable message delivery
* Moderate bandwidth usage
* Lower latency than QoS 2
* Lower power consumption than QoS 2
* Suitable for battery-powered IoT nodes

---

## Example Scenario

Consider a smart water tank system:

```text
Water Level Sensor
        ↓
ESP32 publishes:
"Tank Level = 95%"
        ↓
MQTT Broker
        ↓
Mobile App receives alert:
"Water Tank Almost Full - Stop Pump"
```

With QoS 1:

* If the acknowledgment is lost, the ESP32 retransmits the message.
* The user may receive the alert twice.
* Duplicate notifications are acceptable compared to missing the alert completely.

---

## Conclusion

MQTT QoS levels allow IoT developers to trade reliability for speed and resource usage:

* **QoS 0** prioritizes speed and efficiency.
* **QoS 1** prioritizes reliable delivery with acceptable overhead.
* **QoS 2** prioritizes absolute delivery guarantees.

For an **IoT water level monitoring system**, **QoS 1 (At Least Once)** is the optimal choice because it ensures reliable delivery while maintaining reasonable bandwidth usage, latency, and power consumption, making it ideal for real-world IoT deployments.
