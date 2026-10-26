# Packet Specification

**Protocol version:** 0x01
This document is the contract between nodes. Any change to a wire format bumps `version` and adds a changelog entry.

## Conventions

- **Byte order:** little-endian for all multi-byte fields.
- **Structs:** packed (`__attribute__((packed))`) on every node. AVR and ESP differ in alignment.
- **CRC8:** polynomial 0x07, init 0x00, no input/output reflection, no final XOR. Covers every byte before the `crc8` field. Test vector: CRC8 of ASCII "123456789" = 0xF4.
- **Joint units:** int16, 0.1 degree per count, centered at 0 (neutral). Range +/-900 = +/-90 degrees.
- **Joint order:** 0 = base, 1 = shoulder, 2 = elbow/wrist, 3 = gripper.

## Command packet (controller -> arm), 14 bytes

| Offset | Size | Field | Notes |
|---|---|---|---|
| 0 | 1 | version | 0x01 |
| 1 | 1 | type | 0x01 CMD, 0x02 RECENTER, 0x03 HEARTBEAT |
| 2 | 2 | seq | uint16, wraps at 65535 |
| 4 | 8 | joint[4] | 4 x int16, see units above |
| 12 | 1 | flags | bit0 gripper closed, bit1 mode (0 lower arm, 1 upper arm), bits 2-7 reserved (0) |
| 13 | 1 | crc8 | over bytes 0-12 |

## ACK payload (arm -> controller), 5 bytes

| Offset | Size | Field | Notes |
|---|---|---|---|
| 0 | 2 | last_seq | last valid seq received |
| 2 | 1 | arm_state | 0 OK, 1 FAILSAFE |
| 3 | 1 | limit_flags | bit n = joint n clamped by joint limit |
| 4 | 1 | crc8 | over bytes 0-3 |

## Telemetry packet (arm -> gateway), 19 bytes

| Offset | Size | Field | Notes |
|---|---|---|---|
| 0 | 1 | version | 0x01 |
| 1 | 1 | type | 0x10 TELEMETRY |
| 2 | 4 | rx_count | valid packets received |
| 6 | 4 | missed_count | seq gaps detected |
| 10 | 4 | uptime_ms | arm uptime |
| 14 | 1 | channel | current RF channel |
| 15 | 2 | cmd_age_ms | time since last valid command |
| 17 | 1 | arm_state | 0 OK, 1 FAILSAFE |
| 18 | 1 | crc8 | over bytes 0-17 |

## Open questions

- **Retry count:** the transmitter (controller) observes retries, not the arm. Decide in Stage 1: log retries on the controller over serial, or add a field to a periodic controller packet.

## Changelog

- v1: initial draft.
