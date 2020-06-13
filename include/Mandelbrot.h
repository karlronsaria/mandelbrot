#pragma once
#ifdef DEBUG
#include "/devlib/cpp/og/include/Debug.h"
#endif
#include "Complex.h"
#include "Geometry.h"
#include <SFML/Graphics.hpp>

const sf::Color INIT_COLOR(0, 0, 0);
const int_t INIT_HUE = 240LL;
const int_t MAX_HUE = 360LL;
const int_t ESCAPE_THRESHOLD = 2LL;
const int_t POTENTIAL_THRESHOLD = 360LL;
const int_t ESCAPE_BIAS = 10LL;
const int_t POTENTIAL_BIAS = 180LL;
const flt_t COLLATZ_BIAS = 50.L;

const int_t MANDELBROT = 0LL;
const int_t JULIA = 1LL;

typedef pair_t(*complex_f)(pair_t z, pair_t c, int_t power);
typedef sf::Color(*color_code_f)(flt_t);
typedef int_t(*algorithm_f)(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f);

class Mandelbrot {
private:
	static std::vector<complex_f> _complex_functions;
	static std::vector<algorithm_f> _algorithms;
	static std::vector<color_code_f> _color_schemes;
public:
	enum class Functions {
		COLLATZ_MAP_2 = -3,
		COLLATZ_MAP = -2,
		OPAL_VEIN = -1,
		UNIT_POWER = 0,
		COUNT = 4
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

	static std::vector<complex_f> GetFunctions();
	static std::vector<algorithm_f> GetAlgorithms();
	static std::vector<color_code_f> GetColorSchemes();

	static complex_f FunctionByIndex(int_t index);
	static complex_f FunctionByOrder(int_t order);
	static color_code_f ColorSchemeByIndex(int_t index);
	static algorithm_f AlgorithmByIndex(int_t index);

	static std::string PowerUnitFunctionName(int_t power);
	static std::string GetFunctionName(int_t index);
	static std::string GetAlgorithmName(int_t index);
	static std::string GetColorSchemeName(int_t index);

	static pair_t complex_unit_power(pair_t z, pair_t c, int_t power);
	static pair_t complex_opal_vein(pair_t z, pair_t c, int_t power);
	static pair_t complex_collatz_map(pair_t z, pair_t c, int_t power);
	static pair_t complex_collatz_map_2(pair_t z, pair_t c, int_t power);

	static sf::Uint8 min(sf::Uint8 first, sf::Uint8 secnd);
	static sf::Uint8 max(sf::Uint8 first, sf::Uint8 secnd);

	static sf::Color HsvToColor(int_t hue, flt_t sat, flt_t val);
	static void ColorToHsv(const sf::Color& color, int_t& hue, flt_t& sat, flt_t& val);
	static sf::Color GetColorLinear(flt_t value);
	static sf::Color GetColorHyperbolic(flt_t value);
	static sf::Color GetColorLogarithmic(flt_t value);
	static sf::Color GetColorCircular(flt_t value);

	static int_t EscapeTime(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f);
	static int_t Potential(pair_t& z, const pair_t& c, int_t power, int_t iteration, complex_f f);
};

typedef Mandelbrot mnd;
