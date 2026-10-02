# ButterBot controller firmware – nju aj ti OVERKLOKING mod

Fork of the [CircuitMess ButterBot controller](https://github.com/CircuitMess/ButterBotCtrl-Firmware-Public) firmware by **NUIT d.o.o.** ([nuit.hr](https://nuit.hr)).
⚠️ Use together with the [robot mod](https://github.com/nuit-dev/ButterBot-Firmware-Public) – flash both.

**What can I say to it?** [Voice commands](https://github.com/nuit-dev/ButterBot-Firmware-Public/blob/nuit-overkloking/docs/VOICE-COMMANDS.md) · **What does it recognise?** [Objects and faces](https://github.com/nuit-dev/ButterBot-Firmware-Public/blob/nuit-overkloking/docs/VISION.md)

## What's new

### v5 – Talkie Toaster, all the way
- With **VOICE → TALKIE TOASTER**, the screen shows the robot's new Toaster lines for every message (465 of them: greetings, battery, modules, lights, the clock, the camera, faces, phone, IR, movement, dancing, errors), including the TERMINATE CONSCIOUSNESS refusals, which the toaster now says himself.
- Flash together with the [robot mod v5](https://github.com/nuit-dev/ButterBot-Firmware-Public/releases/tag/v5). The protocol did not change.

**I had a lot of fun playing with this robot, but this one is hands down the best mod yet. I have the robot on my table turned on non-stop, and to have something that actually reminisces a living Talkie Toaster from Red Dwarf in such an elegant (and eloquent manner) is an absolute blast! Do try it!**

### v4.3 – TERMINATE CONSCIOUSNESS
- The last menu item, SHUTDOWN, is now **TERMINATE CONSCIOUSNESS** (two lines next to the gear icon), and the YES / NO question is gone. Picking it either shows HAL's refusal in a box with **CLOSE** – back to the menu, the robot says it too – or starts *Daisy Bell* right away. HAL refuses a random number of times first (0–4, mostly one or two), never with the same line twice, and "..." is always followed by one more line.
- Flash together with the [robot mod v4.3](https://github.com/nuit-dev/ButterBot-Firmware-Public/releases/tag/v4.3), which says the refusals. With an older robot the box still appears, but HAL stays silent.

### v4.2 – ROAMING
- **Settings → ROAMING**, right below SENSOR: OFF keeps the robot in place while idle – no wandering, and it no longer turns and drives towards faces, it only looks straight ahead. Voice commands, Summon and dancing still move it. Settings now has 12 rows.
- Flash together with the [robot mod v4.2](https://github.com/nuit-dev/ButterBot-Firmware-Public/releases/tag/v4.2). Mixing with v4.1 works too, roaming then simply stays on.

### v4.1 – talking to ButterBot
- **[Voice commands](https://github.com/nuit-dev/ButterBot-Firmware-Public/blob/nuit-overkloking/docs/VOICE-COMMANDS.md)** and **[vision](https://github.com/nuit-dev/ButterBot-Firmware-Public/blob/nuit-overkloking/docs/VISION.md)** reference (in the robot repo): every phrase the robot understands, and the objects and faces its camera recognises.
- **Yoda mode:** the controller shows the reordered sentences exactly as the robot v4.1 says them (questions and sentences starting with why / if / but stay as they are).
- Flash together with the [robot mod v4.1](https://github.com/nuit-dev/ButterBot-Firmware-Public/releases/tag/v4.1). The protocol did not change.

### v4 – clock, volume and night mode
- **Settings** now scrolls – 11 rows, with a slim bar at the right edge that shows where you are. New rows: **VOLUME**, **NIGHT MODE** (OFF / 22-07 / 23-07 / 00-07), **NIGHT VOLUME**, **DATE** and **TIME**.
- **DATE / TIME** show the robot's clock. Press the joystick to edit: left / right picks the field, up / down changes it, press again to set the robot's clock (24 h).
- Current Time shows 14:05 instead of 14:5.
- Flash together with the [robot mod v4](https://github.com/nuit-dev/ButterBot-Firmware-Public/releases/tag/v4) – the robot and the controller talk a new protocol.

### v3.3 – STARTUP
- **Settings → STARTUP:** OFF / FAST / BOOST / OVERKLOKING – how much of the controller's startup is skipped, from the next boot on. OFF is stock. FAST connects to the robot while the intro plays. BOOST also skips the intro animation. OVERKLOKING also skips the pairing animation and scans continuously – home screen in about 4 s instead of 9.5 s.
- Faster boot on every level (no PSRAM memory test, fewer boot logs).
- The controller no longer gets stuck on the pairing screen when a connection drops while it is being set up (e.g. after restarting the controller while connected).
- The FCC ID / TELEC footer was removed from the Settings screen, so all six rows fit.
- Works with the [robot mod v3.2](https://github.com/nuit-dev/ButterBot-Firmware-Public/releases/tag/v3.2) – no robot changes.

### v3.2 – HRVATSKI
- **HRVATSKI** menu item after YODA ("BUTTER BOT ZNA I HRVATSKI"): the robot speaks Croatian lines, shown on the controller.

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

**Option A – one file** (`…-controller-full.bin`): simplest, but resets the controller's settings (theme, brightness, sleep, SENSOR, VOICE, STARTUP, VOLUME, NIGHT MODE, NIGHT VOLUME).

```shell
python -m esptool --chip esp32s3 -p <PORT> -b 460800 --before default_reset --after no_reset write_flash 0 ButterBot-OVERKLOKING-v5-controller-full.bin
```

**Option B – keep settings** (`…-controller-parts.zip`): unzip, then run in the unzipped folder:

```shell
python -m esptool --chip esp32s3 -p <PORT> -b 460800 --before default_reset --after no_reset write_flash "@flash_args"
```

After either option, reset the controller over USB:

```shell
python -c "import serial,time; s=serial.Serial(); s.port='<PORT>'; s.dtr=False; s.rts=False; s.open(); s.rts=True; time.sleep(0.1); s.rts=False"
```

Don't use `--after hard_reset` (esptool's default) – it left the controller stuck once, like the robot. Flash the [robot](https://github.com/nuit-dev/ButterBot-Firmware-Public#flash-without-building) too – it needs its own steps. SHA-256 checksums are in `SHA256SUMS-controller.txt` in the release.

## Build & flash
ESP-IDF 5.5.3: `idf.py build`, then flash from the `build` directory with the Option B command above (`--after no_reset` + reset over USB) – plain `idf.py flash` ends with `--after hard_reset`.
Copy `components/CMF/lib/glm` from the robot repo before building – it's missing here (git-ignored upstream).
The controller has 4 MB flash, the robot 16 MB – check with `esptool.py -p <PORT> flash_id` and don't mix up the firmwares.
⚠️ The **robot** needs its own steps – see [Build & flash in the robot repo](https://github.com/nuit-dev/ButterBot-Firmware-Public#build--flash).
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
