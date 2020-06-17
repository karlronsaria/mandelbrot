#include "Complex.h"

template <typename T>
const Complex<T> Complex<T>::Im = Complex<T>{ (T)0, (T)1 };

/*
	cos(x) = (1/2)(e(ix) + e(-ix))
	cos(ix) = (1/2)(e(-x) + e(x))

	sin(x) = (1/2i)(e(ix) + e(-ix))
	sin(ix) = -(1/2)(e(-x) - e(x))i

	cos(a + bi) = cos a cos bi - sin a sin bi
	cos(a + bi) = cos a (1/2)(e(-b) + e(b)) + sin a (1/2)(e(-b) - e(b)) i
	cos(a + bi) = (1/2)( cos a (e(-b) + e(b)), sin a (e(-b) - e(b)) )
*/
pair_t cos(const pair_t& z) {
	return 0.5L * pair_t(
		cos(z.re()) * (exp(-z.im()) + exp(z.im())),
		sin(z.re()) * (exp(-z.im()) - exp(z.im()))
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
pair_t sin(const pair_t& z) {
	return 0.5L * pair_t(
		 sin(z.re()) * (exp(-z.im()) + exp(z.im())),
		 cos(z.re()) * (exp(z.im()) - exp(-z.im()))
	);
}

pair_t cosh(const pair_t& z) {
	return 0.5L * (exp(z) + exp(-z));
}

pair_t sinh(const pair_t& z) {
	return 0.5L * (exp(z) - exp(-z));
}

pair_t cis(flt_t a) {
	return pair_t{ cos(a), sin(a) };
}

pair_t cis(const pair_t& z) {
	return cos(z) + pair_t::Im * sin(z);
}

/*
	exp(a + bi)
		= exp(a) exp(bi)
		= exp(a)( cos(b) + i sin(b) )
*/
pair_t exp(const pair_t& z) {
	return TO_FLT(exp(z.re())) * cis(z.im());
}

/*
	Suppose
		a + bi = ln(z)

	Then
		exp(a + bi) = z
		exp(a) exp(bi) = z
		exp(a) exp(bi) = abs(z) exp(i arg(z))

	Then
		exp(a) = abs(z)
		a = ln(abs(z))
		b = arg(z)
*/
pair_t log(const pair_t& z) {
	return pair_t{ log(abs(z)), phase(z) };
}

flt_t abs(const pair_t& z) {
	return sqrt(z.re() * z.re() + z.im() * z.im());
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
flt_t phase(const pair_t& z) {
	if (z.re() == 0) {
		if (z.im() > 0)
			return PI / 2;

		return 3 * PI / 2;
	}

	flt_t arg = atan(fabs(z.im()) / fabs(z.re()));

	if (z.re() > 0) {
		if (z.im() > 0)
			return arg;

		return 2 * PI - arg;
	}
	else if (z.im() > 0) {
		return PI - arg;
	}
	
	return PI + arg;
}

pair_t pow(const pair_t& z, int_t c) {
	pair_t temp(1.L, 0.L);

	for (int i = 0; i < c; ++i)
		temp *= z;

	return temp;
}

/*
	a^w = a^(c + di)
		= a^c a^(di)
		= a^c exp(di ln a)
		= a^c (cos(d ln a) + i sin(d ln a))
*/
pair_t pow(flt_t a, const pair_t& w) {
	return TO_FLT(pow(a, w.re())) * cis(w.im() * log(a));
}

/*
	z^w = z^(c + di)
		= z^c z^(di)
		= z^c (cos(d ln z) + i sin(d ln z))
*/
pair_t pow(const pair_t& z, const pair_t& w) {
	return fpow(z, w.re()) * cis(w.im() * log(z));
}

/*
	(a + bi)^n
		= (abs(a + bi) * e(i arg(a + bi)))^n
		= abs^n (a + bi) * e(i n arg(a + bi))
		= abs^n (a + bi) * ( cos(n arg(a + bi)) + i sin(n arg(a + bi)) )
		= abs^n (a + bi) * ( cos(n arg(a + bi)), sin(n arg(a + bi)) )
*/
pair_t fpow(const pair_t& z, flt_t c) {
	return TO_FLT(pow(abs(z), c)) * cis(c * phase(z));
}

flt_t sqr_sum(const pair_t& z) {
	return z.re() * z.re() + z.im() * z.im();
}
