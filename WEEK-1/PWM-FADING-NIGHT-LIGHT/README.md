# PWM Fading Night Light

## Objective
To demonstrate Pulse Width Modulation (PWM) by creating a smooth breathing LED effect.

## Components Required
- Arduino UNO
- LED
- 220Ω Resistor
- Breadboard
- Jumper Wires

## Connections
- LED Anode -> D9
- LED Cathode -> GND through 220Ω resistor

## Working Principle
The Arduino continuously changes the PWM duty cycle using `analogWrite()`, resulting in a gradual increase and decrease in LED brightness.

## Concepts Used
- PWM
- analogWrite()
- millis()
- Non-blocking timing
