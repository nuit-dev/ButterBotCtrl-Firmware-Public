#ifndef BUTTERBOTCTRL_FIRMWARE_COM_H
#define BUTTERBOTCTRL_FIRMWARE_COM_H

#include "BLE/Client.h"
#include <BBData.h>
#include <CtrlData.h>
#include <Scenarios.h>
#include <Entity/AsyncEntity.h>
#include <Event/EventBroadcaster.h>
#include <atomic>
#include <cstring>
#include <vector>

class Com : public AsyncEntity {
	GENERATED_BODY(Com, AsyncEntity, CONSTRUCTOR_PACK(BLE::Client * ))
public:
	Com(BLE::Client* client, bool internalStack = true);

	enum class ConnStatus { Connected, Disconnected };
	DECLARE_EVENT(OnConnStatus, Com, ConnStatus);
	OnConnStatus onConnStatus{ this };

	ConnStatus getStatus() const;

	// TODO: template struct (extending ActionData) instead of std::vector<uint8_t>
	DECLARE_EVENT(OnData, Com, BB::State, BB::Action, std::vector<uint8_t>);
	OnData onData{ this };

	void sendCommand(Ctrl::Command command);
	void sendDrive(DriveData data);
	void sendRC(RCData data);
	void sendScenario(BB::Action::Scenario scenario, ScenarioData data);

	// Custom (NUIT): proximity sensor filter; sent now if connected and again on every (re)connect.
	// Call from the UI thread.
	void setSensorCommand(Ctrl::Command command);
	// Custom (NUIT): robot TTS voice preset, same behaviour as setSensorCommand
	void setVoiceCommand(Ctrl::Command command);
	// Custom (NUIT): robot volume / night mode, same behaviour as setSensorCommand
	void setRobotConfig(const RobotConfigData& config);
	// Custom (NUIT): sets the robot's clock (only if connected). Call from the UI thread.
	void sendSetTime(const SetTimeData& data);

protected:
	void tick(float deltaTime) noexcept override;

private:
	// Service UUID
	static constexpr esp_bt_uuid_t ServiceUID = {
			.len = ESP_UUID_LEN_128,
			.uuid = { .uuid128 = { 0x19, 0x08, 0x79, 0xbc, 0x80, 0xbd, 0x40, 0x44, 0x91, 0x98, 0x51, 0x82, 0xe7, 0xca, 0x85, 0x36 }}
	};

	// Server-side RX and TX
	static constexpr esp_bt_uuid_t RxCharUID = {
			.len = ESP_UUID_LEN_128,
			.uuid = { .uuid128 = { 0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x02, 0x00, 0x40, 0x6E }}
	};
	static constexpr esp_bt_uuid_t TxCharUID = {
			.len = ESP_UUID_LEN_128,
			.uuid = { .uuid128 = { 0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x03, 0x00, 0x40, 0x6E }}
	};

	std::shared_ptr<BLE::Client::Service> service;
	std::shared_ptr<BLE::Client::Char> rxChar;
	std::shared_ptr<BLE::Client::Char> txChar;

	ConnStatus status = ConnStatus::Disconnected;

	static constexpr size_t BufSize = 16;
	std::vector<uint8_t> txBuf;

	std::atomic<Ctrl::Command> sensorCommand{ Ctrl::SensorsAllOn };
	std::atomic<Ctrl::Command> voiceCommand{ Ctrl::VoiceNormal };
	std::atomic<bool> sensorSyncPending{ false }; // sensor + voice + robot config
	std::atomic<uint32_t> robotConfigPacked{ 0 }; // RobotConfigData, 0 = not set yet

	template<typename T>
	static std::vector<uint8_t> packet(Ctrl::Command command, const T& data){
		std::vector<uint8_t> buf(sizeof(Ctrl::Command) + sizeof(T));
		memcpy(buf.data(), &command, sizeof(Ctrl::Command));
		memcpy(buf.data() + sizeof(Ctrl::Command), &data, sizeof(T));
		return buf;
	}
	static uint32_t pack(const RobotConfigData& c){ return 0x1000000u | (c.volume << 16) | (c.nightMode << 8) | c.nightVolume; }
	static RobotConfigData unpack(uint32_t p){ return { (uint8_t)(p >> 16), (uint8_t)(p >> 8), (uint8_t)p }; }

};


#endif //BUTTERBOTCTRL_FIRMWARE_COM_H
