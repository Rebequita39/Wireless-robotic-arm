# 03-arm-firmware

## Purpose
Runs on the arm Arduino: receives commands, applies a rate limiter and hard joint limits, drives the servos, and enters failsafe (hold position) when commands stop arriving.

## Wiring
Servos on a separate 5 V supply with a common ground to the MCU. Power the servos after the MCU is up if possible. Watch for the Servo library's PWM conflict on pins 9 and 10 when planning the pin map. Pin table: [docs/wiring](../docs/wiring/).

## Build / Flash
TODO.

## Test / Demo
- Stage 2: drive the arm from serial, stop sending, and confirm it holds position after more than 200 ms.
- Stage 3: follow test commands over the radio, and confirm failsafe triggers when the transmitter is powered off.

## Known Issues
None yet.
