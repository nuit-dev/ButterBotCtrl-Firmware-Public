#include "Feed.h"
#include "Util/stdafx.h"
#include "Memory/ObjectMemory.h"
#include <esp_log.h>
#include <esp_heap_caps.h>
#include <mutex>
#include <utility>
#include <Core/Application.h>

DEFINE_LOG(Feed);

Feed::Feed(){
	dataAvailable = xSemaphoreCreateBinary();

	udp = newObject<UDPListener>(this);
	rxBuf = newObject<RingBuffer>(this, RxBufSize);

	readBuf.resize(ReadBufSize);
	for(auto& frameImg: frameImgs){
		//esp_new_jpeg requires a 16-byte aligned output buffer; keep the frame buffers in PSRAM
		frameImg = (Color*) heap_caps_aligned_alloc(16, FrameBufSize, MALLOC_CAP_SPIRAM);
	}

	jpeg_dec_config_t config = DEFAULT_JPEG_DEC_CONFIG();
	config.output_type = JPEG_PIXEL_FORMAT_RGB565_LE;
	config.rotate = JPEG_ROTATE_0D;

	jpeg_error_t err = jpeg_dec_open(&config, &jpegDec);
	if(err != JPEG_ERR_OK){
		CMF_LOG(Feed, Error, "jpeg_dec_open failed: %d", err);
	}

	readTask = std::make_unique<Threaded>([this](){ readLoop(); }, "FeedRead", 5, 4096, 8, 0, false);

	decodeTask = std::make_unique<Threaded>([this](){ decodeLoop(); }, "FeedDecode", 5, 4096, 8, 0, false);

	readTask->start();
	decodeTask->start();
}

Feed::~Feed(){
	readTask->stop();
	decodeTask->stop(0);
	xSemaphoreGive(dataAvailable);
	while(decodeTask->running()){
		delayMillis(1);
	}

	if(jpegDec != nullptr){
		jpeg_dec_close(jpegDec);
	}
	for(auto& frameImg: frameImgs){
		if(frameImg != nullptr){
			heap_caps_free(frameImg);
		}
	}

	delete *udp;
	delete *rxBuf;

	vSemaphoreDelete(dataAvailable);
}

bool Feed::nextFrame(std::function<void(const Color* img)> cb){
	std::unique_lock lock(readyFrameMut);
	if(readyFrame == -1) return false;

	volatile int frameIndex = readyFrame;
	if(frameIndex == -1) return false;

	volatile const Color* img = frameImgs[frameIndex];

	readyFrame = -1;
	lock.unlock();

	cb((const Color*) img);

	lock.lock();
	freeImgs[frameIndex] = true;

	return true;
}

void Feed::readLoop(){
	if(udp == nullptr){
		return;
	}

	const int64_t bytes = udp->read(readBuf.data(), readBuf.size());

	if(bytes <= 0){
		return;
	}

	std::lock_guard lock(rxMut);
	rxBuf->write(readBuf.data(), bytes);
	xSemaphoreGive(dataAvailable);
}

void Feed::decodeLoop(){
	if(xSemaphoreTake(dataAvailable, portMAX_DELAY) != pdTRUE){
		return;
	}

	std::unique_lock lock(rxMut);
	if(!findFrame()) {
		return;
	}

	rxBuf->skip(sizeof(FeedFrame::Header));
	size_t size;
	rxBuf->read(reinterpret_cast<uint8_t*>(&size), sizeof(size_t));
	rxBuf->skip(sizeof(size_t));

	size_t available = rxBuf->readAvailable();
	auto frame = deserializeFrame(*rxBuf.get(), size);

	size_t readTotal = available - rxBuf->readAvailable();
	rxBuf->skip(size - readTotal); // skip frame if deserialize exited early

	rxBuf->skip(sizeof(FeedFrame::Trailer));

	const bool moreData = rxBuf->readAvailable() >= sizeof(FeedFrame::Header) + sizeof(size_t) * 2;

	lock.unlock();

	if(moreData){
		xSemaphoreGive(dataAvailable);
	}

	if(!frame || frame->size == 0 || frame->data == nullptr){
		return;
	}

	int freeImg = -1;
	{
		std::lock_guard frameLock(readyFrameMut);
		for(int i = 0; i < 3; i++){
			if(freeImgs[i]){
				freeImg = i;
				break;
			}
		}

		if(freeImg == -1){
			return;
		}

		freeImgs[freeImg] = false;
	}

	jpegIo.inbuf = (uint8_t*) frame->data;
	jpegIo.inbuf_len = (int) frame->size;

	jpeg_error_t err = jpeg_dec_parse_header(jpegDec, &jpegIo, &jpegHeader);
	if(err != JPEG_ERR_OK){
		CMF_LOG(Feed, Error, "header parse error: %d", err);
		std::lock_guard frameLock(readyFrameMut);
		freeImgs[freeImg] = true;
		return;
	}

	if((size_t) jpegHeader.width * jpegHeader.height * 2 > FrameBufSize){
		CMF_LOG(Feed, Error, "frame too large: %dx%d", jpegHeader.width, jpegHeader.height);
		std::lock_guard frameLock(readyFrameMut);
		freeImgs[freeImg] = true;
		return;
	}

	jpegIo.outbuf = (uint8_t*) frameImgs[freeImg];

	err = jpeg_dec_process(jpegDec, &jpegIo);
	if(err != JPEG_ERR_OK){
		CMF_LOG(Feed, Error, "decode error: %d", err);
		std::lock_guard frameLock(readyFrameMut);
		freeImgs[freeImg] = true;
		return;
	}

	std::lock_guard frameLock(readyFrameMut);
	if(readyFrame != -1){
		freeImgs[readyFrame] = true;
	}
	readyFrame = freeImg;
}

