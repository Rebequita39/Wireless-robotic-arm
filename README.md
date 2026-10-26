# Wireless Teleoperated Arm

A wearable single-IMU controller drives a servo robot arm over a custom NRF24L01+ link, with a WiFi dashboard showing live link health. Built to practice signal acquisition and filtering (BME), custom protocol design and channel selection (telecom), and real-time scheduling, failsafes and latency measurement (systems).

## Architecture

```
[Controller node]  --NRF24 (ch X, 50 Hz cmd)-->  [Arm node]  --NRF24 (ch Y, 2-5 Hz stats)-->  [Gateway: ESP]
 rgbduino+GY-521                                  Arduino + servos                              WiFi dashboard
 + button/photoresistor   <--ACK payload (status)--                                             + serial/CSV log
```

- **Command link:** 50 Hz fixed rate, auto-ACK with the arm status in the ACK payload.
- **Telemetry link:** low-rate link statistics from the arm to the gateway.
- **Channel selection:** the 2.4 GHz scanner picks a clean channel at startup.

Wire formats are defined in [docs/packet_spec.md](docs/packet_spec.md).

## Modules

| Module | Purpose |
|---|---|
| [01-rf-scanner](01-rf-scanner/) | 2.4 GHz spectrum scanner for channel selection |
| [02-nrf-link](02-nrf-link/) | Radio layer: packets, CRC, sequence numbers, ACK, retries |
| [03-arm-firmware](03-arm-firmware/) | Servo control with rate and joint limits, failsafe |
| [04-controller-node](04-controller-node/) | IMU read, sensor fusion, calibration, mapping |
| [05-gateway-dashboard](05-gateway-dashboard/) | Telemetry receiver and WiFi dashboard |
| [tools](tools/) | Python logging, plotting and latency analysis |

## Results

Each stage adds its results here, including failures. See also [docs/what_went_wrong.md](docs/what_went_wrong.md).

- **v0.0** Radio hello test: _pending_
- **v0.1** Link protocol and loss vs. distance: _pending_
- **v0.2** Arm failsafe from serial: _pending_
- **v0.3** Radio-driven arm: _pending_
- **v0.4** IMU stability and mapping: _pending_
- **v0.5** Channel selection: _pending_
- **v0.6** Dashboard and telemetry: _pending_
- **v1.0** Final metrics and demo: _pending_

## How to run

Each module's README has its own wiring and build steps. Suggested order: 02, 03, 04, 05.

## Status

Repo scaffolded, protocol spec v1 drafted. Hardware bring-up is next.
