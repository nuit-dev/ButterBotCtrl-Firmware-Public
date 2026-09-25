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
