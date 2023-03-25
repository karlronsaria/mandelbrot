#pragma once
// #ifdef DEBUG
// #include "/devlib/cpp/og/include/Debug.h"
// #endif
#include "Complex.h"
#include "Geometry.h"
#include <SFML/Graphics.hpp>

#define NEW_COMPLEX_F(z, c, power) \
[](pair_t z, pair_t c, int_t power)

#define ARRAY_SIZE(arr) \
sizeof(arr)/sizeof(arr[0])

#define LOG(value) log(value)

typedef flt_t threshold_t;

namespace mnd
{
	const sf::Color INIT_COLOR(0, 0, 0);
	const int_t INIT_HUE = 240LL;
	const int_t MAX_HUE = 360LL;
	const int_t ESCAPE_THRESHOLD = 2LL;
	const int_t POTENTIAL_THRESHOLD = 360LL;
	const int_t ESCAPE_BIAS = 10LL;
	const int_t POTENTIAL_BIAS = 180LL;
	const int_t POWER_F_INDEX = 0LL;

	const int_t MANDELBROT = 0LL;
	const int_t JULIA = 1LL;

	typedef pair_t(*complex_f)(pair_t z, pair_t c, int_t power);
	typedef sf::Color(*color_code_f)(flt_t);
	typedef int_t(*algorithm_f)(pair_t& z, const pair_t& c, int_t power, int_t iteration, threshold_t threshold, complex_f f);

	const wchar_t const* const FUNCTION_NAMES[] = {
		  L"z^|z| + c"
		, L"exp(z) + c"
		, L"sin(z) + c"
		, L"z * sin(z) + c"
		, L"z * cos(sin(z)) + c"
		, L"tan(z) + c"
		, L"Collatz Map"
		, L"Collatz Map 2"
		, L"∑( z^(2i + 1), for i in [0, 7] ) + c"
		, L"z * (cos(Re(z)), sin(Im(z))) + c"
		, L"z * (sin(Re(z)), cos(Im(z))) + c"
		, L"exp(floor(z)) + c"
		, L"exp(iz) + c"
	};

	const complex_f FUNCTIONS[] = {
		  NEW_COMPLEX_F(z, c, power) {
			  return c + fpow(z, power);
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + fpow(z, abs(z));
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + exp(z);
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + sin(z);
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + z * sin(z);
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + z * cos(sin(z));
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + tan(z);
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + 0.5L * (
					 z * pow(cos(z * (PI / 2)), 2)
				  + (z * (3.L) + 1.L) * pow(sin(z * (PI / 2)), 2)
			  );
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + 0.25L * (
					 cos(z * PI) + 1.L
				  + (cos(z * PI) + 1.L) * (z * 3.L + 1.L) * (1 / 16.L)
				  * (cos((z * 2.L - 1.L) * (PI / 4.L)) * (-sqrt(2.L)) + 3.L)
			  );
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  for (int i = 0; i < 7; ++i)
			  	  c += fpow(z, 2.L * i + 1);

			  return c;
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + z * pair_t(cos(z.re()), sin(z.im()));
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + z * pair_t(sin(z.re()), cos(z.im()));
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  z = exp(z);
			  return c + pair_t{ floor(z.re()), floor(z.im()) };
		  }
		, NEW_COMPLEX_F(z, c, power) {
			  return c + cis(z);
		  }
	};

	constexpr int_t NUM_FUNCTIONS = ARRAY_SIZE(FUNCTIONS);

	sf::Uint8 min(sf::Uint8 first, sf::Uint8 secnd);
	sf::Uint8 max(sf::Uint8 first, sf::Uint8 secnd);

	sf::Color HsvToColor(int_t hue, flt_t sat, flt_t val);
	void ColorToHsv(const sf::Color& color, int_t& hue, flt_t& sat, flt_t& val);

	const char const* const COLOR_SCHEME_NAMES[] = {
		  "Linear"
		, "Hyberbolic"
		, "Logarithmic"
		, "Sinusoidal"
	};

	const color_code_f COLOR_SCHEMES[] = {
		  [](flt_t v) { return HsvToColor(v + INIT_HUE, 1.f, 1.f); }
		, [](flt_t v) { return HsvToColor(MAX_HUE / v + INIT_HUE, 1.f, 1.f); }
		, [](flt_t v) { return HsvToColor(10 * LOG(v) + INIT_HUE, 1.f, 1.f); }
		, [](flt_t v) { return HsvToColor(100 * sin(v) + INIT_HUE, 1.f, 1.f); }
	};

	constexpr int_t NUM_COLOR_SCHEMES = ARRAY_SIZE(COLOR_SCHEMES);

	int_t EscapeTime(pair_t& z, const pair_t& c, int_t power, int_t iteration, threshold_t threshold, complex_f f);
	int_t Potential(pair_t& z, const pair_t& c, int_t power, int_t iteration, threshold_t threshold, complex_f f);
	int_t Dichromatic(pair_t& z, const pair_t& c, int_t power, int_t iteration, threshold_t threshold, complex_f f);

	const threshold_t THRESHOLDS[] = {
		  2.L
		, 3.L
		, 5.L
		, 10.L
		, 20.L
		, 180.L
		, 1000.L
		, 100000.L
		, 10000000.L
		, 100000000000.L
		, 1000000000000000.L
		, 0.1L
		, 0.5L
		, 0.8L
		, 1.L
		, 1.1L
		, 1.5L
		, 1.8L
	};

	constexpr int_t NUM_THRESHOLDS = ARRAY_SIZE(THRESHOLDS);

	const char const* const ALGORITHM_NAMES[] = {
		  "Escape Time"
		, "Potential"
		, "Dichromatic"
	};

	const algorithm_f ALGORITHMS[] = {
		  EscapeTime
		, Potential
		, Dichromatic
	};

	constexpr int_t NUM_ALGORITHMS = ARRAY_SIZE(ALGORITHMS);

	complex_f FunctionByOrder(int_t order);
	std::string PowerUnitFunctionName(int_t power);
};
