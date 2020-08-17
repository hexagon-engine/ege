/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

namespace EGE
{

namespace AnimationEasingFunctions
{

double constant1(double x);
double constant0(double x);
double constantHalf(double x);
double average(double x);

// it's the default
double linear(double x);

double easeInSine(double x);
double easeOutSine(double x);
double easeInOutSine(double x);

double easeInQuad(double x);
double easeOutQuad(double x);
double easeInOutQuad(double x);

double easeInCubic(double x);
double easeOutCubic(double x);
double easeInOutCubic(double x);

double easeInQuart(double x);
double easeOutQuart(double x);
double easeInOutQuart(double x);

double easeInQuint(double x);
double easeOutQuint(double x);
double easeInOutQuint(double x);

double easeInExpo(double x);
double easeOutExpo(double x);
double easeInOutExpo(double x);

double easeInCirc(double x);
double easeOutCirc(double x);
double easeInOutCirc(double x);

double easeInBack(double x);
double easeOutBack(double x);
double easeInOutBack(double x);

double easeInElastic(double x);
double easeOutElastic(double x);
double easeInOutElastic(double x);

double easeInBounce(double x);
double easeOutBounce(double x);
double easeInOutBounce(double x);

// TODO: cubic-bezier

}

}
