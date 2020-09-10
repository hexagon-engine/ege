/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "AnimationEasingFunctions.h"

#include <cmath>
#include <ege/util/Math.h>

namespace EGE
{

// https://github.com/ai/easings.net/blob/master/src/easings/easingsFunctions.ts
namespace AnimationEasingFunctions
{

static const double c1 = 1.70158;
static const double c2 = c1 * 1.525;
static const double c3 = c1 + 1;
static const double c4 = (2 * pi()) / 3;
static const double c5 = (2 * pi()) / 4.5;

double constant1(double x)
{
    (void) x;
    return 1.0;
}
double constant0(double x)
{
    (void) x;
    return 0.0;
}
double constantHalf(double x)
{
    return x >= 0.5 ? 1.0 : 0.0;
}
double average(double x)
{
    (void) x;
    return 0.5;
}

double linear(double x)
{
    return x;
}

double easeInSine(double x)
{
    return 1 - std::cos((x * M_PI) / 2);
}
double easeOutSine(double x)
{
    return 1 - std::sin((x * M_PI) / 2);
}
double easeInOutSine(double x)
{
    return -(std::cos(M_PI * x) - 1) / 2;
}

double easeInQuad(double x)
{
    return x * x;
}
double easeOutQuad(double x)
{
    return 1 - (1 - x) * (1 - x);
}
double easeInOutQuad(double x)
{
    return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
}

double easeInCubic(double x)
{
    return x * x * x;
}
double easeOutCubic(double x)
{
    return 1 - std::pow(1 - x, 3);
}
double easeInOutCubic(double x)
{
    return x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
}

double easeInQuart(double x)
{
    return x * x * x * x;
}
double easeOutQuart(double x)
{
    return 1 - pow(1 - x, 4);
}
double easeInOutQuart(double x)
{
    return x < 0.5 ? 8 * x * x * x * x : 1 - std::pow(-2 * x + 2, 4) / 2;
}

double easeInQuint(double x)
{
    return x * x * x * x * x;
}
double easeOutQuint(double x)
{
    return 1 - std::pow(1 - x, 5);
}
double easeInOutQuint(double x)
{
    return x < 0.5 ? 16 * x * x * x * x * x : 1 - std::pow(-2 * x + 2, 5) / 2;
}

double easeInExpo(double x)
{
    return x == 0 ? 0 : std::pow(2, 10 * x - 10);
}
double easeOutExpo(double x)
{
    return x == 1 ? 1 : 1 - std::pow(2, -10 * x);
}
double easeInOutExpo(double x)
{
		return x == 0
			? 0
			: x == 1
			? 1
			: x < 0.5
			? std::pow(2, 20 * x - 10) / 2
			: (2 - std::pow(2, -20 * x + 10)) / 2;
}

double easeInCirc(double x)
{
    return 1 - std::sqrt(1 - std::pow(x, 2));
}
double easeOutCirc(double x)
{
    return std::sqrt(1 - std::pow(x - 1, 2));
}
double easeInOutCirc(double x)
{
    return x < 0.5
        ? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
        : (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
}

double easeInBack(double x)
{
    return c3 * x * x * x - c1 * x * x;
}
double easeOutBack(double x)
{
    return 1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2);
}
double easeInOutBack(double x)
{
    return x < 0.5
        ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
        : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

double easeInElastic(double x)
{
    return x == 0
        ? 0
        : x == 1
        ? 1
        : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
}
double easeOutElastic(double x)
{
    return x == 0
        ? 0
        : x == 1
        ? 1
        : std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
}
double easeInOutElastic(double x)
{
    return x == 0
        ? 0
        : x == 1
        ? 1
        : x < 0.5
        ? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) / 2
        : (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) / 2 + 1;
}

double easeInBounce(double x)
{
    return 1 - easeOutBounce(1 - x);
}
double easeOutBounce(double x)
{
    double n1 = 7.5625;
	double d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	} else if (x < 2 / d1) {
	    x -= 1.5 / d1;
		return n1 * x * x + 0.75;
	} else if (x < 2.5 / d1) {
	    x -= 2.25 / d1;
		return n1 * x * x + 0.9375;
	} else {
	    x -= 2.625 / d1;
		return n1 * x * x + 0.984375;
	}
}
double easeInOutBounce(double x)
{
    return x < 0.5
        ? (1 - easeOutBounce(1 - 2 * x)) / 2
        : (1 + easeOutBounce(2 * x - 1)) / 2;
}

}

}
