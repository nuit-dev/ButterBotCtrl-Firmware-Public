# ButterBot controller firmware – nju aj ti OVERKLOKING mod

Fork of the [CircuitMess ButterBot controller](https://github.com/CircuitMess/ButterBotCtrl-Firmware-Public) firmware by **NUIT d.o.o.** ([nuit.hr](https://nuit.hr)).
⚠️ Use together with the [robot mod](https://github.com/nuit-dev/ButterBot-Firmware-Public) – flash both.

## What's new

### v3 – more characters
- **Settings → VOICE:** new TALKIE TOASTER and YODA (VADER is now shown as DARTH VADER). The VOICE box is as wide as THEME and SLEEP.
- **Action menu:** TALKIE TOASTER and YODA after HAL 9000, each in its own voice.
- **Long quotes** (Ultron, Daisy) are shown in full and scroll along with the robot.
- **Yoda / Talkie Toaster modes** change the lines on the controller too, the same way as on the robot.
- Menu subtitles keep their padding when selected.

### v2 – voices
- **Settings → VOICE:** NORMAL / HAWKING / VADER / HAL 9000 – the robot's voice for everything it says. Saved on the controller and sent to the robot on every connect. The Settings rows are a little slimmer so all five fit.
- **Action menu:** DARTH OVERKLOKING (Darth Vader), OVERHAWKING (Stephen Hawking) and HAL 9000 right after OVERKLOKING – each in its own voice.
- **SHUTDOWN** at the bottom of the menu, with the gear icon: asks "TERMINATE CONSCIOUSNESS?" (YES / NO, NO is preselected), then HAL sings *Daisy Bell* on the robot and it powers off.

### v1
- **Menu navigation fix:** joystick up/down now reliably moves through the action menu (the dominant stick axis wins, and left/right also move in the list).
- **Action menu:** OVERKLOKING, BENDER and ULTRON right after SETTINGS – the robot says a random quote and the controller shows it. Long quotes follow the robot sentence by sentence; Shut Up or Poke stops them.
- **Hold Poke 1 s:** a fill bar, then the robot drives ~10 cm forward and says "nju aj ti OVERKLOKING is the best!" A short press is still the normal poke.
- **Settings → SENSOR:** ALL ON / FRONT OFF / FLOOR OFF / ALL OFF, for surfaces where the robot's proximity sensors misfire. Saved on the controller and sent to the robot on every connect. With the floor sensor off, the robot can drive off a table edge.

## About nju aj ti OVERKLOKING

**OVERKLOKING** is a Croatian satirical comic by [Dubravko Mataković](https://nuit.hr/overkloking/dubravko-matakovic/), focused on computers, technology, everyday life and the increasingly absurd relationship between people and the digital world.

The comic began more than two decades ago, when the web looked very different and computers were still mysterious enough to be funny on their own. Over time, it grew far beyond IT: into social satire, black comedy, current events and the ongoing disasters of one thoroughly dysfunctional family.

After more than **1,000 published pages**, the original run ended at Christmas 2025.

At Easter 2026 it returned at NUIT under a new name:

**nju aj ti OVERKLOKING**

The format remains simple: **one new strip every Thursday**, freely available online and without advertising.

Expect computers, bureaucracy, artificial intelligence, family catastrophes, current events and technology that supposedly exists to make life easier – including robots that can *“do nothing instead of me, and do it better.”*

**Read the comic:** [nuit.hr/overkloking](https://nuit.hr/overkloking/)

[![nju aj ti OVERKLOKING #1133 – We're Screwed!](docs/overkloking-1133-we-are-screwed.avif)](https://nuit.hr/overkloking/najebasmo/)
*[nju aj ti OVERKLOKING #1133 – We're Screwed!](https://nuit.hr/overkloking/najebasmo/) by [Dubravko Mataković](https://nuit.hr/overkloking/dubravko-matakovic/), English edition (both pages in Croatian). Robots making robots – what could go wrong?*

## Flash without building
Ready-made images are attached to releases from [v3](https://github.com/nuit-dev/ButterBotCtrl-Firmware-Public/releases/tag/v3) on (v1 and v2 have to be built from source) – no ESP-IDF needed, only Python and esptool: `pip install esptool`.
`<PORT>` is e.g. `COM6` (Windows), `/dev/cu.usbserial-XXXX` (macOS) or `/dev/ttyUSB0` (Linux). First check that it's the controller – it must say **4MB**:

```shell
python -m esptool -p <PORT> flash_id
```

**Option A – one file** (`…-controller-full.bin`): simplest, but resets the controller's settings (theme, brightness, sleep, SENSOR, VOICE).

```shell
python -m esptool --chip esp32s3 -p <PORT> -b 460800 write_flash 0 ButterBot-OVERKLOKING-v3-controller-full.bin
```

**Option B – keep settings** (`…-controller-parts.zip`): unzip, then run in the unzipped folder:

```shell
python -m esptool --chip esp32s3 -p <PORT> -b 460800 write_flash "@flash_args"
```

The controller restarts by itself. Flash the [robot](https://github.com/nuit-dev/ButterBot-Firmware-Public#flash-without-building) too – it needs its own steps. SHA-256 checksums are in `SHA256SUMS-controller.txt` in the release.

## Build & flash
ESP-IDF 5.5.3: `idf.py build`, then `idf.py -p <PORT> flash` (fine for the controller).
Copy `components/CMF/lib/glm` from the robot repo before building – it's missing here (git-ignored upstream).
The controller has 4 MB flash, the robot 16 MB – check with `esptool.py -p <PORT> flash_id` and don't mix up the firmwares.
⚠️ The **robot** must not be flashed with plain `idf.py flash` – see [Build & flash in the robot repo](https://github.com/nuit-dev/ButterBot-Firmware-Public#build--flash).
Full change list: [NUIT-CHANGES.md](NUIT-CHANGES.md).

## Credits
Original firmware © CircuitMess (MIT licence). Mod by Cyberlord ([@kibergospodar](https://github.com/kibergospodar)) / NUIT d.o.o.
*Daisy Bell* (Harry Dacre, 1892) is in the public domain.

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
