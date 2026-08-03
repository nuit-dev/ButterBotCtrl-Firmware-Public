#ifndef BB_CTRL_FIRMWARE_SHUTDOWNSERVICE_H
#define BB_CTRL_FIRMWARE_SHUTDOWNSERVICE_H

#include "Object/Object.h"
#include <esp_sleep.h>
#include "Core/Application.h"
#include "Services/ButtonInput.h"
#include "Services/Settings.h"

enum class ShutdownReason : uint8_t {
	Inactivity, Battery
};

class ShutdownService : public AsyncEntity {
	GENERATED_BODY(ShutdownService, AsyncEntity, void)
public:
	ShutdownService(bool internalStack = true);

	//Hardware shutdown, with notification audio beforehand
	static void shutdown(ShutdownReason reason);

	//To prevent multiple simultaneous shutdown calls
	static SemaphoreHandle_t shutdownSem;

private:
	void tick(float deltaTime) noexcept override;

	static constexpr uint32_t CheckInterval = 60000; //1 min

	Settings* settings;

	float inactivityTime = 0.0f; //seconds

	SemaphoreHandle_t inactivitySem;

	void inputEvent(Enum<int> btn, ButtonInput::Action action);
};


#endif //BB_CTRL_FIRMWARE_SHUTDOWNSERVICE_H