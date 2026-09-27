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
  `Ctrl::Voice*` commands). All Settings rows are 15 px high (were 18 px) so five rows fit above the footer.

## Flashing the robot

Do NOT flash the robot with esptool `--after hard_reset` (default of `idf.py flash`): it ends up stuck, ignores
the power button and drains the battery. Check the port first (16 MB = robot, 4 MB = controller), flash with
`--before no_reset --after no_reset`, then reset in software over USB (pulse RTS with DTR low). Turn the robot on
with the power button, held 4–5 s. Details in the robot repo's NUIT-CHANGES.md.
