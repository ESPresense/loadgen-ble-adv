#include <Arduino.h>
#include <NimBLEDevice.h>

// Forward-declare NimBLE internal - avoids needing the internal include path
extern "C" {
  int ble_hs_id_set_rnd(const uint8_t *rnd_addr);
}

NimBLEAdvertising* adv = nullptr;
uint32_t seqNo = 0;

static void rotateRandomMac() {
  uint8_t addr[6];
  esp_fill_random(addr, 6);
  // BLE random static address: top 2 bits of MSB byte must be 11
  addr[5] |= 0xC0;
  ble_hs_id_set_rnd(addr);
}

static std::string makePayload(uint32_t n) {
  char buf[25];
  for (int i = 0; i < 24; i++) {
    buf[i] = 'A' + ((n + i) % 26);
  }
  buf[24] = '\0';
  return std::string(buf);
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("BLE load generator starting - random MAC rotation mode");

  NimBLEDevice::init("");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9); // max TX power
  NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM);

  rotateRandomMac();

  adv = NimBLEDevice::getAdvertising();
  adv->setMinInterval(0x20); // 20ms - BLE minimum
  adv->setMaxInterval(0x20); // force minimum interval for max throughput
  adv->setScanResponse(false);

  NimBLEAdvertisementData ad;
  ad.setFlags(0x06);
  ad.setManufacturerData(makePayload(0));
  adv->setAdvertisementData(ad);
  adv->start(0);
}

void loop() {
  seqNo++;

  adv->stop();
  rotateRandomMac(); // new random MAC every cycle

  NimBLEAdvertisementData ad;
  ad.setFlags(0x06);
  ad.setManufacturerData(makePayload(seqNo));
  adv->setAdvertisementData(ad);
  adv->start(0);

  if (seqNo % 500 == 0) {
    Serial.printf("loadgen seq=%lu mac-rotations=%lu\n",
                  (unsigned long)seqNo, (unsigned long)seqNo);
  }

  // No delay - run as fast as the BLE stack allows
  // Each cycle: stop → new random MAC → new payload → start
  // At 20ms minimum interval ~3 ad packets/MAC across all 3 channels
}
