#include "Overlay.h"

std::string to_string(flt_t number, int_t precision) {
	std::ostringstream buf;
	buf << std::setprecision(precision) << number;
	return buf.str();
}

bool MouseInView(const canvas_t& canvas) {
	return canvas.getViewport(canvas.getView()).contains(sf::Mouse::getPosition(canvas));
}

Overlay::Overlay(font_t& font, geo_ptr& scales, canvas_t& window) :
	MenuBox(font),
	_scales(scales),
	_iteration(0),
	_max_iterations(DEFAULT_MAX_ITERATIONS)
{
	add_text_item(TO_INT(LabelIndex::TITLE));
	add_text_item(TO_INT(LabelIndex::MOUSE_X));
	add_text_item(TO_INT(LabelIndex::MOUSE_Y));
	add_text_item(TO_INT(LabelIndex::POWER));
	add_text_item(TO_INT(LabelIndex::MAGNIFICATION));
	add_text_item(TO_INT(LabelIndex::ITERATION));
	add_text_item(TO_INT(LabelIndex::THRESHOLD));
	add_text_item(TO_INT(LabelIndex::ALGORITHM));
	add_text_item(TO_INT(LabelIndex::COLOR_SCHEME));
	add_text_item(INIT_X_POS, INIT_Y_POS + (TO_INT(LabelIndex::RENDERING)) * ITEM_HEIGHT + 15);
	add_text_item(INIT_X_POS, INIT_Y_POS + (TO_INT(LabelIndex::NOTIFICATION)) * ITEM_HEIGHT + 15);
	add_text_item(INIT_X_POS, window.getSize().y - 2 * ITEM_HEIGHT);

	type(mnd::MANDELBROT, INIT_PAIR);
	power(DEFAULT_POWER);
	magnification(DEFAULT_MAGNIFICATION);
	iteration(_iteration, _max_iterations);
	threshold(DEFAULT_THRESHOLD);
	algorithm(DEFAULT_ALGORITHM_INDEX);
	color_scheme(DEFAULT_COLOR_SCHEME_INDEX);
}

int Overlay::update(canvas_t& window) {
	_mouse_x = sf::Mouse::getPosition(window).x;
	_mouse_y = sf::Mouse::getPosition(window).y;
	_labels[(int)LabelIndex::MOUSE_X].setString("X:  " + std::to_string(_scales->coord_x(_mouse_x)));
	_labels[(int)LabelIndex::MOUSE_Y].setString("Y:  " + std::to_string((-1) * _scales->coord_y(_mouse_y)));
	return (int)UpdateCodes::IDLE;
}

std::string Overlay::title() const {
	return _labels[(int)LabelIndex::TITLE].getString();
}

int_t Overlay::iteration() const {
	return _iteration;
}

int_t Overlay::init_iteration() {
	_iteration = 0;
	iteration(_iteration, _max_iterations);
	return _iteration;
}

int_t Overlay::next_iteration() {
	_iteration = _iteration + 1;
	iteration(_iteration, _max_iterations);
	return _iteration;
}

int_t Overlay::max_iterations() const {
	return _max_iterations;
}

std::string Overlay::notification() const {
	return _labels[(int)LabelIndex::NOTIFICATION].getString();
}

Geometry2D Overlay::scales() const {
	return *_scales;
}

Overlay& Overlay::type(int_t type, pair_t coords) {
	switch (type) {
	case mnd::MANDELBROT:
		init_title();
		break;
	case mnd::JULIA:
		julia_title(coords);
		break;
	};

	return *this;
}

Overlay& Overlay::title(const std::string& title) {
	_labels[TO_INT(LabelIndex::TITLE)].setString(title);
	return *this;
}

Overlay& Overlay::init_title() {
	title(DEFAULT_TITLE);
	return *this;
}

Overlay& Overlay::julia_title(pair_t coordinates) {
	title(
		"Julia Set (" + to_string(coordinates.re(), DISPLAY_PRECISION)
		       + ", " + to_string(coordinates.im(), DISPLAY_PRECISION) + ")"
	);
	return *this;
}

Overlay& Overlay::power(int_t value) {
	if (value > 0)
		_labels[(int)LabelIndex::POWER].setString(mnd::PowerUnitFunctionName(value));
	else
		_labels[(int)LabelIndex::POWER].setString(mnd::FUNCTION_NAMES[-value]);

	return *this;
}

Overlay& Overlay::magnification(int_t value) {
	_labels[(int)LabelIndex::MAGNIFICATION]
		.setString(
			"Magnification:  "
			+ std::to_string(TO_INT(pow(TrackingBox::factor, value)))
		);
	return *this;
}

Overlay& Overlay::iteration(int_t it) {
	_labels[(int)LabelIndex::ITERATION].setString("Iteration:  " + std::to_string(it) + " of " + std::to_string(_max_iterations));
	return *this;
}

