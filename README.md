# ButterBot robot firmware – NUIT OVERKLOKING mod

Fork of the [CircuitMess ButterBot](https://github.com/CircuitMess/ButterBot-Firmware-Public) firmware by **NUIT d.o.o.** ([nuit.hr](https://nuit.hr)).
⚠️ Use together with the [controller mod](https://github.com/nuit-dev/ButterBotCtrl-Firmware-Public) – flash both.

## What's new
- **Action menu:** OVERKLOKING, BENDER and ULTRON – the robot says a random quote and the controller shows it. Long quotes go sentence by sentence; Shut Up or Poke stops them.
- **Hold Poke 1 s:** drives ~10 cm forward and says "nju aj ti OVERKLOKING is the best!"
- **Settings → SENSOR:** turn off the front and/or floor proximity sensors, for surfaces where they misfire. With the floor sensor off, the robot can drive off a table edge.
- **Menu navigation fix** (in the controller mod): joystick up/down now reliably moves through the action menu.

## About nju aj ti OVERKLOKING
**nju aj ti OVERKLOKING** is a satirical comic strip about everyday IT life – users, sysadmins, printers that never work and all the chaos in between. It's drawn by Croatian comic artist **Dubravko Mataković** and published by NUIT. If you've ever worked in IT, you'll recognise yourself in it.
👉 Read it on [nuit.hr](https://nuit.hr)

## Build & flash
ESP-IDF 5.5.3: `idf.py build`, then `idf.py -p <port> flash`.
The robot has 16 MB flash, the controller 4 MB – don't mix up the firmwares.
Quotes live in `components/ButterBot-Common/src/Phrases.cpp`. Full change list: [NUIT-CHANGES.md](NUIT-CHANGES.md).

## Credits
Original firmware © CircuitMess (MIT licence). Mod by Cyberlord / NUIT d.o.o.

---
*Original CircuitMess README below.*

# ButterBot Controller Firmware

## Building

To build the ButterBot Controller firmware, you'll need the ESP-IDF. You can find the getting
started guide [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/).
The production firmware is built using IDF version 5.5.3.

All required components are contained in the repository, and managed components (LVGL,
esp_new_jpeg) are fetched automatically by the IDF component manager during the build.

In the root directory of the project:

**To build the firmware** run ```idf.py build```

**To upload the firmware to the device** run ```idf.py -p <PORT> flash```. Replace `<PORT>` with
the port the controller is attached to, for ex. ```COM6``` or ```/dev/ttyACM0```.

### Asset archives

The SPIFFS filesystem image is built automatically from the pre-generated archives in
[spiffs_image](spiffs_image). If you modify the source assets in
[spiffs_archives](spiffs_archives), regenerate the archives by running
[scripts/archive.sh](scripts/archive.sh) from the project root. See
[scripts/archive.sh.md](scripts/archive.sh.md) for details.

## Restoring the stock firmware

To restore the stock firmware, you can download the prebuilt binary on
the [releases page](https://github.com/CircuitMess/ButterBotCtrl-Firmware-Public/releases) of
this repository and flash it manually using esptool:

```shell
esptool -c esp32s3 -b 921600 -p <PORT> write_flash 0 ButterBotCtrl-Firmware.bin
```

Alternatively, you can also do so using [CircuitBlocks](https://code.circuitmess.com/) by
logging in, clicking the "Restore Firmware" button in the top-right corner, and following the
on-screen instructions.
