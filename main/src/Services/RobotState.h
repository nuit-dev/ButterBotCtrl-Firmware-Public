#ifndef BUTTERBOTCTRL_FIRMWARE_ROBOTSTATE_H
#define BUTTERBOTCTRL_FIRMWARE_ROBOTSTATE_H

#include <atomic>
#include <cstdint>
#include <Core/Application.h>
#include <BBData.h>
#include <ctime>
#include <esp_timer.h>

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

	// Custom (NUIT): the robot's clock (TimeInfoData), kept running here between updates
	void setRobotTime(const TimeInfoData& info){
		tm t = {};
		t.tm_year = info.year - 1900;
		t.tm_mon = info.month - 1;
		t.tm_mday = info.day;
		t.tm_hour = info.hour;
		t.tm_min = info.minute;
		t.tm_sec = info.second;
		robotEpoch.store((int64_t)mktime(&t)); // no TZ set, so this is the robot's local time as-is
		robotEpochAtUs.store(esp_timer_get_time());
		robotTimeKnown.store(info.configured);
	}

	bool getRobotTime(tm& out) const{
		if(!robotTimeKnown.load()) return false;
		const time_t now = (time_t)(robotEpoch.load() + (esp_timer_get_time() - robotEpochAtUs.load()) / 1000000);
		gmtime_r(&now, &out);
		return true;
	}

private:
	std::atomic<uint8_t> botBatteryLevel{ 0 };
	std::atomic<ChargeStatus> chargeState{ ChargeStatus::Unplugged };
	std::atomic<bool> idleNone{ true };
	std::atomic<bool> muted{ false };
	std::atomic<int64_t> robotEpoch{ 0 };
	std::atomic<int64_t> robotEpochAtUs{ 0 };
	std::atomic<bool> robotTimeKnown{ false };
};

#endif //BUTTERBOTCTRL_FIRMWARE_ROBOTSTATE_H
