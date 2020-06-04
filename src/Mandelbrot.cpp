#include "Mandelbrot.h"
#include <stack>

template <typename T>
OrderedPair<T> mult(const OrderedPair<T>& a, const OrderedPair<T>& b)
{
	return OrderedPair<T>{
		a.first * b.first - a.secnd * b.secnd,
		a.first * b.secnd + a.secnd * b.first
	};
}

template <typename T>
OrderedPair<T>& OrderedPair<T>::operator+=(const OrderedPair<T>& other)
{
	first = first + other.first;
	secnd = secnd + other.secnd;
	return *this;
}

template <typename T>
const OrderedPair<T> OrderedPair<T>::operator+(const OrderedPair<T>& other) const
{
	auto temp = *this;
	temp += other;
	return temp;
}

template <typename T>
OrderedPair<T>& OrderedPair<T>::operator*=(const OrderedPair<T>& other)
{
	*this = OrderedPair<T>{
		first * other.first - secnd * other.secnd,
		first * other.secnd + secnd * other.first
	};

	return *this;
}

template <typename T>
const OrderedPair<T> OrderedPair<T>::operator*(const OrderedPair<T>& other) const
{
	OrderedPair<T> temp = *this;
	temp *= other;
	return temp;
}

flt_t abs(const pair_t& complex)
{
	return sqrt(complex.first * complex.first + complex.secnd * complex.secnd);
}

pair_t pow(const pair_t& complex, int_t power)
{
	pair_t temp = { 1.f, 0.f };

	for (int i = 0; i < power; ++i)
	{
		temp *= complex;
	}

	return temp;
}

flt_t sqr_sum(const pair_t& complex)
{
	return complex.first * complex.first + complex.secnd * complex.secnd;
}

pair_t complex_unit_power(pair_t z, pair_t c, int_t power)
{
	return pow(z, power) + c;
}

pair_t complex_opal_vein(pair_t z, pair_t c, int_t power)
{
	return z * pair_t{ sin(z.first), cos(z.secnd) } + c;
}

sf::Uint8 min(sf::Uint8 first, sf::Uint8 secnd)
{
	return first < secnd ? first : secnd;
}

sf::Uint8 max(sf::Uint8 first, sf::Uint8 secnd)
{
	return first > secnd ? first : secnd;
}

