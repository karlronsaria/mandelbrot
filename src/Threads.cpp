#include "Threads.h"

std::vector<thread_ref> Threads::list;
volatile std::atomic<bool> Threads::delay_next_poll = false;
volatile std::atomic<bool> Threads::notifying = false;

void Join(std::vector<std::thread>& threads) {
	for (auto& thread : threads)
		if (thread.joinable())
			thread.join();
}

void Threads::Add(const thread_ref& ref) {
	Threads::list.push_back(ref);
}

void Threads::Join() {
	for (auto& thread : Threads::list)
		if (thread.get().joinable())
			thread.get().join();
}
