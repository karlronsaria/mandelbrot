#pragma once
#include "Mandelbrot.h"
#include <stack>

const model_t INIT_MODEL = AssertNewBounds<flt_t>(-2.5L, 1.5L, -1.5L, 1.5L);
const int_t DEFAULT_POWER = 2LL;
const int_t DEFAULT_MAGNIFICATION = 0LL;

#ifdef DEBUG
const int_t DEFAULT_MAX_ITERATIONS = 15LL;
#else
const int_t DEFAULT_MAX_ITERATIONS = 100LL;
#endif

const int_t DEFAULT_THRESHOLD = 0LL;
const int_t DEFAULT_ZOOM = 5LL;
const pair_t INIT_PAIR = { 0.L, 0.L };

const int_t DEFAULT_ALGORITHM_INDEX = 0LL;
const int_t DEFAULT_COLOR_SCHEME_INDEX = 0LL;

typedef std::stack<model_t> model_stack_t;

int_t min_power();
model_stack_t init_model_stack();
model_stack_t push(model_stack_t models, model_t model);
model_stack_t pop(model_stack_t models);

struct State {
public:
	static view_t _init_view;

	view_t view;
	model_stack_t models;
	int_t type;
	pair_t j_coords;
	int_t power;
	int_t magnification;
	int_t max_iterations;
	int_t threshold;
	int_t color_scheme_index;
	int_t algorithm_index;

	State();
	State(int_t width_pixels, int_t height_pixels);
	State(view_t, const model_stack_t&, int_t, pair_t, int_t, int_t, int_t, int_t, int_t, int_t);
	State(const State&) = default;
	virtual ~State() = default;
	State& operator=(const State&) = default;

	State& new_view(view_t value);
	State& init_view();

	State& new_model_stack(const model_stack_t& value);
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

	State& init_threshold();
	State& next_threshold();
	State& prev_threshold();

	State& new_algorithm(int_t value);
	State& next_algorithm();
	State& prev_algorithm();

	State& new_color_scheme(int_t value);
	State& next_color_scheme();
	State& prev_color_scheme();
};
