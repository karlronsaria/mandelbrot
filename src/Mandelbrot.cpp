#include "Mandelbrot.h"

std::vector<complex_f> Mandelbrot::_complex_functions = Mandelbrot::GetFunctions();
std::vector<algorithm_f> Mandelbrot::_algorithms = Mandelbrot::GetAlgorithms();
std::vector<color_code_f> Mandelbrot::_color_schemes = Mandelbrot::GetColorSchemes();

complex_f Mandelbrot::FunctionByIndex(int_t index) {
	return _complex_functions[index];
}

complex_f Mandelbrot::FunctionByOrder(int_t order) {
	if (order > 0)
		return _complex_functions[TO_INT(Functions::UNIT_POWER)];

	return _complex_functions[(-1) * (order + TO_INT(Functions::OPAL_VEIN))];
}

color_code_f Mandelbrot::ColorSchemeByIndex(int_t index) {
	return _color_schemes[index];
}

algorithm_f Mandelbrot::AlgorithmByIndex(int_t index) {
	return _algorithms[index];
}

std::vector<complex_f> Mandelbrot::GetFunctions() {
	std::vector<complex_f> list;
	list.push_back(complex_unit_power);     // 0
	list.push_back(complex_opal_vein);      // 1
	list.push_back(complex_collatz_map);    // 2
	list.push_back(complex_collatz_map_2);  // 3
//  list.push_back(other);                  // 4
	return list;
}

std::vector<algorithm_f> Mandelbrot::GetAlgorithms() {
	std::vector<algorithm_f> list;
	list.push_back(EscapeTime);
	list.push_back(Potential);
	return list;
}

std::vector<color_code_f> Mandelbrot::GetColorSchemes() {
	std::vector<color_code_f> list;
	list.push_back(GetColorLinear);
	list.push_back(GetColorHyperbolic);
	list.push_back(GetColorLogarithmic);
	list.push_back(GetColorCircular);
	return list;
}

std::string Mandelbrot::PowerUnitFunctionName(int_t power) {
	return "z^" + std::to_string(power) + " + c";
}

std::string Mandelbrot::GetFunctionName(int_t index) {
	switch ((Functions)(index - 1)) {
	case Functions::COLLATZ_MAP_2:
		return "Collatz Map 2";
	case Functions::COLLATZ_MAP:
		return "Collatz Map";
	case Functions::OPAL_VEIN:
		return "z * (sin(Re(z)), cos(Im(z))) + c";
	default:
		return "";
	}
}

std::string Mandelbrot::GetAlgorithmName(int_t index) {
	switch ((Algorithms)index) {
	case Algorithms::ESCAPE_TIME:
		return "Escape Time";
	case Algorithms::POTENTIAL:
		return "Potential";
	default: return "";
	}
}

std::string Mandelbrot::GetColorSchemeName(int_t index) {
	switch ((ColorSchemes)index) {
	case ColorSchemes::LINEAR:
		return "Linear";
	case ColorSchemes::HYPERBOLIC:
		return "Hyperbolic";
	case ColorSchemes::LOGARITHMIC:
		return "Logarithmic";
	case ColorSchemes::CIRCULAR:
		return "Circular";
	default: return "";
	}
}

pair_t Mandelbrot::complex_unit_power(pair_t z, pair_t c, int_t power) {
	return pow(z, power) + c;
}

pair_t Mandelbrot::complex_opal_vein(pair_t z, pair_t c, int_t power) {
	return z * pair_t( sin(z.re()), cos(z.im()) ) + c;
}

pair_t Mandelbrot::complex_collatz_map(pair_t z, pair_t c, int_t power) {
	return (
		   c                * pow(cos(c * (PI/2)), 2)
		+ (c * (3.f) + 1.f) * pow(sin(c * (PI/2)), 2)
	) * (0.5f) * COLLATZ_BIAS;
}

pair_t Mandelbrot::complex_collatz_map_2(pair_t z, pair_t c, int_t power) {
	return (
		cos(c * PI) + 1.f
		+ (cos(c * PI) + 1.f) * (c * 3.f + 1.f) * (1/16.f)
		* (cos((c * 2.f + -1.f) * (PI/4.f)) * (-sqrt(2.f)) + 3.f)
	) * (0.25f) * COLLATZ_BIAS;
}

sf::Uint8 Mandelbrot::min(sf::Uint8 first, sf::Uint8 secnd) {
	return first < secnd ? first : secnd;
}

sf::Uint8 Mandelbrot::max(sf::Uint8 first, sf::Uint8 secnd) {
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
sf::Color Mandelbrot::HsvToColor(int_t hue, flt_t sat, flt_t val) {
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
void Mandelbrot::ColorToHsv(const sf::Color& color, int_t& hue, flt_t& sat, flt_t& val) {
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

sf::Color Mandelbrot::GetColorLinear(flt_t value) {
	return HsvToColor(value + INIT_HUE, 1.f, 1.f);
}

sf::Color Mandelbrot::GetColorHyperbolic(flt_t value) {
	return HsvToColor(MAX_HUE / value + INIT_HUE, 1.f - 1.f / value, 1.f);
}

sf::Color Mandelbrot::GetColorLogarithmic(flt_t value) {
	return HsvToColor(10 * log(value) + INIT_HUE, 1.f, 1.f);
}

sf::Color Mandelbrot::GetColorCircular(flt_t value) {
	return HsvToColor(100 * sin(value) + INIT_HUE, 1.f, 1.f);
}

int_t Mandelbrot::EscapeTime(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f) {
	z = f(z, c, power);

	if (sqr_sum(z) > ESCAPE_THRESHOLD * ESCAPE_THRESHOLD)
		return ESCAPE_BIAS * iteration;

	return -1LL;
}

int_t Mandelbrot::Potential(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f) {
	flt_t temp;
	z = f(z, c, power);
	temp = sqr_sum(z);

	if (temp > POTENTIAL_THRESHOLD * POTENTIAL_THRESHOLD)
		return POTENTIAL_BIAS * pow(2, iteration) / log(temp);

	return -1LL;
}
