#ifndef BUTTERBOTCTRL_FIRMWARE_ROBOTSTATE_H
#define BUTTERBOTCTRL_FIRMWARE_ROBOTSTATE_H

#include <atomic>
#include <cstdint>
#include <Core/Application.h>
#include <BBData.h>

/**
 * @brief Holds state of the currently connected ButterBot, shared across the application.
 */
class RobotState : public Object {
	GENERATED_BODY(RobotState, Object, void)

public:
	uint8_t getBotBatteryLevel() const{ return botBatteryLevel.load(); }
	void setBotBatteryLevel(uint8_t level){ botBatteryLevel.store(level); }

	ChargeStatus getChargeState() const{ return chargeState.load(); }
	void setChargeState(ChargeStatus state){ chargeState.store(state); }

	bool isIdleNone() const{ return idleNone.load(); }
	void setIdleNone(bool value){ idleNone.store(value); }

	bool isMuted() const{ return muted.load(); }
	void setMuted(bool value){ muted.store(value); }

private:
	std::atomic<uint8_t> botBatteryLevel{ 0 };
	std::atomic<ChargeStatus> chargeState{ ChargeStatus::Unplugged };
	std::atomic<bool> idleNone{ true };
	std::atomic<bool> muted{ false };
};

#endif //BUTTERBOTCTRL_FIRMWARE_ROBOTSTATE_H
