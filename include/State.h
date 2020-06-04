#pragma once
#include "Mandelbrot.h"
#include <stack>

const int_t WIDTH_PIXELS = 1200;
const int_t HEIGHT_PIXELS = 900;
const view_t INIT_VIEW{ 0, WIDTH_PIXELS, 0, HEIGHT_PIXELS };
const model_t INIT_MODEL{ -2.5, 1.5, -1.5, 1.5 };
const int_t DEFAULT_POWER = 2;
const int_t DEFAULT_MAGNIFICATION = 0;

#ifdef DEBUG
const int_t DEFAULT_MAX_ITERATIONS = 3;
#else
const int_t DEFAULT_MAX_ITERATIONS = 100;
#endif

const int_t DEFAULT_ZOOM = 5;
const pair_t INIT_PAIR = { 0.F, 0.F };

const int_t MANDELBROT = 0;
const int_t JULIA = 1;

const int_t DEFAULT_ALGORITHM_INDEX = 0;
const int_t DEFAULT_COLOR_SCHEME_INDEX = 0;

typedef std::stack<model_t> model_stack;

enum class Functions {
	OPAL_VEIN = -1,
	UNIT_POWER = 0,
	COUNT = 2
};

enum class Algorithms {
	ESCAPE_TIME,
	POTENTIAL,
	COUNT
};

enum class ColorSchemes {
	LINEAR,
	HYPERBOLIC,
	LOGARITHMIC,
	CIRCULAR,
	COUNT
};

int_t min_power();
model_stack init_model_stack();
model_stack push(model_stack models, model_t model);
model_stack pop(model_stack models);

struct State
{
	view_t view;
	model_stack models;
	int_t type;
	pair_t j_coords;
	int_t power;
	int_t magnification;
	int_t max_iterations;
	int_t color_scheme_index;
	int_t algorithm_index;

	State();
	State(view_t, const model_stack&, int_t, pair_t, int_t, int_t, int_t, int_t, int_t);
	State(const State&) = default;
	virtual ~State() = default;
	State& operator=(const State&) = default;

	State& new_view(view_t value);
	State& init_view();

	State& new_model_stack(const model_stack& value);
	State& init_model_stack();
	State& push_model(model_t value);
	State& pop_model();

	State& new_type(int_t value);
	State& new_j_coords(pair_t value);

	State& new_power(int_t value);
	State& init_power();
	State& next_power();
	State& prev_power();

	State& init_magnification();
	State& next_magnification();
	State& prev_magnification();

	State& new_max_iterations(int_t value);
	State& init_max_iterations();

	State& new_algorithm(int_t value);
	State& next_algorithm();
	State& prev_algorithm();

	State& new_color_scheme(int_t value);
	State& next_color_scheme();
	State& prev_color_scheme();
};