Overlay& Overlay::iteration(int_t it, int_t max) {
	_max_iterations = max;
	return iteration(it);
}

Overlay& Overlay::threshold(int_t value) {
	std::ostringstream buf;
	buf << std::setprecision(4)
		<< mnd::THRESHOLDS[value];
	_labels[(int)LabelIndex::THRESHOLD].setString("Threshold:  " + buf.str());
	return *this;
}

Overlay& Overlay::algorithm(int_t value) {
	_labels[(int)LabelIndex::ALGORITHM].setString("Algorithm:  " + std::string(mnd::ALGORITHM_NAMES[value]));
	return *this;
}

Overlay& Overlay::color_scheme(int_t value) {
	_labels[(int)LabelIndex::COLOR_SCHEME].setString("Color Scheme:  " + std::string(mnd::COLOR_SCHEME_NAMES[value]));
	return *this;
}

Overlay& Overlay::notification(const std::string& message) {
	_labels[(int)LabelIndex::NOTIFICATION].setString(message);
	return *this;
}

Overlay& Overlay::endnote(const std::string& message) {
	_labels[(int)LabelIndex::END_NOTE].setString(message);
	return *this;
}

Overlay& Overlay::rendering(bool isRendering) {
	_labels[(int)LabelIndex::RENDERING].setString(isRendering ? "Rendering..." : "");
	return *this;
}

Overlay& Overlay::state(const State& other) {
	type(other.type, other.j_coords);
	power(other.power);
	magnification(other.magnification);
	iteration(0, other.max_iterations);
	threshold(other.threshold);
	algorithm(other.algorithm_index);
	color_scheme(other.color_scheme_index);
	return *this;
}

int_t TrackingBox::factor = DEFAULT_ZOOM;

TrackingBox::TrackingBox(canvas_t& canvas) :
	_shape(sf::Vector2f{
		static_cast<float>(canvas.getSize().x / TrackingBox::factor),
		static_cast<float>(canvas.getSize().y / TrackingBox::factor)
	})
{
	_shape.setPosition(sf::Mouse::getPosition(canvas).x, sf::Mouse::getPosition(canvas).y);
	_shape.setOutlineColor(sf::Color(255, 255, 255, 128));
	_shape.setOutlineThickness(-3.f);
	_shape.setFillColor(sf::Color::Transparent);
}

void TrackingBox::move(int_t x, int_t y) {
	auto size = _shape.getSize();
	_shape.setPosition(x - size.x / 2, y - size.y / 2);
}

int TrackingBox::update(canvas_t& canvas) {
	auto mouse = canvas.mapPixelToCoords(sf::Mouse::getPosition(canvas));
	auto size = _shape.getSize();
	auto canvasSize = canvas.getSize();
	
	_shape.setPosition(
		mouse.x - size.x / 2 >= 0 && mouse.x + size.x / 2 <= canvasSize.x
		? mouse.x - size.x / 2
		: _shape.getPosition().x,
	
		mouse.y - size.y / 2 >= 0 && mouse.y + size.y / 2 <= canvasSize.y
		? mouse.y - size.y / 2
		: _shape.getPosition().y
	);

	return (int)UpdateCodes::IDLE;
}

void TrackingBox::draw_to(target_t& canvas) {
	canvas.draw(_shape);
}

void TrackingBox::draw_to(canvas_t& canvas) {
	if (MouseInView(canvas))
		canvas.draw(_shape);
}

OrderedPair<int_t> TrackingBox::get_position() const {
	auto size = _shape.getSize();
	auto pos = _shape.getPosition();
	return OrderedPair<int_t> {
		TO_INT(pos.x + size.x / 2),
		TO_INT(pos.y + size.y / 2)
	};
}

model_t TrackingBox::get_boundaries(const Geometry2D& geo) const {
	auto rect = _shape.getGlobalBounds();
	return model_t{
		geo.coord_x(TO_INT(rect.left)),
		geo.coord_x(TO_INT(rect.left) + TO_INT(rect.width)),
		geo.coord_y(TO_INT(rect.top)),
		geo.coord_y(TO_INT(rect.top) + TO_INT(rect.height))
	};
}


InputBox::InputBox(font_t& font):
	MenuBox(font)
{
	add_text_item(
		font,
		INPUT_BOX_FONT_POINT,
		MenuBox::DEFAULT_FILL_COLOR,
		INIT_X_POS + INPUT_BOX_X,
		INIT_Y_POS + INPUT_BOX_Y
	);
}

int InputBox::update(canvas_t& window) {
	return (int)UpdateCodes::IDLE;
}

std::string InputBox::get() const {
	return _labels.back().getString();
}

void InputBox::set(const std::string& msg) {
	_labels.back().setString(msg);
}
