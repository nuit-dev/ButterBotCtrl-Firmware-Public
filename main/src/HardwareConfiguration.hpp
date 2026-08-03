#ifndef BB_CTRL_HARDWARECONFIGURATION_H
#define BB_CTRL_HARDWARECONFIGURATION_H

#include <lgfx/v1/panel/Panel_Device.hpp>
#include <lgfx/v1/platforms/esp32/Bus_SPI.hpp>

#include <Misc/Singleton.h>
#include <Drivers/Input/InputGPIO.h>
#include <Drivers/Output/OutputPWM.h>

#include "Pins.hpp"
#include "Enums.hpp"

class HardwareConfiguration : public Singleton {
	GENERATED_BODY(HardwareConfiguration, Singleton, void)

public:
	const std::vector<GPIOPinDef>& getGPIOInputs() const noexcept{
		return GPIOInputs;
	}

	const std::vector<OutputPinDef>& getGPIOOutputs() const noexcept{
		return GPIOOutputs;
	}

	const lgfx::Bus_SPI::config_t& getDisplayBusConfig() const noexcept{
		return DisplayBusConfig;
	}

	const lgfx::Panel_Device::config_t& getDisplayPanelConfig() const noexcept{
		return DisplayPanelConfig;
	}

	const std::vector<OutputPWMPinDef>& getPwmOutputs() const{
		return PWMOutputs;
	}

private:
	const std::vector<GPIOPinDef> GPIOInputs = {
		{ { BTN_JOY, true }, PullMode::Up },
		{ { BTN_MANOVER, true }, PullMode::Up },
		{ { BTN_POKE, true }, PullMode::Up },
		{ { BTN_SHUTUP, true }, PullMode::Up },
		{ { BTN_SUMMON, true }, PullMode::Up },
	};

	const std::vector<OutputPinDef> GPIOOutputs = {
		{ PIN_VREF, false },
	};

	const std::vector<OutputPWMPinDef> PWMOutputs = {
		{ { (int)LEDs::BigGreen, false }, (gpio_num_t)LED_BIG },
		{ { (int)LEDs::Wifi, false }, (gpio_num_t)LED_WIFI },
		{ { (int)LEDs::Power, false }, (gpio_num_t)LED_POWER },
		{ { (int)LEDs::BatteryLow, false }, (gpio_num_t)LED_BATTLOW },
		{ { (int)LEDs::TftBacklight, true }, (gpio_num_t)TFT_BL },
	};

	const lgfx::Bus_SPI::config_t DisplayBusConfig = {
		.freq_write = 40000000,
		.freq_read = 40000000,
		.pin_sclk = TFT_SCK,
		.pin_miso = -1,
		.pin_mosi = TFT_SDA,
		.pin_dc = TFT_DC,
		.spi_mode = 0,
		.spi_3wire = false,
		.use_lock = false,
		.dma_channel = LGFX_ESP32_SPI_DMA_CH,
		.spi_host = SPI2_HOST
	};

	const lgfx::Panel_Device::config_t DisplayPanelConfig = {
		.pin_cs = -1,
		.pin_rst = TFT_RST,
		.pin_busy = -1,
		.memory_width = 128,
		.memory_height = 128,
		.panel_width = 128,
		.panel_height = 128,
		.offset_x = 0,
		.offset_y = 0,
		.offset_rotation = 0,
		.readable = false,
		.invert = false,
		.rgb_order = false,
		.dlen_16bit = false,
		.bus_shared = false
	};
};

#endif //BB_CTRL_HARDWARECONFIGURATION_H
