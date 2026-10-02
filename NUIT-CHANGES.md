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
- v4 - Settings has 11 rows and scrolls (`SettingsWindow::RowY`, `scrollToRow`, 2 px scrollbar at the right edge):
  - VOLUME and NIGHT VOLUME (`Components/SettingsWindow/PercentSlider`, 10-100 %), NIGHT MODE
    (`NightModeSelector`: OFF / 22-07 / 23-07 / 00-07). NVS keys "Volume", "NightMode", "NightVol"; sent with
    `Com::setRobotConfig` on change and on every connect (`Ctrl::RobotConfig`).
  - DATE and TIME (`DateTimeRow`) show the robot's clock (`RobotState::getRobotTime`, from `Idle::TimeInfo`).
    The joystick press on these rows starts / confirms editing (`SettingsWindow::onJoystickPress`, called from
    `SettingsScreen` before it closes Settings) and sends `Ctrl::SetTime` (`Com::sendSetTime`).
  - `HomeScreen` ignores `Idle::TimeInfo`; `RambleWindow` shows the list from `RambleData::kind`;
    `CurrentTimeWindow` shows 14:05 instead of 14:5.
- v4.1 - ButterBot-Common only (identical to the robot repo): new Yoda reordering rules and respelled `pronounced`
  texts in `Phrases.cpp`, so `mapShown()` shows the same Yoda sentences the robot v4.1 speaks. Voice command and
  vision reference: `docs/VOICE-COMMANDS.md` and `docs/VISION.md` in the robot repo.
- v4.2 - Settings row ROAMING (`Components/SettingsWindow/RoamingSelector`, ON / OFF) below SENSOR, 12 rows
  (`RowY`, `DateRowIndex` 10, `TimeRowIndex` 11). Stored in `RobotConfigData::roaming` (NVS key "Roaming"), sent with
  `Ctrl::RobotConfig`; `Com::pack` keeps it in bit 24, the "set" marker moved to bit 31.
- v4.3 - SHUTDOWN renamed to TERMINATE CONSCIOUSNESS (`ScenarioMapping.h`, two lines next to the gear: 107 px
  does not fit one line with the icon). The YES / NO dialog is replaced by `ActionElement::onTerminate`: a random
  number of refusals (0-4, weights 5/30/30/20/15 %) before `DaisySong`, each an unused `Phrase::TerminateRefusal`
  line ("..." never last) shown in a popup with CLOSE over the list (`showRefusal`) and sent to the robot as
  `Scenario::TerminateRefusal` with the line index. The count and used lines are static, so they survive closing
  the list; sending Daisy starts a new sequence on the next pick.
- v5 (was v4.4) - ButterBot-Common only (identical to the robot repo): Talkie Toaster versions of every phrase category,
  picked by `Phrases::get()` / `map()` / `mapShown()` while `Phrases::toasterMode` (set from VOICE), so the screen shows
  the same Toaster line the robot says. The refusal popup uses the Toaster refusals in that mode.

## Flashing

Do NOT flash the robot with esptool `--after hard_reset` (default of `idf.py flash`): it ends up stuck, ignores
the power button and drains the battery. The controller got stuck the same way once too, so flash both with
`--before no_reset --after no_reset`, then reset in software over USB (pulse RTS with DTR low). Check the port
first (16 MB = robot, 4 MB = controller). Turn the robot on with the power button, held 4–5 s. Details in the
robot repo's NUIT-CHANGES.md.
