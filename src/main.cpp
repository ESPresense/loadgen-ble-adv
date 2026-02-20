/**
 * loadgen-ble-adv
 *
 * Arduino ESP32 BLE advertisement load generator.
 * Cycles through as many random MAC addresses as possible, emitting a
 * non-connectable BLE advertisement packet from each one before rotating
 * to the next.  Statistics (rotations/s) are printed to Serial every
 * REPORT_INTERVAL_MS milliseconds.
 */

#include <Arduino.h>
#include <NimBLEDevice.h>
#include "host/ble_hs.h"

// How long (ms) to advertise on each MAC before rotating.
// 20 ms is the minimum BLE advertising interval (32 × 0.625 ms).
static constexpr uint32_t ADV_DURATION_MS = 20;

// How often (ms) to print throughput statistics to Serial.
static constexpr uint32_t REPORT_INTERVAL_MS = 5000;

// Manufacturer-specific payload: company ID 0xFFFF (test/reserved) + "ESPLoad"
static const uint8_t kMfgData[] = {0xff, 0xff, 'E', 'S', 'P', 'L', 'o', 'a', 'd'};

static NimBLEAdvertising* pAdvertising = nullptr;
static uint32_t advCount = 0;
static uint32_t lastReportMs = 0;

/**
 * Generate a new random static BLE address and program it into the NimBLE
 * host.  Random static addresses require the two most-significant bits of
 * the most-significant byte to be 1 (0xC0 mask, per the BLE spec §1.3.2.1).
 */
static void rotateRandomAddress() {
    uint8_t addr[6];
    esp_fill_random(addr, sizeof(addr));
    addr[5] |= 0xC0;  // enforce random-static address format
    ble_hs_id_set_rnd(addr);
}

void setup() {
    Serial.begin(115200);
    Serial.println("BLE Advertisement Load Generator starting...");

    NimBLEDevice::init("");
    NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM);

    pAdvertising = NimBLEDevice::getAdvertising();

    // Non-connectable undirected advertising – lowest overhead, highest rate.
    pAdvertising->setAdvertisementType(BLE_GAP_CONN_MODE_NON);

    // Set both min and max interval to 20 ms (32 × 0.625 ms) for maximum
    // advertisement density.
    pAdvertising->setMinInterval(32);
    pAdvertising->setMaxInterval(32);

    NimBLEAdvertisementData advData;
    advData.setFlags(0x06);  // LE General Discoverable | BR/EDR Not Supported
    advData.setManufacturerData(
        std::string(reinterpret_cast<const char*>(kMfgData), sizeof(kMfgData)));
    pAdvertising->setAdvertisementData(advData);

    lastReportMs = millis();
    Serial.println("Setup complete – beginning MAC rotation.");
}

void loop() {
    // Stop current advertising, rotate to a new random MAC, then resume.
    pAdvertising->stop();
    rotateRandomAddress();
    pAdvertising->start(0);  // 0 = advertise until explicitly stopped

    delay(ADV_DURATION_MS);
    advCount++;

    const uint32_t now = millis();
    if (now - lastReportMs >= REPORT_INTERVAL_MS) {
        const uint32_t elapsed = now - lastReportMs;
        Serial.printf("MAC rotations: %u in %u ms  (%.1f rotations/s)\n",
                      advCount,
                      elapsed,
                      static_cast<float>(advCount) * 1000.0f / static_cast<float>(elapsed));
        advCount = 0;
        lastReportMs = now;
    }
}
