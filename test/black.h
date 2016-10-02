// black.h - Black option forward pricing model.
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "xll_roots.h"

namespace prob {

	inline double normal_cdf(double x)
	{
		return (1 + erf(x/M_SQRT2))/2;
	}

} // namespace prob

namespace black {

	// P(F <= k) where F = f exp(-s^2t/2 + s B_t)
	// B_t/sqrt(t) <= (log k/f - s^2t/2)/s sqrt(t)
	inline double P(double f, double s, double k, double t)
	{
		auto srt = s*sqrt(t);

		return prob::normal_cdf(log(k/f)/srt + srt/2);
	}

	// P(F exp(s^2t) <= k) where F = f exp(-s^2t/2 + s B_t)
	inline double P_(double f, double s, double k, double t)
	{
		auto srt = s*sqrt(t);

		return prob::normal_cdf(log(k/f)/srt - srt/2);
	}

	// E max(k - F, 0) where F = f exp(-s^2t/2 + s B_t)
	inline double put_value(double f, double s, double k, double t)
	{
		return k*P(f, s, k, t) - f*P_(f, s, k, t);
	}
#pragma warning(push)
#pragma warning(disable: 100)
	// Calculate volatility given price.
	inline double implied_volatility(double f, double p, double k, double t)
	{
		//!!! *Use the class gsl::root
		//!!! find an initial guess
		double s = 0;

		//!!! set up a 1-d solver from xll_roots.h

		//!!! solve for the implied volatility

		return s;
	}
#pragma warning(pop)
} // namespace black
