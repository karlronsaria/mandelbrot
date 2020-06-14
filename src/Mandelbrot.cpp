#include "Mandelbrot.h"

mnd::complex_f mnd::FunctionByOrder(int_t order) {
	return FUNCTIONS[
		order > mnd::POWER_F_INDEX
			? mnd::POWER_F_INDEX
			: mnd::POWER_F_INDEX + 1 - order
	];
}

std::string mnd::PowerUnitFunctionName(int_t power) {
	return "z^" + std::to_string(power) + " + c";
}

sf::Uint8 mnd::min(sf::Uint8 first, sf::Uint8 secnd) {
	return first < secnd ? first : secnd;
}

sf::Uint8 mnd::max(sf::Uint8 first, sf::Uint8 secnd) {
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
sf::Color mnd::HsvToColor(int_t hue, flt_t sat, flt_t val) {
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
void mnd::ColorToHsv(const sf::Color& color, int_t& hue, flt_t& sat, flt_t& val) {
	// R, G, B values are divided by 255 
	// to change the range from 0..255 to 0..1 
	auto r = color.r / 255.f;
	auto g = color.g / 255.f;
	auto b = color.b / 255.f;

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
		hue = TO_INT(60 * ((TO_INT(g) - TO_INT(b)) / diff) + 360) % 360;

	// if cmax equal g then compute h
	else if (cmax == g)
		hue = TO_INT(60 * ((TO_INT(b) - TO_INT(r)) / diff) + 120) % 360;

	// if cmax equal b then compute h
	else if (cmax == b)
		hue = TO_INT(60 * ((TO_INT(r) - TO_INT(g)) / diff) + 240) % 360;

	// if cmax equal zero
	sat = cmax ? (diff / cmax) * 100 : 0;

	// compute v 
	val = cmax * 100;
}

int_t mnd::EscapeTime(pair_t& z, const pair_t& c, int_t power, int_t iteration, int_t threshold, complex_f f) {
	z = f(z, c, power);

	if (sqr_sum(z) > threshold * threshold)
		return ESCAPE_BIAS * iteration;

	return -1LL;
}

int_t mnd::Potential(pair_t& z, const pair_t& c, int_t power, int_t iteration, int_t threshold, complex_f f) {
	z = f(z, c, power);
	auto temp = sqr_sum(z);

	if (temp > threshold * threshold)
		return POTENTIAL_BIAS * pow(2, iteration) / log(temp);

	return -1LL;
}
