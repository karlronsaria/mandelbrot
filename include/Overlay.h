#pragma once
// #ifdef DEBUG
// #include "/devlib/cpp/og/include/Debug.h"
// #endif
#include "Entity.h"
#include "Mandelbrot.h"
#include "MenuBox.h"
#include "State.h"
#include <iomanip>
#include <sstream>

const char* const DEFAULT_TITLE = "Mandelbrot Set";
const int_t DISPLAY_PRECISION = 3;

const int_t INPUT_BOX_X = 0;
const int_t INPUT_BOX_Y = 245;
const int_t INPUT_BOX_FONT_POINT = 20;

class Overlay : public MenuBox {
private:
	geo_ptr _scales;
	int_t _mouse_x;
	int_t _mouse_y;
	int_t _iteration;
	int_t _max_iterations;

	enum class LabelIndex {
		  TITLE
		, MOUSE_X
		, MOUSE_Y
		, POWER
		, MAGNIFICATION
		, ITERATION
		, THRESHOLD
		, ALGORITHM
		, COLOR_SCHEME
		, RENDERING
		, NOTIFICATION
		, END_NOTE
		, COUNT
	};
public:
	static const LabelIndex STATIC_ITEMS[];

	Overlay() = delete;
	Overlay(const Overlay&) = default;
	Overlay& operator=(const Overlay&) = default;

	Overlay(font_t& font, geo_ptr& geo, canvas_t& window);
	int update(canvas_t& window) override;
	void draw_static_to(target_t& window);

	std::string title() const;
	int_t iteration() const;
	int_t init_iteration();
	int_t next_iteration();
	int_t max_iterations() const;
	std::string notification() const;
	Geometry2D scales() const;

	Overlay& type(int_t type, pair_t coords);
	Overlay& title(const std::string& title);
	Overlay& init_title();
	Overlay& julia_title(pair_t coords);
	Overlay& power(int_t value);
	Overlay& magnification(int_t value);
	Overlay& iteration(int_t it);
	Overlay& iteration(int_t it, int_t max);
	Overlay& threshold(int_t value);
	Overlay& algorithm(int_t value);
	Overlay& color_scheme(int_t value);
	Overlay& notification(const std::string& message);
	Overlay& endnote(const std::string& message);
	Overlay& rendering_msg(const std::string& message);
	Overlay& state(const State& other);
};

std::string to_string(flt_t number, int_t precision);
bool MouseInView(const canvas_t& canvas);

class TrackingBox : public WindowEntity {
private:
	sf::RectangleShape _shape;
public:
	TrackingBox(canvas_t& canvas);
	void move(int_t x, int_t y);
	virtual int update(canvas_t& canvas) override;
	virtual void draw_to(target_t& canvas) override;
	virtual void draw_to(canvas_t& canvas) override;
	OrderedPair<int_t> get_position() const;
	model_t get_boundaries(const Geometry2D& geo) const;

	static int_t factor;
};

class InputBox : public MenuBox {
protected:
	static int default_point;
public:
	InputBox() = delete;
	InputBox(const InputBox&) = delete;
	InputBox& operator=(const InputBox&) = delete;

	InputBox(font_t& font);
	int update(canvas_t& window) override;

	std::string get() const;
	void set(const std::string& msg);
};
