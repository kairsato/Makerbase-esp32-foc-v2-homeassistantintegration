# MKS ESP32 FOC V2.0 + ESPHome + Home Assistant

This project integrates the Makerbase MKS ESP32 FOC V2.0 board with:

- ESPHome
- Home Assistant
- SimpleFOC

Features:

- Dual motor speed control
- Home Assistant sliders
- OTA updates
- Enable/disable controls
- Velocity mode

---

# Requirements

Install:

- Home Assistant
- ESPHome Addon OR ESPHome CLI
- VSCode (optional)

---

# Installation

## 1. Copy Files

Copy the `esphome` folder into your ESPHome config directory.

## 2. Edit WiFi

Update:

```yaml
wifi:
  ssid: "YOUR_WIFI"
  password: "YOUR_PASSWORD"
```

## 3. Compile + Upload

Use:

```bash
esphome run dual_foc.yaml
```

or upload through the ESPHome dashboard.

---

# Home Assistant Entities

You will get:

- number.motor_1_speed
- number.motor_2_speed
- switch.motor_enable

---

# IMPORTANT

This is open-loop velocity control initially.

For proper FOC you should later add:

- AS5600 encoder
- AS5048 encoder
- Hall sensors
