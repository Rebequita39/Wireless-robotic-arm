# 02-nrf-link

## Purpose
Radio layer shared by all nodes: packet encode/decode, CRC8, sequence numbers, auto-ACK with ACK payloads, and retry handling, following [docs/packet_spec.md](../docs/packet_spec.md).

## Wiring
NRF24L01+ on SPI: CE, CSN, SCK, MOSI, MISO, VCC (3.3 V, with a 10-100 uF + 0.1 uF cap at the module), GND, plus IRQ if used. Per-node pin tables live in [docs/wiring](../docs/wiring/). PA/LNA modules must not be powered from the MCU's 3.3 V pin.

## Build / Flash
TODO: library choice and build steps after Stage 0.

## Test / Demo
- Stage 0: send 1000 hello packets and log loss.
- Stage 1: measure loss at 1 m and 5 m, and confirm corrupted packets are rejected by the CRC (CRC8 of "123456789" must equal 0xF4).

## Known Issues
None yet.
