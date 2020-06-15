#pragma once
#ifdef DEBUG
#include "C:/devlib/cpp/og/include/Debug.h"
#endif
#include "History.h"
#include "Overlay.h"
#include "Render.h"
#include "Save.h"
#include "TextEntry.h"
#include <chrono>
#include <functional>

const char* const DEFAULT_END_NOTE = "[ H for Help ]";
const int BITS_PER_PIXEL = 32;
const int DELAY_INTERVAL_SEC = 1;
const int J_COORD_PRECISION = 4;

class Application {
private:
	font_t _font;
	geo_ptr _scales;
	canvas_t _window;
	History<State> _states;
	Overlay _main_overlay;
	TrackingBox _magnifier;
	bool _show_overlay;
	bool _show_help;
	int_t _titlebar_height;
	
	sf::Image   _image;
	sf::Sprite  _sprite;
	sf::Texture _texture;

	std::thread _render_thread;
	std::thread _clock_thread;
public:
	static const int_t MSG_DURATION_SEC;
	static const int_t POLLING_DELAY_MSEC;
	static const char* DEFAULT_EXTENSION;

	static volatile std::atomic<bool> delay_next_poll;
	static volatile std::atomic<bool> notifying;

	State current_state;
	const canvas_t& canvas() const;
	const Geometry2D& scales() const;
	
	Application(font_t& font, int_t width_pixels, int_t height_pixels, const std::string& title);
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	virtual ~Application();

	void Mandelbrot();
	void Julia(pair_t coordinates);
	void PushHistory();
	void PushOverlay();
	void ChangeState(std::function<void()>);
	void RestartRender();
	void ChangeOverlay(std::function<void()>);
	void ChangeOverlayAndHistory(std::function<void()>);
	bool GoToPreviousState();
	bool GoToNextState();
	void RebuildGeometry();
	bool PollNext(sf::Event& e);
	void Magnify();
	void Demagnify();
	void Update();
	void Clear();
	void Draw();
	void Show();

	pair_t GetCenterCoords() const;
	std::string NewFileName(std::string extension = Application::DEFAULT_EXTENSION) const;
	bool Save();
	void GoTo(const std::string& str);
	bool EnterNewMaximum(int_t& max);
	bool EnterNewCoordinates(pair_t& coords);

	bool ToggleOverlay();
	bool TogglePauseRender();
	bool ToggleHelpMessage(const char* msg);
	bool IsOpen() const;
	void Close();

	void StartRenderAsync();
	void StopRenderAsync();
	void StartTimedMessageAsync(const std::string& message, int_t seconds = Application::MSG_DURATION_SEC);
	void StartPollDelayAsync();
};
