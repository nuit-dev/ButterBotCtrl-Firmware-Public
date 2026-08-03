#ifndef BUTTERBOTCTRL_FIRMWARE_FEED_H
#define BUTTERBOTCTRL_FIRMWARE_FEED_H

#include <Entity/AsyncEntity.h>
#include "UDPListener.h"
#include "Util/RingBuffer.h"
#include <FeedFrame.h>
#include "esp_jpeg_dec.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <atomic>

class Feed : public Object {
	GENERATED_BODY(Feed, Object, void)

	typedef uint16_t Color;
public:
	Feed();
	~Feed() override;

	bool nextFrame(std::function<void(const Color* img)>);

private:
	jpeg_dec_handle_t jpegDec = nullptr;
	jpeg_dec_io_t jpegIo{};
	jpeg_dec_header_info_t jpegHeader{};

	StrongObjectPtr<UDPListener> udp;

	StrongObjectPtr<RingBuffer> rxBuf;
	std::mutex rxMut;

	SemaphoreHandle_t dataAvailable = nullptr;

	std::unique_ptr<Threaded> readTask;
	std::vector<uint8_t> readBuf;
	static constexpr size_t ReadBufSize = 4096;
	void readLoop();

	std::unique_ptr<Threaded> decodeTask;
	void decodeLoop();
	bool findFrame() const;

	bool freeImgs[3] = { true, true, true };
	Color* frameImgs[3] = {};

	std::mutex readyFrameMut;
	int readyFrame = -1;

	constexpr static int FrameWidth = 128;
	constexpr static int FrameHeight = 128;
	constexpr static size_t FrameBufSize = (size_t) FrameWidth * FrameHeight * 2; //decoded RGB565 frame size in bytes

	constexpr static size_t JpgMaxSize = 8000; //upper size limit for JPG quality 30 on 128x128 resolution
	constexpr static size_t RxBufSize = 3 * (sizeof(FeedFrame) + JpgMaxSize);

	static std::unique_ptr<FeedFrame> deserializeFrame(RingBuffer& buf, size_t size);
};

#endif //BUTTERBOTCTRL_FIRMWARE_FEED_H
