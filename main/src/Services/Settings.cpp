#include "Settings.h"
#include <nvs_flash.h>

DEFINE_LOG(Settings)

Settings::Settings(){
	ESP_ERROR_CHECK(nvs_open(NVSNamespace, NVS_READWRITE, &handle));
	load();
}

Settings::~Settings(){
	nvs_close(handle);
}

SettingsStruct Settings::get() const{
	return settingsStruct;
}

void Settings::set(const SettingsStruct& settings){
	settingsStruct = settings;
}

void Settings::store() const{
	esp_err_t err = nvs_set_blob(handle, BlobName, &settingsStruct, sizeof(SettingsStruct));
	if(err != ESP_OK){
		CMF_LOG(Settings, LogLevel::Error, "Error storing settings: %s", esp_err_to_name(err));
		return;
	}
	err = nvs_set_u8(handle, SensorKey, static_cast<uint8_t>(sensorMode));
	if(err != ESP_OK){
		CMF_LOG(Settings, LogLevel::Error, "Error storing sensor mode: %s", esp_err_to_name(err));
	}
	err = nvs_set_u8(handle, VoiceKey, static_cast<uint8_t>(voiceMode));
	if(err != ESP_OK){
		CMF_LOG(Settings, LogLevel::Error, "Error storing voice mode: %s", esp_err_to_name(err));
	}
	err = nvs_set_u8(handle, FastStartKey, static_cast<uint8_t>(fastStart));
	if(err != ESP_OK){
		CMF_LOG(Settings, LogLevel::Error, "Error storing fast start: %s", esp_err_to_name(err));
	}
	nvs_set_u8(handle, VolumeKey, robotConfig.volume);
	nvs_set_u8(handle, NightModeKey, robotConfig.nightMode);
	nvs_set_u8(handle, NightVolumeKey, robotConfig.nightVolume);
	nvs_set_u8(handle, RoamingKey, robotConfig.roaming);
	nvs_commit(handle);
}

SensorMode Settings::getSensorMode() const{
	return sensorMode;
}

void Settings::setSensorMode(SensorMode mode){
	sensorMode = mode;
}

VoiceMode Settings::getVoiceMode() const{
	return voiceMode;
}

void Settings::setVoiceMode(VoiceMode mode){
	voiceMode = mode;
	applyVoiceModeToPhrases(mode);
}

FastStart Settings::getFastStart() const{
	return fastStart;
}

void Settings::setFastStart(FastStart level){
	fastStart = level;
}

RobotConfigData Settings::getRobotConfig() const{
	return robotConfig;
}

void Settings::setRobotConfig(const RobotConfigData& config){
	robotConfig = config;
}

void Settings::load(){
	size_t len = sizeof(SettingsStruct);
	esp_err_t err = nvs_get_blob(handle, BlobName, &settingsStruct, &len);
	if(err != ESP_OK){
		CMF_LOG(Settings, LogLevel::Warning, "No stored settings found, using defaults: %s", esp_err_to_name(err));
		settingsStruct = SettingsStruct();
	}

	uint8_t sensorVal = 0;
	if(nvs_get_u8(handle, SensorKey, &sensorVal) == ESP_OK && sensorVal <= static_cast<uint8_t>(SensorMode::AllOff)){
		sensorMode = static_cast<SensorMode>(sensorVal);
	}else{
		sensorMode = SensorMode::AllOn;
	}

	uint8_t voiceVal = 0;
	if(nvs_get_u8(handle, VoiceKey, &voiceVal) == ESP_OK && voiceVal <= static_cast<uint8_t>(VoiceMode::Yoda)){
		voiceMode = static_cast<VoiceMode>(voiceVal);
	}else{
		voiceMode = VoiceMode::Normal;
	}
	applyVoiceModeToPhrases(voiceMode);

	uint8_t fastVal = 0;
	if(nvs_get_u8(handle, FastStartKey, &fastVal) == ESP_OK && fastVal <= static_cast<uint8_t>(FastStart::Overkloking)){
		fastStart = static_cast<FastStart>(fastVal);
	}else{
		fastStart = FastStart::Off;
	}

	uint8_t val = 0;
	if(nvs_get_u8(handle, VolumeKey, &val) == ESP_OK && val >= 10 && val <= 100) robotConfig.volume = val;
	if(nvs_get_u8(handle, NightModeKey, &val) == ESP_OK && val <= static_cast<uint8_t>(NightMode::From00)) robotConfig.nightMode = val;
	if(nvs_get_u8(handle, NightVolumeKey, &val) == ESP_OK && val >= 10 && val <= 100) robotConfig.nightVolume = val;
	if(nvs_get_u8(handle, RoamingKey, &val) == ESP_OK && val <= 1) robotConfig.roaming = val;

	switch(settingsStruct.inactivityTimeout){
		case InactivityTimeout::Min2:
		case InactivityTimeout::Min5:
		case InactivityTimeout::Min10:
		case InactivityTimeout::Min30:
		case InactivityTimeout::Off:
			break;
		default:
			settingsStruct.inactivityTimeout = SettingsStruct().inactivityTimeout;
			break;
	}
}
