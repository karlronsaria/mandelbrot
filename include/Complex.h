#pragma once
#include "types.h"
#include <math.h>

const flt_t PI = 3.141592653589793L;

template <typename T>
struct OrderedPair {
	T first = (T)0;
	T secnd = (T)0;
};

template <typename T>
class Complex {
private:
	OrderedPair<T> _pair;
public:
	Complex();
	Complex(const T& re, const T& im);

	T re() const;
	T im() const;

	const Complex operator-() const;

	Complex& operator+=(const Complex& other);
	Complex& operator-=(const Complex& other);
	Complex& operator*=(const Complex& other);
	Complex& operator/=(const Complex& other);
	Complex& operator+=(const T& scalar);
	Complex& operator-=(const T& scalar);
	Complex& operator*=(const T& scalar);
	Complex& operator/=(const T& scalar);

	// See CONST_BINARY_OPERATOR below
	const Complex operator+(const Complex& other) const;
	const Complex operator-(const Complex& other) const;
	const Complex operator*(const Complex& other) const;
	const Complex operator/(const Complex& other) const;
	const Complex operator+(const T& scalar) const;
	const Complex operator-(const T& scalar) const;
	const Complex operator*(const T& scalar) const;
	const Complex operator/(const T& scalar) const;
};

typedef Complex<flt_t> pair_t;

template <typename T>
Complex<T> conj(const Complex<T>& complex) {
	return Complex<T>(complex.re(), -complex.im());
}

pair_t cos(const pair_t& complex);
pair_t sin(const pair_t& complex);
pair_t exp(const pair_t& complex);
pair_t log(const pair_t& complex);
flt_t abs(const pair_t& complex);
flt_t phase(const pair_t& complex);
pair_t pow(const pair_t& complex, int_t power);
pair_t fpow(const pair_t& complex, flt_t power);
flt_t sqr_sum(const pair_t& complex);

template <typename T>
Complex<T>::Complex() :
	_pair{ (T)0, (T)0 } {}

template <typename T>
Complex<T>::Complex(const T& re, const T& im) :
	_pair{ re, im } {}

template <typename T>
T Complex<T>::re() const {
	return _pair.first;
}

template <typename T>
T Complex<T>::im() const {
	return _pair.secnd;
}

template <typename T>
const Complex<T> Complex<T>::operator-() const {
	return Complex<T>(-re(), -im());
}

template <typename T>
Complex<T>& Complex<T>::operator+=(const Complex<T>& other) {
	_pair.first = re() + other.re();
	_pair.secnd = im() + other.im();
	return *this;
}

template <typename T>
Complex<T>& Complex<T>::operator-=(const Complex<T>& other) {
	_pair.first = re() - other.re();
	_pair.secnd = im() - other.im();
	return *this;
}

template <typename T>
Complex<T>& Complex<T>::operator*=(const Complex<T>& other) {
	_pair = {
		re() * other.re() - im() * other.im(),
		re() * other.im() + im() * other.re()
	};

	return *this;
}

/*
	z / w
	
		= (abs(z) exp(i arg(z))) / (abs(w) exp(i arg(w)))
		= (abs(z) / abs(w)) exp(i (arg(z) - arg(w)))
		= (abs(z) / abs(w))( cos(arg(z) - arg(w)) + i sin(arg(z) - arg(w)) )
*/
template <typename T>
Complex<T>& Complex<T>::operator/=(const Complex<T>& other) {
	auto Mod = abs(*this) / abs(other);
	auto Arg = phase(*this) - phase(other);

	_pair = {
		Mod * cos(Arg),
		Mod * sin(Arg)
	};

	return *this;
}

template <typename T>
Complex<T>& Complex<T>::operator+=(const T& scalar) {
	_pair.first = re() + scalar;
	return *this;
}

template <typename T>
Complex<T>& Complex<T>::operator-=(const T& scalar) {
	_pair.first = re() - scalar;
	return *this;
}

template <typename T>
Complex<T>& Complex<T>::operator*=(const T& scalar) {
	_pair = {
		re() * scalar,
		im() * scalar
	};

	return *this;
}

template <typename T>
Complex<T>& Complex<T>::operator/=(const T& scalar) {
	_pair = {
		re() / scalar,
		im() / scalar
	};

	return *this;
}

#define CONST_BINARY_OPERATOR(CLASS, TYPE, OP) \
const CLASS CLASS::operator OP (const TYPE & value) const { \
	auto temp = *this; \
	temp OP##= value; \
	return temp; \
}

#define STATIC_CONST_BINARY_OPERATOR(CLASS, TYPE, OP) \
const CLASS operator OP (const TYPE & scalar, const CLASS & vector) { \
	return vector OP scalar; \
}

template <typename T> CONST_BINARY_OPERATOR(Complex<T>, Complex<T>, +)
template <typename T> CONST_BINARY_OPERATOR(Complex<T>, Complex<T>, -)
template <typename T> CONST_BINARY_OPERATOR(Complex<T>, Complex<T>, *)
template <typename T> CONST_BINARY_OPERATOR(Complex<T>, Complex<T>, /)
template <typename T> CONST_BINARY_OPERATOR(Complex<T>, T, +)
template <typename T> CONST_BINARY_OPERATOR(Complex<T>, T, -)
template <typename T> CONST_BINARY_OPERATOR(Complex<T>, T, *)
template <typename T> CONST_BINARY_OPERATOR(Complex<T>, T, /)

template <typename T> STATIC_CONST_BINARY_OPERATOR(Complex<T>, T, +)
template <typename T> STATIC_CONST_BINARY_OPERATOR(Complex<T>, T, *)

template <typename T>
const Complex<T> operator-(const T& scalar, const Complex<T>& vector) {
	return scalar + (-vector);
}

template <typename T>
const Complex<T> operator/(const T& scalar, const Complex<T>& vector) {
	return pair_t{ scalar, 0 } / vector;
}