bool Feed::findFrame() const{
	if(rxBuf->readAvailable() < sizeof(FeedFrame::Header)) return false;

	// Search for frame header
	size_t bytesRead = 0;
	size_t bytesMatched = 0;
	for(; bytesRead < rxBuf->readAvailable(); bytesRead++){
		uint8_t byte = *rxBuf->peek<uint8_t>(bytesRead);

		if(byte == FeedFrame::Header[bytesMatched]){
			bytesMatched++;
			if(bytesMatched == sizeof(FeedFrame::Header)){
				bytesRead++;
				break;
			}
		}else{
			bytesMatched = 0;
		}
	}

	// Clear buffer if header isn't found
	if(bytesMatched != sizeof(FeedFrame::Header)){
		size_t size = rxBuf->readAvailable();
		rxBuf->clear();
		CMF_LOG(Feed, Debug, "Couldn't find frame header. Skipping %zu bytes", size);
		return false;
	}

	// Skip bytes preceding header
	if(bytesRead - bytesMatched != 0){
		rxBuf->skip(bytesRead - bytesMatched);
		CMF_LOG(Feed, Debug, "Found header. Skipping %zu bytes", bytesRead - bytesMatched);
	}

	// Needs at least 8 more bytes to continue (frame size + frame size confirmation)
	if(rxBuf->readAvailable() < sizeof(FeedFrame::Header) + sizeof(size_t) * 2){
		return false;
	}

	// Read frame size
	uint8_t frameSizeRaw[4];
	for(int i = 0; i < 4; i++){
		frameSizeRaw[i] = *rxBuf->peek<uint8_t>(sizeof(FeedFrame::Header) + i);
	}
	size_t frameSize = *((size_t*) frameSizeRaw);


	// Clear buffer if frame is malformed
	if(frameSize >= RxBufSize){
		CMF_LOG(Feed, Debug, "Frame size huge: %zu", frameSize);
		rxBuf->clear();
		return false;
	}

	// Read shifted frame size
	uint8_t frameShiftedSizeRaw[4];
	for(int i = 0; i < 4; i++){
		frameShiftedSizeRaw[i] = *rxBuf->peek<uint8_t>(sizeof(FeedFrame::Header) + sizeof(size_t) + i);
	}

	// Clear buffer if shifted size doesn't match size when shifted
	for(int i = 0; i < 4; i++){
		if(frameShiftedSizeRaw[FeedFrame::SizeShift[i]] != frameSizeRaw[i]){
			CMF_LOG(Feed, Debug, "Frame checksum doesn't match");
			rxBuf->skip(sizeof(FeedFrame::Header));
			return false;
		}
	}

	// Abort if rest of frame is missing
	if(rxBuf->readAvailable() < frameSize + sizeof(FeedFrame::Header) + sizeof(FeedFrame::Trailer) + sizeof(size_t) * 2){
		return false;
	}

	// Search for frame trailer
	size_t endOffset = frameSize + sizeof(FeedFrame::Header) + sizeof(size_t) * 2;
	for(bytesMatched = 0; bytesMatched < sizeof(FeedFrame::Trailer); bytesMatched++){
		uint8_t byte = *rxBuf->peek<uint8_t>(endOffset + bytesMatched);
		if(byte != FeedFrame::Trailer[bytesMatched]) break;
	}

	// Clear whole frame if trailer isn't found at expected offset
	if(bytesMatched != sizeof(FeedFrame::Trailer)){
		rxBuf->skip(endOffset + sizeof(FeedFrame::Trailer));
		CMF_LOG(Feed, Debug, "Trailer missmatch. Clearing %zu bytes", endOffset + sizeof(FeedFrame::Trailer));
		return false;
	}

	return true;
}

std::unique_ptr<FeedFrame> Feed::deserializeFrame(RingBuffer& buf, size_t size){
	auto frame = std::make_unique<FeedFrame>();
	frame->size = size;

	if(buf.readAvailable() < frame->size){
		CMF_LOG(Feed, Debug, "Deserialize data too short, lacks JPG frame (%d, expected %d)", buf.readAvailable(), frame->size);
		return nullptr;
	}

	frame->data = malloc(frame->size);

	if(frame->data == nullptr){
		CMF_LOG(Feed, Debug, "Couldn't allocate buffer for jpg frame data");
		return nullptr;
	}

	buf.read((uint8_t*) frame->data, frame->size);

	return frame;
}
