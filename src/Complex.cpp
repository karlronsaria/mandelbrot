#include "Complex.h"

/*
	cos(x) = (1/2)(e(ix) + e(-ix))
	cos(ix) = (1/2)(e(-x) + e(x))

	sin(x) = (1/2i)(e(ix) + e(-ix))
	sin(ix) = -(1/2)(e(-x) - e(x))i

	cos(a + bi) = cos a cos bi - sin a sin bi
	cos(a + bi) = cos a (1/2)(e(-b) + e(b)) + sin a (1/2)(e(-b) - e(b)) i
	cos(a + bi) = (1/2)( cos a (e(-b) + e(b)), sin a (e(-b) - e(b)) )
*/
pair_t cos(const pair_t& complex) {
	return pair_t(
		0.5f * cos(complex.re()) * (exp(-complex.im()) + exp(complex.im())),
		0.5f * sin(complex.re()) * (exp(-complex.im()) - exp(complex.im()))
	);
}

/*
	cos(x) = (1/2)(e(ix) + e(-ix))
	cos(ix) = (1/2)(e(-x) + e(x))

	sin(x) = (1/2i)(e(ix) + e(-ix))
	sin(ix) = -(1/2)(e(-x) - e(x))i

	sin(a + bi) = sin a cos bi + cos a sin bi
	sin(a + bi) = sin a (1/2)(e(-b) + e(b)) - cos a (1/2)(e(-b) - e(b)) i
	sin(a + bi) = (1/2)( sin a (e(-b) + e(b)), cos a (e(b) - e(-b)) )
*/
pair_t sin(const pair_t& complex) {
	return pair_t(
		 0.5f * sin(complex.re()) * (exp(-complex.im()) + exp(complex.im())),
		 0.5f * cos(complex.re()) * (exp(complex.im()) - exp(-complex.im()))
	);
}

flt_t abs(const pair_t& complex) {
	return sqrt(complex.re() * complex.re() + complex.im() * complex.im());
}

/*
	phase(a + bi) = {
		with t = atan(abs(b) / abs(a)) :
			t : a > 0 and b > 0,
			pi - t : a < 0 and b > 0,
			pi + t : a < 0 and b < 0,
			2pi - t : a > 0 and b < 0
	}
*/
flt_t phase(const pair_t& complex) {
	flt_t arg = atan(fabs(complex.im()) / fabs(complex.re()));

	if (complex.re() > 0) {
		if (complex.im() > 0)
			return arg;

		return 2 * PI - arg;
	}
	else if (complex.im() > 0) {
		return PI - arg;
	}
	
	return PI + arg;
}

pair_t pow(const pair_t& complex, int_t power) {
	pair_t temp(1.L, 0.L);

	for (int i = 0; i < power; ++i)
		temp *= complex;

	return temp;
}

/*
	(a + bi)^n
	
		= (abs(a + bi) * e(i arg(a + bi)))^n
		= abs^n (a + bi) * e(i n arg(a + bi))
		= abs^n (a + bi) * ( cos(n arg(a + bi)) + sin(n arg(a + bi)) i )
		= abs^n (a + bi) * ( cos(n arg(a + bi)), sin(n arg(a + bi)) )
*/
pair_t fpow(const pair_t& complex, flt_t power) {
	auto Mod = pow(abs(complex), power);
	auto Arg = power * phase(complex);

	return pair_t(
		Mod * cos(Arg),
		Mod * sin(Arg)
	);
}

flt_t sqr_sum(const pair_t& complex) {
	return complex.re() * complex.re() + complex.im() * complex.im();
}
