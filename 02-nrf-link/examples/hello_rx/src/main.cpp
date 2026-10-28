#include <SPI.h>
#include <RF24.h>
#include "config.h"

RF24 radio(PIN_CE, PIN_CSN);
struct __attribute__((packed)) Hello { uint16_t seq; };

uint16_t lastSeq = 0;
bool     first = true;
uint32_t rxCount = 0, missed = 0;

void setup() {
  Serial.begin(115200);
  if (!radio.begin()) { Serial.println("radio not responding"); while (1); }
  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(1, (const uint8_t*)RF_ADDRESS);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    Hello h;
    radio.read(&h, sizeof(h));
    if (!first) missed += (uint16_t)(h.seq - lastSeq - 1);
    first = false;
    lastSeq = h.seq;
    rxCount++;
    if (rxCount % 100 == 0) {
      Serial.print("rx="); Serial.print(rxCount);
      Serial.print(" missed="); Serial.println(missed);
    }
  }
}
