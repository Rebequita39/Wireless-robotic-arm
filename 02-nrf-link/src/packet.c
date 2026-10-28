#include "packet.h"
#include "crc8.h"
#include <string.h>

// CRC covers every byte except the trailing crc field.
#define CRC_LEN(T) (sizeof(T) - 1u)

void cmd_init(cmd_packet_t *p, uint8_t type, uint16_t seq,
              const int16_t joint[PKT_NUM_JOINTS], uint8_t flags)
{
    memset(p, 0, sizeof(*p));
    p->version = PKT_VERSION;
    p->type    = type;
    p->seq     = seq;
    memcpy(p->joint, joint, sizeof(p->joint));
    p->flags   = flags;
    p->crc     = crc8((const uint8_t *)p, CRC_LEN(cmd_packet_t));
}

pkt_status_t cmd_validate(const cmd_packet_t *p)
{
    if (crc8((const uint8_t *)p, CRC_LEN(cmd_packet_t)) != p->crc) return PKT_ERR_CRC;
    if (p->version != PKT_VERSION) return PKT_ERR_VERSION;
    if (p->type != PKT_TYPE_CMD && p->type != PKT_TYPE_RECENTER &&
        p->type != PKT_TYPE_HEARTBEAT) return PKT_ERR_TYPE;
    return PKT_OK;
}

void ack_init(ack_packet_t *p, uint16_t last_seq, uint8_t arm_state, uint8_t limit_flags)
{
    memset(p, 0, sizeof(*p));
    p->last_seq    = last_seq;
    p->arm_state   = arm_state;
    p->limit_flags = limit_flags;
    p->crc         = crc8((const uint8_t *)p, CRC_LEN(ack_packet_t));
}

pkt_status_t ack_validate(const ack_packet_t *p)
{
    return (crc8((const uint8_t *)p, CRC_LEN(ack_packet_t)) == p->crc) ? PKT_OK : PKT_ERR_CRC;
}

void telemetry_init(telemetry_packet_t *p, uint32_t rx_count, uint32_t missed_count,
                    uint32_t uptime_ms, uint8_t channel, uint16_t cmd_age_ms,
                    uint8_t arm_state)
{
    memset(p, 0, sizeof(*p));
    p->version      = PKT_VERSION;
    p->type         = PKT_TYPE_TELEMETRY;
    p->rx_count     = rx_count;
    p->missed_count = missed_count;
    p->uptime_ms    = uptime_ms;
    p->channel      = channel;
    p->cmd_age_ms   = cmd_age_ms;
    p->arm_state    = arm_state;
    p->crc          = crc8((const uint8_t *)p, CRC_LEN(telemetry_packet_t));
}

pkt_status_t telemetry_validate(const telemetry_packet_t *p)
{
    if (crc8((const uint8_t *)p, CRC_LEN(telemetry_packet_t)) != p->crc) return PKT_ERR_CRC;
    if (p->version != PKT_VERSION) return PKT_ERR_VERSION;
    if (p->type != PKT_TYPE_TELEMETRY) return PKT_ERR_TYPE;
    return PKT_OK;
}
