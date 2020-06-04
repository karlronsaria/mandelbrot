#include "OverlayNotification.h"

volatile std::atomic<bool> OverlayNotification::notifying = false;
std::thread OverlayNotification::clock_thread;

void OverlayNotification::StartTimedAsync(
	Overlay& myOverlay, const std::string& message, int_t seconds)
{
	myOverlay.notification(message);

	if (clock_thread.joinable()) {
		notifying = false;
		clock_thread.join();
	}

	clock_thread = std::thread([=, &myOverlay]() {
		auto msg = message;
		auto duration = seconds;
		notifying = true;
		myOverlay.notification(msg);

		for (int i = duration; notifying && i > 0; --i)
			std::this_thread::sleep_for(std::chrono::seconds(1));

		if (notifying) 
			myOverlay.notification("");
	});
}
