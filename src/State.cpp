#include "State.h"

view_t State::_init_view = { 0LL, 1LL, 0LL, 1LL };

int_t min_power() {
	return 2LL - mnd::NUM_FUNCTIONS;
}

model_stack_t init_model_stack() {
	model_stack_t models;
	models.push(model_t{
		DEFAULT_ZOOM * INIT_MODEL.left,
		DEFAULT_ZOOM * INIT_MODEL.right,
		DEFAULT_ZOOM * INIT_MODEL.top,
		DEFAULT_ZOOM * INIT_MODEL.bottom
	});
	models.push(INIT_MODEL);
	return models;
}

model_stack_t push(model_stack_t models, model_t model) {
	models.push(model);
	return models;
}

model_stack_t pop(model_stack_t models) {
	models.pop();
	return models;
}

State::State() :
	view(State::_init_view),
	models(::init_model_stack()),
	type(mnd::MANDELBROT),
	j_coords(INIT_PAIR),
	power(DEFAULT_POWER),
	magnification(DEFAULT_MAGNIFICATION),
	max_iterations(DEFAULT_MAX_ITERATIONS),
	threshold(DEFAULT_THRESHOLD),
	color_scheme_index(0),
	algorithm_index(0)
{}

State::State(int_t width_pixels, int_t height_pixels) :
	view(view_t{ 0LL, width_pixels, 0LL, height_pixels }),
	models(::init_model_stack()),
	type(mnd::MANDELBROT),
	j_coords(INIT_PAIR),
	power(DEFAULT_POWER),
	magnification(DEFAULT_MAGNIFICATION),
	max_iterations(DEFAULT_MAX_ITERATIONS),
	threshold(DEFAULT_THRESHOLD),
	color_scheme_index(0),
	algorithm_index(0)
{
	_init_view = view;
}

State::State(
	view_t someView,
	const model_stack_t& someStack,
	int_t someType,
	pair_t someJCoords,
	int_t somePower,
	int_t someMagnification,
	int_t someMaxIterations,
	int_t someThreshold,
	int_t someColorSchemeIndex,
	int_t someAlgorithmIndex
) :
	view(someView),
	models(someStack),
	type(someType),
	j_coords(someJCoords),
	power(somePower),
	magnification(someMagnification),
	max_iterations(someMaxIterations),
	threshold(someThreshold),
	color_scheme_index(someColorSchemeIndex),
	algorithm_index(someAlgorithmIndex)
{}

State& State::new_view(view_t value) {
	view = value;
	return *this;
}

State& State::init_view() {
	return new_view(State::_init_view);
}

State& State::new_model_stack(const model_stack_t& value) {
	models = value;
	return *this;
}

State& State::init_model_stack() {
	return new_model_stack(::init_model_stack());
}

State& State::push_model(model_t value) {
	return new_model_stack(push(models, value));
}

State& State::pop_model() {
	return new_model_stack(pop(models));
}

State& State::new_type(int_t value) {
	type = value;
	return *this;
}

State& State::new_j_coords(pair_t value) {
	j_coords = value;
	return *this;
}

State& State::new_power(int_t value) {
	auto min = min_power();
	power = value < min ? min : value;
	return *this;
}

State& State::init_power() {
	return new_power(DEFAULT_POWER);
}

State& State::next_power() {
	return new_power(power + 1);
}

State& State::prev_power() {
	return new_power(power - 1);
}

State& State::init_magnification() {
	magnification = 0;
	return *this;
}

State& State::next_magnification() {
	magnification = magnification + 1;
	return *this;
}

State& State::prev_magnification() {
	magnification =
		magnification == -1
		? magnification
		: magnification - 1;

	return *this;
}

State& State::new_max_iterations(int_t value) {
	max_iterations = value;
	return *this;
}

State& State::init_max_iterations() {
	return new_max_iterations(DEFAULT_MAX_ITERATIONS);
}

State& State::init_threshold() {
	threshold = DEFAULT_THRESHOLD;
	return *this;
}

State& State::next_threshold() {
	threshold =
		threshold == mnd::NUM_THRESHOLDS - 1
		? 0 : threshold + 1;

	return *this;
}

State& State::prev_threshold() {
	threshold =
		threshold == 0
		? mnd::NUM_THRESHOLDS - 1
		: threshold - 1;

	return *this;
}

State& State::new_algorithm(int_t value) {
	algorithm_index = value;
	return *this;
}

State& State::next_algorithm() {
	algorithm_index =
		algorithm_index == mnd::NUM_ALGORITHMS - 1
		? 0 : algorithm_index + 1;

	return *this;
}

State& State::prev_algorithm() {
	algorithm_index =
		algorithm_index == 0
		? mnd::NUM_ALGORITHMS - 1
		: algorithm_index - 1;

	return *this;
}

State& State::new_color_scheme(int_t value) {
	color_scheme_index = value;
	return *this;
}

State& State::next_color_scheme() {
	color_scheme_index =
		color_scheme_index == mnd::NUM_COLOR_SCHEMES - 1
		? 0 : color_scheme_index + 1;

	return *this;
}

State& State::prev_color_scheme() {
	color_scheme_index =
		color_scheme_index == 0
		? mnd::NUM_COLOR_SCHEMES - 1
		: color_scheme_index - 1;

	return *this;
}
