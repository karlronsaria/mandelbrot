#pragma once
#ifdef DEBUG
#include "/devlib/cpp/og/include/Debug.h"
#endif
#include "Entity.h"
#include "Mandelbrot.h"
#include "MenuBox.h"
#include "State.h"
#include <iomanip>
#include <sstream>

const char* const DEFAULT_TITLE = "Mandelbrot Set";
const int_t DISPLAY_PRECISION = 3;

const int_t INPUT_BOX_X = 0;
const int_t INPUT_BOX_Y = 200;
const int_t INPUT_BOX_FONT_POINT = 20;

class Overlay : public MenuBox {
private:
	geo_ptr _scales;
	int_t _mouse_x;
	int_t _mouse_y;
	int_t _iteration;
	int_t _max_iterations;

	enum class LabelIndex {
		TITLE,
		MOUSE_X,
		MOUSE_Y,
		POWER,
		MAGNIFICATION,
		ITERATION,
		ALGORITHM,
		COLOR_SCHEME,
		RENDERING,
		NOTIFICATION,
		COUNT
	};
public:
	Overlay() = delete;
	Overlay(const Overlay&) = default;
	Overlay& operator=(const Overlay&) = default;

	Overlay(font_t& font, geo_ptr& geo);
	int update(canvas_t& window) override;

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
	Overlay& algorithm(int_t value);
	Overlay& color_scheme(int_t value);
	Overlay& notification(const std::string& message);
	Overlay& rendering(bool isRendering);
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
	model_t get_boundaries(const Geometry2D& geo) const;

	static int_t factor;
	// static int_t box_width;
	// static int_t box_height;
	// static int_t box_x_radius;
	// static int_t box_y_radius;
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
