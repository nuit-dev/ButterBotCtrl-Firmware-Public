#include "WiFiAccessPoint.h"
#include <Periphery/WiFi.h>

WiFiAccessPoint::WiFiAccessPoint(WiFi* wifi) : wifi(wifi){
	static constexpr const char* Charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	static constexpr size_t MaxIndex = sizeof(Charset) - 1; // -1 for '\0', another -1 to get index

	for(size_t i = 0; i < sizeof(SSID) - 1; ++i){
		SSID[i] = Charset[rand() % MaxIndex];
	}

	for(size_t i = 0; i < sizeof(password) - 1; ++i){
		password[i] = Charset[rand() % MaxIndex];
	}

	SSID[sizeof(SSID) - 1] = '\0';
	password[sizeof(password) - 1] = '\0';

    wifi->startAccessPoint(SSID, password);
    wifi->setHidden(true);

    wifi->OnAccessPointConnection.bind(this, &WiFiAccessPoint::onConnected);
    wifi->OnAccessPointDisconnection.bind(this, &WiFiAccessPoint::onDisconnected);
}

void WiFiAccessPoint::onConnected(/*const std::string& mac,*/ uint8_t aid, bool isMeshChild){
	connectionStatus = EventType::Connect;
    OnConnectionEvent.broadcast(/*mac,*/ EventType::Connect);
}

void WiFiAccessPoint::onDisconnected(/*const std::string& mac, */uint8_t aid, bool isMeshChild, uint8_t reason){
	connectionStatus = EventType::Disconnect;
    OnConnectionEvent.broadcast(/*mac,*/ EventType::Disconnect);
}