// Source:
//    https://en.sfml-dev.org/forums/index.php?topic=7313.0
//    https://en.sfml-dev.org/forums/index.php?action=profile;u=2463
//    
// Retrieved:
//    2020_05_10
//    
// hue: 0-360°; sat: 0.f-1.f; val: 0.f-1.f
sf::Color HsvToColor(int_t hue, flt_t sat, flt_t val)
{
	hue %= 360;
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int_t h = hue / 60;
	flt_t f = flt_t(hue) / 60 - h;
	flt_t p = val * (1.f - sat);
	flt_t q = val * (1.f - sat * f);
	flt_t t = val * (1.f - sat * (1 - f));

	switch (h)
	{
	default:
	case 0:
	case 6: return sf::Color(val * 255, t * 255, p * 255);
	case 1: return sf::Color(q * 255, val * 255, p * 255);
	case 2: return sf::Color(p * 255, val * 255, t * 255);
	case 3: return sf::Color(p * 255, q * 255, val * 255);
	case 4: return sf::Color(t * 255, p * 255, val * 255);
	case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}

// Source:
//    https://www.geeksforgeeks.org/program-change-rgb-color-model-hsv-color-model/
//    
// Retrieved:
//    2020_05_12
//    
void ColorToHsv(const sf::Color& color, int_t& hue, flt_t& sat, flt_t& val)
{
	// R, G, B values are divided by 255 
	// to change the range from 0..255 to 0..1 
	auto r = color.r / 255.0;
	auto g = color.g / 255.0;
	auto b = color.b / 255.0;

	// h, s, v = hue, saturation, value 
	double cmax = max(r, max(g, b)); // maximum of r, g, b 
	double cmin = min(r, min(g, b)); // minimum of r, g, b 
	double diff = cmax - cmin; // diff of cmax and cmin. 

	hue = -1, sat = -1;

	// if cmax and cmax are equal then h = 0 
	if (cmax == cmin)
		hue = 0;

	// if cmax equal r then compute h 
	else if (cmax == r)
		hue = (int_t)(60 * ((g - b) / diff) + 360) % 360;

	// if cmax equal g then compute h 
	else if (cmax == g)
		hue = (int_t)(60 * ((b - r) / diff) + 120) % 360;

	// if cmax equal b then compute h 
	else if (cmax == b)
		hue = (int_t)(60 * ((r - g) / diff) + 240) % 360;

	// if cmax equal zero
	sat = cmax ? (diff / cmax) * 100 : 0;

	// compute v 
	val = cmax * 100;
}

sf::Color GetColorLinear(flt_t value)
{
	return HsvToColor(value + INIT_HUE, 1.f, 1.f);
}

sf::Color GetColorHyperbolic(flt_t value)
{
	return HsvToColor(MAX_HUE / value + INIT_HUE, 1.f - 1.f / value, 1.f);
}

sf::Color GetColorLogarithmic(flt_t value)
{
	return HsvToColor(10 * log(value) + INIT_HUE, 1.f, 1.f);
}

sf::Color GetColorCircular(flt_t value)
{
	return HsvToColor(100 * sin(value) + INIT_HUE, 1.f, 1.f);
}

// pair_t PassThrough(const Geometry2D& geo, const pair_t& c)
// {
// 	return c;
// }
// 
// pair_t Transform(const Geometry2D& geo, const pair_t& c)
// {
// 	return pair_t{ geo.coord_x(c.first), geo.coord_y(c.secnd) };
// }

// bool RenderEscapeTimePixel(RenderInfo& info, int_t x, int_t y, pair_t c, color_code_f ColorCoding, complex_f f, transform_f NewPair)
// {
// 	if (info.image.getPixel(x, y) == INIT_COLOR)
// 	{
// 		auto index = (info.view.right - info.view.left) * y + x;
// 		
// 		info.plot[index] = f(info.plot[index], NewPair(info.geo, c), info.power);
// 		
// 		if (sqr_sum(info.plot[index]) > ESCAPE_THRESHOLD * ESCAPE_THRESHOLD)
// 		{
// 			info.image.setPixel(x, y, ColorCoding(info.iteration * 10));
// 			return true;
// 		}
// 	}
// 
// 	return false;
// }
// 
// bool RenderPotentialPixel(RenderInfo& info, int_t x, int_t y, pair_t c, color_code_f ColorCoding, complex_f f, transform_f NewPair)
// {
// 	if (info.image.getPixel(x, y) == INIT_COLOR)
// 	{
// 		flt_t temp;
// 		auto index = (info.view.right - info.view.left) * y + x;
// 
// 		info.plot[index] = f(info.plot[index], NewPair(info.geo, c), info.power);
// 
// 		temp = sqr_sum(info.plot[index]);
// 
// 		if (temp > 360LL*360LL)
// 		{
// 			temp = pow(2, info.iteration) / log(temp);
// 			info.image.setPixel(x, y, ColorCoding(180 * temp));
// 			return true;
// 		}
// 	}
// 
// 	return false;
// }

int_t EscapeTime(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f)
{
	z = f(z, c, power);

	if (sqr_sum(z) > ESCAPE_THRESHOLD * ESCAPE_THRESHOLD)
		return ESCAPE_BIAS * iteration;

	return -1LL;
}

int_t Potential(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f)
{
	flt_t temp;
	z = f(z, c, power);
	temp = sqr_sum(z);

	if (temp > POTENTIAL_THRESHOLD * POTENTIAL_THRESHOLD)
		return POTENTIAL_BIAS * pow(2, iteration) / log(temp);

	return -1LL;
}
