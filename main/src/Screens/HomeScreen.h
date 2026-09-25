#ifndef BUTTERBOTCTRL_FIRMWARE_HOMESCREEN_H
#define BUTTERBOTCTRL_FIRMWARE_HOMESCREEN_H

#include <queue>
#include <BBData.h>
#include <LV_Interface/LVScreen.h>
#include <Services/ButtonInput.h>

#include "Enums.hpp"
#include "Services/Com.h"
#include "Services/LEDController.h"
#include "Services/RobotState.h"
#include "Components/TopBar.h"
#include "Components/HomeWindow.h"
#include "Components/GuideElement.h"
#include "Components/ActionElement.h"

class HomeScreen : public LVScreen {
public:
	HomeScreen();
	~HomeScreen() override;

private:
	struct AnimUserData {
		bool* animActive;
	};

	struct ComState {
		BB::State state;
		BB::Action action;
	};

	struct PendingEvent {
		BB::State state;
		BB::Action action;
		std::vector<uint8_t> data;
	};

	ThemeService* theme;
	TopBar* topBar;
	ButtonInput* buttonInput;
	LEDController* ledController;
	Com* com;
	RobotState* robotState;
	ComState comState = {
		.state = BB::State::Idle,
		.action = BB::Action{ BB::Action::Idle::None },
	};
	// Phase of the last received Listen packet, for detecting phase changes
	ListenData::Phase lastListenPhase = ListenData::Phase::Listening;

	static constexpr int32_t WindowGap = 6;
	lv_obj_t* windowContainer;
	lv_obj_t* scrollOffset;
	HomeWindow* activeWindow = nullptr;
	GuideElement* guide = nullptr;

	lv_anim_t windowFadeAnim;
	AnimUserData currentAnimData;
	bool animActive = false;

	static constexpr uint64_t MinWindowDisplayMs = 1000;
	static constexpr uint64_t ClearGracePeriodMs = 200;
	static constexpr uint64_t UninterruptedDisplayMs = 3000;
	static constexpr size_t MaxQueuedWindows = 16;
	std::queue<PendingEvent> windowQueue;
	uint64_t activeWindowShownAt = 0;
	// Refreshed on every data update of the active window; the uninterrupted fade-out timing counts from here
	uint64_t activeWindowUpdatedAt = 0;
	bool clearPending = false;
	uint64_t clearPendingAt = 0;
	bool summonHoldEnabled = false;

	static constexpr uint64_t SummonShortPressMaxMs = 250;
	static constexpr uint64_t SummonLongPressMinMs = 2000;
	uint64_t summonPressTime = 0;
	// Listen commands sent via summon short press are capped until the ListenWindow is removed
	static constexpr uint8_t MaxListenSends = 2;
	uint8_t listenSendCount = 0;
	bool activeWindowIsListen = false;
	static constexpr uint64_t OverrideHoldMinMs = 2000;
	static constexpr uint64_t OverrideShortPressMaxMs = 300;
	static constexpr uint64_t OverrideHintDurationMs = 3000;
	uint64_t overridePressTime = 0;
	// Custom (NUIT): short Poke = stock poke, hold 1 s = 10 cm forward + "nju aj ti OVERKLOKING is the best"
	static constexpr uint64_t PokeShortPressMaxMs = 300;
	static constexpr uint64_t PokeLongPressMinMs = 1000;
	uint64_t pokePressTime = 0;
	uint64_t overrideHintShownAt = 0;

	// "Loading RC..." indicator
	static constexpr const char* RCLoadText = "Loading RC...";
	static constexpr uint64_t RCLoadTimeoutMs = 2000;
	uint64_t rcWaitStartTime = 0;
	lv_obj_t* rcLoadingLabel = nullptr;

	// Centered pop-up hovering over everything, driven by a held button. Not part of the
	// windowContainer window system; has no come-in/come-out animation.
	HomeWindow* popup = nullptr;
	Button popupOwner = Button::COUNT;

	// Full-screen modal action list opened by joystick press; while open, the joystick is
	// routed through the LVGL keypad indev and all other button functions are suppressed
	ActionElement* actionElement = nullptr;
	// Button whose press dismissed the list; its release is swallowed too (Poke acts on release)
	Button actionDismissBtn = Button::COUNT;
	// A scenario selection closes the list, deferred to loop() because the callback fires
	// mid-event-dispatch (deleting the list there would free the button being clicked)
	bool actionsClosePending = false;
	// Focused entry when the list was last closed
	uint16_t lastActionIndex = 0;

	void loop() override;

	void handleButtonEvent(Button btn, ButtonInput::Action action);
	void handleDataEvent(BB::State state, BB::Action action, const std::vector<uint8_t>& data);
	void ledDataBlink(const std::vector<uint8_t>& data);
	void startRC();

	HomeWindow* createActionWindow(BB::State state, BB::Action action, const std::vector<uint8_t>& data);

	/**
	 * Insert a new window to home screen with animation
	 * @param window new action window
	 * @param isListen whether the window is a ListenWindow
	 */
	void insertWindow(HomeWindow* window, bool isListen);

	/** Fade out and delete the current active window */
	void clearWindow();

	/**
	 * Show a centered pop-up hovering over everything. Any existing pop-up is interrupted
	 * (removed immediately) and replaced by this one.
	 * @param window pop-up window (parented to this screen)
	 * @param owner button that owns the pop-up; only that button's release clears it
	 */
	void showPopup(HomeWindow* window, Button owner);

	/** Immediately delete the current pop-up, if any */
	void clearPopup();

	/** Clear the current pop-up only if it is owned by the given button */
	void clearPopupFor(Button owner);

	/** Show the modal action list, cancelling any in-flight button holds */
	void showActions();

	/** Delete the action list */
	void clearActions();

	/** Drain the pending event queue, respecting animation and minimum display time */
	void processQueue();

	void buildUI();
	void updateAnim();
};

#endif //BUTTERBOTCTRL_FIRMWARE_HOMESCREEN_H
