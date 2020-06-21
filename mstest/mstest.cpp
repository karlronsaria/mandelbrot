#include "pch.h"
#include "CppUnitTest.h"
#include "Complex.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ComplexType
{
	const flt_t DELTA = 1.0e-15L;
	static pair_t a(1.7L, 4.3L);

	TEST_CLASS(Operations)
	{
	public:
		TEST_METHOD(Phase)
		{
			Assert::AreEqual(1.1943059923483736f, arg(a), DELTA, L"Complex phase");
			auto temp = arg(a);
		}

		TEST_METHOD(AbsoluteValue)
		{
			Assert::AreEqual(4.623851208678757f, abs(a), DELTA, L"Complex absolute value");
		}

		TEST_METHOD(SquareSum)
		{
			Assert::AreEqual(4.623851208678757f, sqrt(sqr_sum(a)), DELTA, L"Complex square sum");
		}

		TEST_METHOD(IntegralPower)
		{
			pair_t a_raised(pow(a, 5LL));
			Assert::AreEqual(2011.7657199999992f, a_raised.re(), DELTA, L"Real part of a complex raised by an integer");
			Assert::AreEqual(-648.09772f, a_raised.im(), DELTA, L"Imag part of a complex raised by an integer");
		}

		TEST_METHOD(FractionalPower)
		{
			pair_t a_raised(fpow(a, 2.9L));
			Assert::AreEqual(-80.46583924311182f, a_raised.re(), DELTA, L"Real part of a complex raised by a floating-point value");
			Assert::AreEqual(-26.83483675155851f, a_raised.im(), DELTA, L"Imag part of a complex raised by a floating-point value");

			a_raised = fpow(a, 5.L);
			Assert::AreEqual(2011.7657199999992f, a_raised.re(), DELTA, L"Real part of a complex raised by an integer");
			Assert::AreEqual(-648.09772f, a_raised.im(), DELTA, L"Imag part of a complex raised by an integer");
		}
	};
}
