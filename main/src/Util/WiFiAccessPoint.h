#ifndef BUTTERBOT_CTRL_WIFIACCESSPOINT_H
#define BUTTERBOT_CTRL_WIFIACCESSPOINT_H

#include <CtrlData.h>
#include <Event/EventBroadcaster.h>
#include <Object/Object.h>

class WiFiAccessPoint : public Object {
    GENERATED_BODY(WiFiAccessPoint, Object, CONSTRUCTOR_PACK(class WiFi*))

public:
    enum class EventType : uint8_t {
        Connect,
        Disconnect
    };

    DECLARE_EVENT(ConnectionEvent, WiFiAccessPoint/*, std::string*/, EventType);
    ConnectionEvent OnConnectionEvent{this};

public:
    WiFiAccessPoint(WiFi* wifi);

	inline const char* getSSID() const { return SSID; }
	inline const char* getPassword() const { return password; }

	inline EventType getConnectionStatus() const { return connectionStatus; }

private:
	StrongObjectPtr<WiFi> wifi;
	char SSID[sizeof(RCData::SSID)];
	char password[sizeof(RCData::password)];
	EventType connectionStatus = EventType::Disconnect;

private:
    void onConnected(/*const std::string& mac,*/ uint8_t aid, bool isMeshChild);
    void onDisconnected(/*const std::string& mac,*/ uint8_t aid, bool isMeshChild, uint8_t reason);
};

#endif //BUTTERBOT_CTRL_WIFIACCESSPOINT_H