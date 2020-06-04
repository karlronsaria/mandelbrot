#pragma once
#ifdef DEBUG
#include "/devlib/cpp/og/include/Debug.h"
#endif
#include "History.h"
#include "Mandelbrot.h"
#include "Overlay.h"
#include "Render.h"
#include "State.h"
#include "Threads.h"
#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stack>
#include <thread>
#include <time.h>

const int FLT_T_SIZE = sizeof(flt_t);
const int COUPLET_SIZE = 2;
const int MAX_TEXT_ENTRY = 15;

std::string to_hex_str(flt_t value);
flt_t to_float(const std::string& word);
std::string GetDateTimeString();

struct TextEntry {
	std::string str;
	int pos = 0;
	int max_length = MAX_TEXT_ENTRY;
};

bool IsValidCharacter(char temp);
void Insert(TextEntry& info, char payload);
void Backspace(TextEntry& info);
void Delete(TextEntry& info);
void PositionStart(TextEntry& info);
void PositionEnd(TextEntry& info);
void PositionForward(TextEntry& info);
void PositionBack(TextEntry& info);
int_t ToInteger(TextEntry& info);

class Application {
private:
	font_t  _font;
	geo_ptr _scales;
	History<State> _states;
	State _current_state;
	Overlay _main_overlay;
public:
	canvas_t    _window;
	sf::Image   _image;
	sf::Sprite  _sprite;
	sf::Texture _texture;

	TrackingBox _magnifier;
	bool _show_overlay;
private:
	std::thread _render_thread;
	std::thread _clock_thread;
public:
	static const flt_t MIN_PIXEL_RATE;
	static const int_t MSG_DURATION_SEC;
	static const int_t POLLING_DELAY_MSEC;
	static const char* DEFAULT_EXTENSION;

	Application(font_t& font);
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	virtual ~Application();

	const State& state() const;
	State& state();
	const Overlay& main_overlay() const;
	Overlay& main_overlay();
	void Mandelbrot();
	void Julia(pair_t coordinates);

	void PushHistory();
	void PushOverlay();
	bool GoToPreviousState();
	bool GoToNextState();
	void RebuildGeometry();
	bool PollNext();
	flt_t GetPixelMinimum(flt_t rate = Application::MIN_PIXEL_RATE) const;
	void GetCenter(flt_t& x, flt_t& y) const;
	std::string NewFileName(std::string extension = Application::DEFAULT_EXTENSION) const;
	void Magnify();
	void Demagnify();
	void Update();
	void Clear();
	void Draw();
	void Show();
	bool Save();
	void GoTo(const std::string& str);
	bool EnterNewMaximum(int_t& max);
	bool PollCoordinates(pair_t& coords);

	void Render();
	void StartRenderAsync();
	void StopRenderAsync();
	void StartTimedMessageAsync(const std::string& message, int_t seconds = Application::MSG_DURATION_SEC);
	void StartPollDelayAsync();

	sf::Event event;

	static volatile std::atomic<bool> delay_next_poll;
	static volatile std::atomic<bool> notifying;
};
