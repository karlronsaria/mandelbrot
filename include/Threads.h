#pragma once
#include <atomic>
#include <functional>
#include <thread>
#include <vector>

typedef std::reference_wrapper<std::thread> thread_ref;

void Join(std::vector<std::thread>& threads);

class Threads {
public:
	static std::vector<thread_ref> list;
	static void Add(const thread_ref& ref);
	static void Join();

	static volatile std::atomic<bool> delay_next_poll;
	static volatile std::atomic<bool> notifying;
};
