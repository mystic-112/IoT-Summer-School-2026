# Q20. Explain the `setup()` and `loop()` Functions in Arduino. What Happens if a Long `delay()` is Used? What is the Non-Blocking Alternative?

## Introduction

Every Arduino program, known as a sketch, consists of two mandatory functions:

1. `setup()`
2. `loop()`

These two functions form the backbone of every Arduino application, whether it is a simple LED blink program or a complex IoT system involving multiple sensors and communication protocols.

Understanding how these functions work is essential for designing efficient and responsive embedded systems.

---

# 1. The `setup()` Function

## Definition

The `setup()` function is executed only once when:

* The Arduino board is powered ON.
* The reset button is pressed.
* The microcontroller restarts.

It is primarily used for initialization tasks that need to be performed only once before the main program starts running.

---

## Typical Tasks Performed in `setup()`

### Pin Configuration

```cpp
pinMode(13, OUTPUT);
```

### Serial Communication Initialization

```cpp
Serial.begin(9600);
```

### Sensor Initialization

```cpp
dht.begin();
```

### Wi-Fi Initialization

```cpp
WiFi.begin(ssid, password);
```

---

## Example

```cpp
void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}
```

When the board starts, these instructions execute once and are never repeated unless the board resets.

---

## Real-World Analogy

Consider a classroom before a lecture begins:

* The teacher enters the room.
* The projector is switched on.
* Attendance is taken.
* Presentation slides are prepared.

These activities happen only once before the lecture starts.

The `setup()` function performs a similar role in Arduino programs.

---

# 2. The `loop()` Function

## Definition

After the `setup()` function finishes execution, Arduino repeatedly executes the `loop()` function forever.

This function contains the main logic of the application.

---

## Example

```cpp
void loop() {
    digitalWrite(13, HIGH);
    delay(1000);

    digitalWrite(13, LOW);
    delay(1000);
}
```

The LED continuously turns ON and OFF because the `loop()` function runs repeatedly.

---

## Typical Tasks Performed in `loop()`

* Reading sensors
* Processing data
* Sending information to cloud platforms
* Controlling actuators
* Communicating with users

---

## Real-World Analogy

Returning to the classroom example:

After preparation is complete:

* The teacher explains concepts.
* Students ask questions.
* Discussions continue.

These activities continue throughout the lecture.

Similarly, `loop()` continuously performs the main tasks of the system.

---

# Program Execution Flow

```text
Power ON
   ↓
setup()
   ↓
loop()
   ↓
loop()
   ↓
loop()
   ↓
Infinite repetition
```

---

# What Happens if a Long `delay()` is Used?

## Definition of `delay()`

The `delay()` function pauses program execution for a specified amount of time.

Example:

```cpp
delay(5000);
```

This pauses the microcontroller for 5 seconds.

---

## Problem with Long Delays

During the delay period, the microcontroller stops executing other instructions.

It cannot:

* Read sensors
* Process inputs
* Respond to button presses
* Communicate with cloud servers
* Update displays

The processor simply waits.

---

## Example

```cpp
void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(10000);

    digitalWrite(LED_BUILTIN, LOW);
    delay(10000);
}
```

Here the Arduino remains idle for 10 seconds at a time.

---

## Impact on Sensor Responsiveness

Suppose a fire detection system uses an MQ2 gas sensor.

```cpp
void loop() {
    delay(10000);

    int gasValue = analogRead(A0);

    if(gasValue > threshold) {
        activateAlarm();
    }
}
```

If smoke appears during the 10-second delay period, the system cannot detect it immediately.

The alarm may activate several seconds late.

In safety-critical applications this delay can be dangerous.

---

## Another Example

Consider an automatic door system:

```cpp
delay(5000);
```

If a person presses the button during those 5 seconds, the system ignores the input until the delay finishes.

This results in poor user experience and slow response.

---

# Blocking Programming

Programs that depend heavily on `delay()` are called **blocking programs** because execution is blocked while waiting.

Characteristics include:

* Poor responsiveness
* Missed sensor events
* Inefficient CPU usage
* Difficulty handling multiple tasks

---

# The Non-Blocking Alternative: `millis()`

Instead of stopping the processor, Arduino provides the `millis()` function.

`millis()` returns the number of milliseconds elapsed since the board started running.

---

## Example

```cpp
unsigned long previousTime = 0;
const long interval = 1000;

void loop() {
    unsigned long currentTime = millis();

    if(currentTime - previousTime >= interval) {
        previousTime = currentTime;

        digitalWrite(LED_BUILTIN,
                     !digitalRead(LED_BUILTIN));
    }
}
```

The LED still blinks every second, but the processor remains free to perform other tasks.

---

## Advantages of `millis()`

* Multiple tasks can run simultaneously.
* Sensors can be monitored continuously.
* Better responsiveness.
* Improved scalability.
* Efficient CPU utilization.

---

# Example: Smart Street Light

Using `delay()`:

```cpp
turnLightOn();
delay(30000);
turnLightOff();
```

The controller cannot detect new motion during those 30 seconds.

Using `millis()`:

```cpp
if(motionDetected) {
    lightOnTime = millis();
}

if(millis() - lightOnTime >= 30000) {
    turnLightOff();
}
```

The controller continues checking sensors while keeping track of elapsed time.

---

# Comparison Between `delay()` and `millis()`

| Parameter                    | delay() | millis()  |
| ---------------------------- | ------- | --------- |
| Stops program execution      | Yes     | No        |
| Processor usable during wait | No      | Yes       |
| Suitable for multitasking    | No      | Yes       |
| Sensor responsiveness        | Poor    | Excellent |
| Recommended for IoT systems  | No      | Yes       |

---

# Importance in IoT Systems

Modern IoT systems often need to perform several tasks simultaneously:

* Read sensors
* Publish MQTT data
* Handle Wi-Fi communication
* Update displays
* Process user input

Using `delay()` makes this difficult, while `millis()` allows all these tasks to coexist efficiently.

---

# Conclusion

The `setup()` function executes once and is responsible for initialization, while the `loop()` function runs continuously and contains the main program logic.

Long delays inside `loop()` block program execution and reduce responsiveness, making them unsuitable for modern IoT applications.

The preferred solution is to use `millis()` for non-blocking timing, enabling efficient multitasking and responsive embedded systems.
