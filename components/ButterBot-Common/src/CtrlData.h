#ifndef BUTTERBOT_COMMON_CONTROLLERDATA_H
#define BUTTERBOT_COMMON_CONTROLLERDATA_H

#include <cstdint>

struct Ctrl {
	enum Command {
		EnterRC, ExitRC, Listen, Summon, ShutUp, Poke, Drive, RCSound, Scenario,
		// Custom (NUIT) - proximity sensor filter
		SensorsAllOn, SensorsFrontOff, SensorsFloorOff, SensorsAllOff,
		// Custom (NUIT) - TTS voice preset, keep LAST
		VoiceNormal, VoiceHawking, VoiceVader, VoiceHal, VoiceToaster, VoiceYoda,
		// Custom (NUIT) - payload commands: RobotConfigData / SetTimeData, keep LAST
		RobotConfig, SetTime
	} type;
};

// Custom (NUIT): night mode - quieter, no idle comments or wandering. Every range ends at 07:00.
enum class NightMode : uint8_t {
	Off = 0, From22 = 1, From23 = 2, From00 = 3
};

inline bool isNightHour(NightMode mode, int hour){
	switch(mode){
		case NightMode::From22: return hour >= 22 || hour < 7;
		case NightMode::From23: return hour >= 23 || hour < 7;
		case NightMode::From00: return hour < 7;
		default: return false;
	}
}

// Custom (NUIT): robot settings from the controller's Settings screen, sent on change and on every connect
struct RobotConfigData {
	uint8_t volume;      // [10, 100] %
	uint8_t nightMode;   // NightMode
	uint8_t nightVolume; // [10, 100] %
	uint8_t roaming = 1; // 1 = may wander and turn to people while idle. Added in v4.2: a 3-byte payload means 1
};
static constexpr uint8_t RobotConfigDataV4Size = 3;

// Custom (NUIT): sets the robot's RTC (local time, 24 h)
struct SetTimeData {
	uint16_t year;
	uint8_t month;  // 1-12
	uint8_t day;    // 1-31
	uint8_t hour;   // 0-23
	uint8_t minute; // 0-59
};

struct DriveData {
	int8_t joystickX; // [-100, 100]
	int8_t joystickY; // [-100, 100]
};

struct RCData {
	char SSID[16];
	char password[16];
};

#endif //BUTTERBOT_COMMON_CONTROLLERDATA_H