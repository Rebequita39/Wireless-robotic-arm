#include <SPI.h>
#include <RF24.h>
#include "config.h"

RF24 radio(PIN_CE, PIN_CSN);
struct __attribute__((packed)) Hello { uint16_t seq; };

uint16_t seq = 0;
uint32_t sent = 0, failed = 0, lastTx = 0;

void setup() {
  Serial.begin(115200);
  if (!radio.begin()) { Serial.println("radio not responding"); while (1); }
  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setRetries(2, 5);   // ~4 ms worst case, stays inside the 20 ms period
  radio.openWritingPipe((const uint8_t*)RF_ADDRESS);
  radio.stopListening();
  delay(2000);              // let the RX board come up
  lastTx = millis();
}

void loop() {
  if (sent >= HELLO_COUNT) {
    Serial.print("DONE sent="); Serial.print(sent);
    Serial.print(" failed="); Serial.println(failed);
    while (1);
  }
  if (millis() - lastTx >= HELLO_PERIOD_MS) {
    lastTx += HELLO_PERIOD_MS;
    Hello h = { seq++ };
    if (!radio.write(&h, sizeof(h))) failed++;
    sent++;
  }
}
