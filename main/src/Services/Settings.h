#ifndef BUTTERBOTCTRL_FIRMWARE_SETTINGS_H
#define BUTTERBOTCTRL_FIRMWARE_SETTINGS_H

#include "Object/Object.h"
#include "Object/Class.h"
#include <nvs.h>
#include <CtrlData.h>

#include "Themes.hpp"
#include <CtrlData.h>
#include <Phrases.h>

enum class InactivityTimeout : uint8_t {
	Min2 = 2, Min5 = 5, Min10 = 10, Min30 = 30, Off = 0xFF
};

// Custom (NUIT): robot TTS voice preset, stored separately from SettingsStruct
enum class VoiceMode : uint8_t {
	Normal = 0, Hawking = 1, Vader = 2, Hal = 3, Toaster = 4, Yoda = 5
};

inline Ctrl::Command voiceModeToCommand(VoiceMode mode){
	switch(mode){
		case VoiceMode::Hawking: return Ctrl::VoiceHawking;
		case VoiceMode::Vader: return Ctrl::VoiceVader;
		case VoiceMode::Hal: return Ctrl::VoiceHal;
		case VoiceMode::Toaster: return Ctrl::VoiceToaster;
		case VoiceMode::Yoda: return Ctrl::VoiceYoda;
		default: return Ctrl::VoiceNormal;
	}
}

// Custom (NUIT): how much of the controller's startup is skipped. Stored separately from SettingsStruct, read at boot.
// FAST: connect to the robot while the intro plays. BOOST: + no intro animation. OVERKLOKING: + no pairing
// animation (static CONNECTING screen, theme assets load while connecting) and a continuous BLE scan.
enum class FastStart : uint8_t {
	Off = 0, Fast = 1, Boost = 2, Overkloking = 3
};

// Custom (NUIT): TALKIE TOASTER / YODA also change which lines the controller shows (same as the robot)
inline void applyVoiceModeToPhrases(VoiceMode mode){
	Phrases::toasterMode = mode == VoiceMode::Toaster;
	Phrases::yodaMode = mode == VoiceMode::Yoda;
}

struct SettingsStruct {
	float screenBrightness = 1.0f;
	Theme currentTheme = Theme::Main;
	InactivityTimeout inactivityTimeout = InactivityTimeout::Min10;
	FrontSensorMode frontSensor = FrontSensorMode::On;
	BottomSensorMode bottomSensor = BottomSensorMode::Reduced;
	ActivityMode activityMode = ActivityMode::Active;
};

class Settings : public Object {
	GENERATED_BODY(Settings, Object, void)
public:
	Settings();
	~Settings() override;

	SettingsStruct get() const;
	void set(const SettingsStruct& settings);
	void store() const;

	VoiceMode getVoiceMode() const;
	void setVoiceMode(VoiceMode mode);

	FastStart getFastStart() const;
	void setFastStart(FastStart level);

	// Custom (NUIT): robot volume, night mode and roaming (sent to the robot with Com::setRobotConfig)
	RobotConfigData getRobotConfig() const;
	void setRobotConfig(const RobotConfigData& config);

private:
	SettingsStruct settingsStruct;
	VoiceMode voiceMode = VoiceMode::Normal;
	static constexpr const char* VoiceKey = "Voice";
	FastStart fastStart = FastStart::Off;
	static constexpr const char* FastStartKey = "FastStart";
	RobotConfigData robotConfig{ 80, 0, 40, 1 };
	static constexpr const char* VolumeKey = "Volume";
	static constexpr const char* NightModeKey = "NightMode";
	static constexpr const char* NightVolumeKey = "NightVol";
	static constexpr const char* RoamingKey = "Roaming";

	static constexpr const char* BlobName = "Settings";
	static constexpr const char* NVSNamespace = "ButterbotCtrl";

	nvs_handle_t handle{};

	void load();
};

#endif //BUTTERBOTCTRL_FIRMWARE_SETTINGS_H
