# OBD-II Simulator
---

This simulator is a simple tool designed for testing purposes.

It emulates the behavior of an On-Board Diagnostics II (OBD-II) sensor using Bluetooth Low Energy (BLE) communication.

## Overview
---
I am creating a mobile app that communicates with an OBD-II sensor. This project is designed to assist in its testing and debugging.

It emulates OBD-II sensor behavior, allowing you to simulate OBD data responses via BLE communication.

## Features
---
- Emulates OBD data responses based on received commands.
- Supports Bluetooth Low Energy (BLE) communication for mobile device interaction.
- Generates random data to simulate OBD responses.

## Getting Started
---

### Initial setup
Make sure to have Rust and Cargo [installed](https://www.rust-lang.org/tools/install).

Follow the steps in [The Rust on ESP3 Book](https://esp-rs.github.io/book/) to setup the toolchain.
- This project depends on the ```std``` library.

### Build and run
Once you have the environment set up, clone this repository and try to build it.

```bash
git clone https://github.com/KaueMiziara/esp32-obd-scan-sim.git

cargo build
```

- Note that the first build will take some time.

If it builds sucessfully, you may flash it into you ESP32.
The following example flashes the code in a board connected to [PORT] and opens the serial monitor:

```bash
cargo espflash flash -p [PORT] --monitor
```

### Sending OBD commands
> TODO

## License
---
This project is licensed under the GPLv3 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
---

> TODO

---

**Note:** This project is a work in progress and may have limitations or incomplete features. It is primarily intended for testing and development purposes.
