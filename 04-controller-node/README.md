# 04-controller-node

## Purpose
Wearable node: reads the MPU6050, fuses pitch and roll with a complementary filter, uses yaw rate with a deadzone for base rotation, handles calibration, recenter and gripper inputs, and transmits commands at 50 Hz.

## Wiring
GY-521 over I2C (check the rgbduino's logic level and pinout before connecting), NRF24 on SPI, one or two buttons, optional photoresistor on an ADC pin. Pin table: [docs/wiring](../docs/wiring/).

## Build / Flash
TODO.

## Test / Demo
Log output at rest for 60 s and report standard deviation and drift. Confirm the recenter button zeroes orientation.

## Known Issues
Yaw has no absolute reference (no magnetometer), so the design uses yaw rate plus a recenter button.
