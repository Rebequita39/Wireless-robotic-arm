#pragma once
// Packet formats for the wireless arm link. Authoritative spec: docs/packet_spec.md
// Conventions: little-endian, packed structs, CRC8 (see crc8.h).
// AVR, ESP8266/ESP32 and x86 are all little-endian, so structs go on air as-is.
// Any change to a format must bump PKT_VERSION.

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PKT_VERSION        1u

// Types
#define PKT_TYPE_CMD       0x01u
#define PKT_TYPE_RECENTER  0x02u
#define PKT_TYPE_HEARTBEAT 0x03u
#define PKT_TYPE_TELEMETRY 0x10u

// Joints: int16, 0.1 deg units, centered at 0 (+/-900 = +/-90 deg)
#define PKT_NUM_JOINTS     4u
#define JOINT_BASE         0u
#define JOINT_SHOULDER     1u
#define JOINT_ELBOW_WRIST  2u
#define JOINT_GRIPPER      3u
#define JOINT_LIMIT_DECI   900

// Command flags
#define CMD_FLAG_GRIPPER_CLOSED (1u << 0)
#define CMD_FLAG_MODE           (1u << 1)

// Arm state (ACK and telemetry)
#define ARM_STATE_OK       0u
#define ARM_STATE_FAILSAFE 1u

#if defined(__GNUC__)
  #define PKT_PACKED __attribute__((packed))
#else
  #error "Packed structs need GCC/Clang"
#endif

// Controller -> arm, 14 bytes
typedef struct PKT_PACKED {
    uint8_t  version;
    uint8_t  type;
    uint16_t seq;
    int16_t  joint[PKT_NUM_JOINTS];
    uint8_t  flags;
    uint8_t  crc;      // over bytes 0..12
} cmd_packet_t;

// Arm -> controller (ACK payload), 5 bytes
typedef struct PKT_PACKED {
    uint16_t last_seq;
    uint8_t  arm_state;
    uint8_t  limit_flags;  // bit n = joint n clamped
    uint8_t  crc;          // over bytes 0..3
} ack_packet_t;

// Arm -> gateway, 19 bytes
typedef struct PKT_PACKED {
    uint8_t  version;
    uint8_t  type;
    uint32_t rx_count;
    uint32_t missed_count;
    uint32_t uptime_ms;
    uint8_t  channel;
    uint16_t cmd_age_ms;
    uint8_t  arm_state;
    uint8_t  crc;          // over bytes 0..17
} telemetry_packet_t;

#ifdef __cplusplus
  static_assert(sizeof(cmd_packet_t) == 14, "cmd_packet_t must be 14 bytes");
  static_assert(sizeof(ack_packet_t) == 5, "ack_packet_t must be 5 bytes");
  static_assert(sizeof(telemetry_packet_t) == 19, "telemetry_packet_t must be 19 bytes");
#else
  _Static_assert(sizeof(cmd_packet_t) == 14, "cmd_packet_t must be 14 bytes");
  _Static_assert(sizeof(ack_packet_t) == 5, "ack_packet_t must be 5 bytes");
  _Static_assert(sizeof(telemetry_packet_t) == 19, "telemetry_packet_t must be 19 bytes");
#endif

typedef enum {
    PKT_OK = 0,
    PKT_ERR_CRC,
    PKT_ERR_VERSION,
    PKT_ERR_TYPE
} pkt_status_t;

// Fill version + CRC. Caller sets type, seq, joints, flags beforehand via *_init.
void cmd_init(cmd_packet_t *p, uint8_t type, uint16_t seq,
              const int16_t joint[PKT_NUM_JOINTS], uint8_t flags);
pkt_status_t cmd_validate(const cmd_packet_t *p);

void ack_init(ack_packet_t *p, uint16_t last_seq, uint8_t arm_state, uint8_t limit_flags);
pkt_status_t ack_validate(const ack_packet_t *p);

void telemetry_init(telemetry_packet_t *p, uint32_t rx_count, uint32_t missed_count,
                    uint32_t uptime_ms, uint8_t channel, uint16_t cmd_age_ms,
                    uint8_t arm_state);
pkt_status_t telemetry_validate(const telemetry_packet_t *p);

#ifdef __cplusplus
}
#endif
