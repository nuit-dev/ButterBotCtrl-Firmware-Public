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

};

}


#endif //CLOCKSTAR_FIRMWARE_GAP_H
