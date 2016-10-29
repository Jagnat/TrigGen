
#include <math.h>

#define J_PI 3.14159265359L


// Simple utility functions
int factorial(int x)
{
	if (x > 1)
		return x * factorial(x - 1);
	else
		return 1;
}

double power(double e, int r)
{
	double p = 1;
	for (int i = 0; i < r; ++i)
	{
		p *= e;
	}
	return p;
}

// Approximates the sine function using the taylor series
double j_sin(double i)
{
	// Scale value to range 0 - 2pi
	double scaled;
	if (i < 0 || i > 2 * J_PI)
		scaled = fmod(i, (double)(2 * J_PI));
	else
		scaled = i;

	// translates input to sample the taylor series near 0
	// for better accuracy, and then post-reflect across
	// the x-axis if necessary
	int multiplier = 1;
	if (scaled > J_PI)
	{
		multiplier = -1;
		scaled -= J_PI;
	}

	if (scaled > J_PI / 2)
	{
		scaled -= J_PI / 2;
		scaled = J_PI / 2 - scaled;
	}

	// iterate
	double sum = 0;

	// Higher for a more accurate approximation
	int max_iterations = 6;

	// Used to switch between adding and subtracting
	// functions in the taylor series
	// More info: https://en.wikipedia.org/wiki/Taylor_series#Approximation

	int polarity = 1;
	int degree = 1;

	for (int x = 0; x < max_iterations; ++x)
	{
		double p = power(scaled, degree);
		p /= (double)factorial(degree);
		p *= polarity;
		sum += p;

		polarity = -polarity;
		degree += 2;
	}

	// Reflect output across x-axis if necessary
	return sum * multiplier;
}

// All other trigonometric functions can be calculated by transforming sine

double j_cos(double i)
{
	return sin(i + J_PI / 2.L);
}

double j_tan(double i)
{
	return sin(i) / cos(i);
}

double j_csc(double i)
{
	return 1 / sin(i);
}

double j_sec(double i)
{
	return 1 / cos(i);
}

double j_cot(double i)
{
	return cos(i) / sin(i);
}