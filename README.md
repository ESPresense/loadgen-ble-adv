# loadgen-ble-adv

Arduino ESP32 project that generates BLE advertisement load by cycling through as many random MAC addresses as possible.

## What it does

On every loop iteration the firmware:

1. Stops the current BLE advertiser.
2. Programs a new cryptographically-random *static random* MAC address into the NimBLE host (`ble_hs_id_set_rnd`).
3. Restarts advertising (non-connectable, 20 ms interval – the BLE minimum).
4. Waits 20 ms so at least one advertisement packet is emitted on each of the three advertising channels (37, 38, 39).
5. Prints throughput statistics to Serial every 5 s.

This maximises both the number of unique source MAC addresses seen by BLE scanners and the total advertisement packet rate.

## Hardware

Any ESP32 board with Bluetooth support:

| PlatformIO environment | Board |
|---|---|
| `esp32dev` (default) | Generic ESP32 / ESP-WROOM-32 |
| `esp32c3` | ESP32-C3 DevKitM-1 |
| `esp32s3` | ESP32-S3 Box |

## Build & flash

Install [PlatformIO](https://platformio.org/) (CLI or IDE extension), then:

```bash
# Build and flash the default environment (esp32dev)
pio run -t upload

# Monitor serial output
pio device monitor

# Use a specific board
pio run -e esp32c3 -t upload
```

## Serial output

```
BLE Advertisement Load Generator starting...
Setup complete – beginning MAC rotation.
MAC rotations: 198 in 5001 ms  (39.6 rotations/s)
MAC rotations: 200 in 5000 ms  (40.0 rotations/s)
```