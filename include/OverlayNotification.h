#pragma once
#include "Overlay.h"
#include "Threads.h"

class OverlayNotification {
public:
	static const int_t MSG_DURATION_SEC = 3;

	static volatile std::atomic<bool> notifying;
	static std::thread clock_thread;

	void StartTimedAsync(
		Overlay& myOverlay,
		const std::string& message,
		int_t seconds = OverlayNotification::MSG_DURATION_SEC
	);
};
