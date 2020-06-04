#pragma once
#ifdef DEBUG
#include "/devlib/cpp/og/include/Debug.h"
#endif
#include "Geometry.h"
#include <SFML/Graphics.hpp>

const sf::Color INIT_COLOR(0, 0, 0);
const int_t INIT_HUE = 240;
const int_t MAX_HUE = 360;
const int_t ESCAPE_THRESHOLD = 2LL;
const int_t POTENTIAL_THRESHOLD = 360LL;
const int_t ESCAPE_BIAS = 10;
const int_t POTENTIAL_BIAS = 180;

template <typename T>
struct OrderedPair
{
	T first = (T)0;
	T secnd = (T)0;

	OrderedPair& operator+=(const OrderedPair& other);
	const OrderedPair operator+(const OrderedPair& other) const;

	OrderedPair& operator*=(const OrderedPair& other);
	const OrderedPair operator*(const OrderedPair& other) const;
};

typedef OrderedPair<flt_t> pair_t;

struct RenderInfo
{
	Geometry2D geo;
	view_t view;
	int_t iteration;
	int_t power;
	pair_t* plot;
	sf::Image image;
};

typedef pair_t(*complex_f)(pair_t z, pair_t c, int_t power);
typedef sf::Color(*color_code_f)(flt_t);
typedef int_t(*algorithm_f)(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f);
// typedef pair_t(*transform_f)(const Geometry2D&, const pair_t&);
// typedef bool(*algorithm_f)(RenderInfo&, int_t, int_t, pair_t, color_code_f, complex_f, transform_f);

flt_t abs(const pair_t& complex);
pair_t pow(const pair_t& complex, int_t power);
flt_t sqr_sum(const pair_t& complex);

pair_t complex_unit_power(pair_t z, pair_t c, int_t power);
pair_t complex_opal_vein(pair_t z, pair_t c, int_t power);

sf::Uint8 min(sf::Uint8 first, sf::Uint8 secnd);
sf::Uint8 max(sf::Uint8 first, sf::Uint8 secnd);

sf::Color HsvToColor(int_t hue, flt_t sat, flt_t val);
void ColorToHsv(const sf::Color& color, int_t& hue, flt_t& sat, flt_t& val);
sf::Color GetColorLinear(flt_t value);
sf::Color GetColorHyperbolic(flt_t value);
sf::Color GetColorLogarithmic(flt_t value);
sf::Color GetColorCircular(flt_t value);
// pair_t PassThrough(const Geometry2D& geo, const pair_t& c);
// pair_t Transform(const Geometry2D& geo, const pair_t& c);

// bool RenderEscapeTimePixel(
// 	RenderInfo& info,
// 	int_t x, int_t y, pair_t c,
// 	color_code_f ColorCoding,
// 	complex_f f = complex_unit_power,
// 	transform_f NewPair = PassThrough
// );
// 
// bool RenderPotentialPixel(
// 	RenderInfo& info,
// 	int_t x, int_t y, pair_t c,
// 	color_code_f ColorCoding,
// 	complex_f f = complex_unit_power,
// 	transform_f NewPair = PassThrough
// );

int_t EscapeTime(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f);
int_t Potential(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f);
