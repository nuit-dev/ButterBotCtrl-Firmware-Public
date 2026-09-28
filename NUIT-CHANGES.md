# NUIT changes (ButterBot controller firmware)

Custom OVERKLOKING mod by NUIT d.o.o. Must be flashed together with the matching ButterBot robot firmware.

- Action menu (joystick press): SETTINGS, OVERKLOKING, BENDER, ULTRON, then stock items
  (`Util/ScenarioMapping.h`, subtitles in `Components/ActionElement.cpp`)
- Menu navigation fix: dominant joystick axis wins, threshold 40, LEFT/RIGHT also move in the list
  (`Services/JoystickInputLVGL.*`, `Components/ActionElement.cpp`)
- Poke: short press = stock poke, hold 1 s = OVERKLOKING drive + line (`Screens/HomeScreen.*`)
- `Components/HomeWindows/QuoteWindow` – shows the quote; long quotes follow the robot sentence by sentence
- ButterBot-Common is shared with the robot repo – keep it identical in both.
- Settings: 4th row SENSOR (ALL ON / FRONT OFF / FLOOR OFF / ALL OFF), `Components/SettingsWindow/SensorSelector`.
  Stored in NVS key "Sensors" (separate from the settings blob), sent to the robot on change and on every connect
  (`Services/Com::setSensorCommand`). "Press joystick to return" hint removed to make room.
  Home screen guide shows "SENSORS: ... OFF" while any sensor is off.
- `components/CMF/lib/glm` is git-ignored in this repo's CMF copy but required – keep it (copy from the robot repo).
- Action menu: HRVATSKI ("BUTTER BOT ZNA I HRVATSKI") after YODA - Croatian lines, in the VOICE setting's voice.
- Action menu: DARTH OVERKLOKING, OVERHAWKING, HAL 9000, TALKIE TOASTER, YODA right after OVERKLOKING (shown in
  `QuoteWindow`). The robot speaks them in the Darth Vader / Hawking / HAL / Talkie Toaster / Yoda voice regardless
  of the VOICE setting.
- `Components/HomeWindows/QuoteWindow`: long quotes (Ultron, Daisy) are shown in full in a scrolling box that follows
  the robot sentence by sentence (scrolled to the part index the robot sends).
- Action menu subtitles keep their 2 px side padding when focused (they touched the selection box).
- Action menu: SHUTDOWN as the last item, with the gear icon like SETTINGS (`ActionElement::addButton(..., icon)`).
  Asks "TERMINATE CONSCIOUSNESS?" YES / NO first (`ActionElement::showConfirm`, NO focused), then sends
  `DaisySong`: the robot sings Daisy as a dying HAL and powers off.
- Settings: 5th row VOICE (NORMAL / HAWKING / DARTH VADER / HAL 9000 / TALKIE TOASTER / YODA),
  `Components/SettingsWindow/VoiceSelector` (box as wide as THEME / SLEEP). TALKIE TOASTER and YODA also switch
  `Phrases::toasterMode` / `yodaMode` on the controller (`applyVoiceModeToPhrases` in `Services/Settings.h`).
  Stored in NVS key "Voice", sent to the robot on change and on every connect (`Services/Com::setVoiceCommand`,
  `Ctrl::Voice*` commands). All Settings rows are 15 px high (were 18 px).
- Settings: 6th row STARTUP (OFF / FAST / BOOST / OVERKLOKING), `Components/SettingsWindow/FastStartSelector`, NVS
  key "FastStart", read at the next boot (`main.cpp`):
  - FAST: `gap->connect()` right after boot instead of after the intro; if connected when the intro ends,
    `IntroScreen` goes straight to `HomeScreen`
  - BOOST: + no intro animation (starts on `PairingScreen`)
  - OVERKLOKING: + no pairing animation (static "CONNECTING..." screen that loads the theme while BLE connects)
    and a continuous BLE scan (`GAP::setContinuousScan`, window = interval)
  - `GAP::connect()` waits for `ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT` (max 2 s) - scanning right after
    boot with an RPA before privacy is set up can fail
- The FCC ID / TELEC footer was removed from the Settings screen so all six rows fit (no e-label on screen).
- BLE recovery (all STARTUP levels): `PairingScreen::loop()` connects again whenever BLE is idle and goes home if
  already connected; a failed open returns to Idle; a failed service search disconnects at once. Fixes getting
  stuck on the pairing screen when the robot still holds the previous link (controller restarted while connected).
- `HomeScreen` ignores `Idle::ShutUp` like `Idle::BatteryLevel` (mute state is handled in `main.cpp`; it logged
  "Idle action not recognised").
- `sdkconfig`: PSRAM memory test off, bootloader and default log level WARN (~0.4 s faster boot). App image
  validation on boot stays on.

## Flashing

Do NOT flash the robot with esptool `--after hard_reset` (default of `idf.py flash`): it ends up stuck, ignores
the power button and drains the battery. The controller got stuck the same way once too, so flash both with
`--before no_reset --after no_reset`, then reset in software over USB (pulse RTS with DTR low). Check the port
first (16 MB = robot, 4 MB = controller). Turn the robot on with the power button, held 4–5 s. Details in the
robot repo's NUIT-CHANGES.md.
