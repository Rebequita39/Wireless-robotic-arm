# 05-gateway-dashboard

## Purpose
ESP node that receives telemetry from the arm over its own NRF24 and serves a live WiFi dashboard (loss %, channel, command age), with serial/CSV logging for analysis.

## Wiring
NRF24 on the ESP's SPI pins, with decoupling caps at the module. Pin table: [docs/wiring](../docs/wiring/).

## Build / Flash
Copy `secrets.h.example` to `secrets.h` and fill in your WiFi credentials. `secrets.h` is gitignored and must never be committed. Remaining steps: TODO.

## Test / Demo
Dashboard shows loss %, channel and command age updating live while the arm is running.

## Known Issues
The ESP's WiFi may interfere with its own NRF24. Test this early, using separate channels and physical spacing.
