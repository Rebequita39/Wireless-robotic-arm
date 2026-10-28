// Host-side tests (PC). Build: see 02-nrf-link/test/README or the command in the commit message.
#include <stdio.h>
#include <string.h>
#include "../src/crc8.h"
#include "../src/packet.h"

static int failures = 0;
#define CHECK(cond, msg) do { if (!(cond)) { printf("FAIL: %s\n", msg); failures++; } } while (0)

static void dump(const char *label, const void *p, size_t n)
{
    const uint8_t *b = (const uint8_t *)p;
    printf("%s:", label);
    for (size_t i = 0; i < n; i++) printf(" %02X", b[i]);
    printf("\n");
}

// Flip every single bit of a packet; CRC8 must catch all of them.
static int count_undetected_bitflips(const void *pkt, size_t n,
                                     pkt_status_t (*validate)(const void *))
{
    uint8_t buf[32];
    int undetected = 0;
    for (size_t byte = 0; byte < n; byte++) {
        for (int bit = 0; bit < 8; bit++) {
            memcpy(buf, pkt, n);
            buf[byte] ^= (uint8_t)(1u << bit);
            if (validate(buf) == PKT_OK) undetected++;
        }
    }
    return undetected;
}

static pkt_status_t v_cmd(const void *p) { return cmd_validate((const cmd_packet_t *)p); }
static pkt_status_t v_ack(const void *p) { return ack_validate((const ack_packet_t *)p); }
static pkt_status_t v_tel(const void *p) { return telemetry_validate((const telemetry_packet_t *)p); }

int main(void)
{
    // Endianness assumption of the spec
    uint16_t probe = 0x0102;
    CHECK(((uint8_t *)&probe)[0] == 0x02, "host must be little-endian");

    // CRC8 test vector
    CHECK(crc8((const uint8_t *)"123456789", 9) == 0xF4, "crc8 test vector 0xF4");
    CHECK(crc8((const uint8_t *)"", 0) == 0x00, "crc8 of empty = init");

    // CMD roundtrip, including joint extremes
    int16_t j[4] = { -900, 0, 900, 123 };
    cmd_packet_t c;
    cmd_init(&c, PKT_TYPE_CMD, 0xABCD, j, CMD_FLAG_GRIPPER_CLOSED);
    CHECK(cmd_validate(&c) == PKT_OK, "cmd valid after init");
    CHECK(c.seq == 0xABCD && c.joint[0] == -900 && c.joint[2] == 900, "cmd fields preserved");
    dump("golden CMD", &c, sizeof(c));

    // Byte layout checks (little-endian on the wire)
    const uint8_t *raw = (const uint8_t *)&c;
    CHECK(raw[0] == PKT_VERSION && raw[1] == PKT_TYPE_CMD, "cmd header bytes");
    CHECK(raw[2] == 0xCD && raw[3] == 0xAB, "seq little-endian");
    CHECK(raw[4] == 0x7C && raw[5] == 0xFC, "joint0 -900 = 0xFC7C little-endian");
    CHECK(raw[12] == CMD_FLAG_GRIPPER_CLOSED, "flags at byte 12");

    // Bad version / type
    cmd_packet_t bad = c;
    bad.version = 2;
    bad.crc = crc8((const uint8_t *)&bad, sizeof(bad) - 1);
    CHECK(cmd_validate(&bad) == PKT_ERR_VERSION, "wrong version rejected");
    bad = c;
    bad.type = 0x7F;
    bad.crc = crc8((const uint8_t *)&bad, sizeof(bad) - 1);
    CHECK(cmd_validate(&bad) == PKT_ERR_TYPE, "unknown type rejected");

    // RECENTER and HEARTBEAT are valid CMD-family types
    cmd_init(&bad, PKT_TYPE_RECENTER, 1, j, 0);
    CHECK(cmd_validate(&bad) == PKT_OK, "recenter valid");
    cmd_init(&bad, PKT_TYPE_HEARTBEAT, 2, j, 0);
    CHECK(cmd_validate(&bad) == PKT_OK, "heartbeat valid");

    // ACK
    ack_packet_t a;
    ack_init(&a, 0xABCD, ARM_STATE_FAILSAFE, 0x05);
    CHECK(ack_validate(&a) == PKT_OK, "ack valid");
    dump("golden ACK", &a, sizeof(a));

    // Telemetry
    telemetry_packet_t t;
    telemetry_init(&t, 100000, 42, 3600000, 76, 18, ARM_STATE_OK);
    CHECK(telemetry_validate(&t) == PKT_OK, "telemetry valid");
    dump("golden TEL", &t, sizeof(t));

    // Single-bit corruption: CRC8 detects 100% of these
    CHECK(count_undetected_bitflips(&c, sizeof(c), v_cmd) == 0, "cmd: all single-bit flips detected");
    CHECK(count_undetected_bitflips(&a, sizeof(a), v_ack) == 0, "ack: all single-bit flips detected");
    CHECK(count_undetected_bitflips(&t, sizeof(t), v_tel) == 0, "tel: all single-bit flips detected");

    printf(failures ? "\n%d test(s) FAILED\n" : "\nall tests passed\n", failures);
    return failures ? 1 : 0;
}
