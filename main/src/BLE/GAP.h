#ifndef CLOCKSTAR_FIRMWARE_GAP_H
#define CLOCKSTAR_FIRMWARE_GAP_H

#include <Misc/Singleton.h>
#include <Event/EventBroadcaster.h>
#define BLE_42_FEATURE_SUPPORT TRUE
#include <esp_gap_ble_api.h>
#include <unordered_set>
#include <esp_gattc_api.h>

namespace BLE {

class Client;

class GAP : public Singleton {
	GENERATED_BODY(GAP, Singleton, void);
public:
	GAP();
	virtual ~GAP();

	bool isConnected();
	bool isConnecting();
	void connect();

	// Custom (NUIT): FAST START OVERKLOKING - scan all the time (window = interval) instead of 30 of every 50 ms
	void setContinuousScan(bool continuous);

	enum class ConnEvent { Connected, Failed };
	DECLARE_EVENT(OnConnEvent, GAP, ConnEvent);
	OnConnEvent onConnEvent{ this };

private:
	static GAP* self;

	void ble_GAP_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param);

	void initSecure();

	friend Client;
	Client* client = nullptr;
	void setClient(Client* client);

	void scanResult(const esp_ble_gap_cb_param_t& res);
	void scanDone();

	struct {
		esp_bd_addr_t addr;
		esp_ble_addr_type_t type;
		bool found = false;
	} result;

	enum {
		Idle, Scanning, Stopping, Connecting, Connected
	} state = Idle;

	static constexpr const char* Name = "CircuitMess Butter Bot";

	bool continuousScan = false;

	// Custom (NUIT): FAST START connects right after boot - wait until local privacy (RPA) is set up, else the scan
	// can fail to start. Falls back to connecting anyway after PrivacyWaitMaxUs.
	bool privacyReady = false;
	bool connectPending = false;
	int64_t createdAtUs = 0;
	static constexpr int64_t PrivacyWaitMaxUs = 2000000;

};

}


#endif //CLOCKSTAR_FIRMWARE_GAP_H
