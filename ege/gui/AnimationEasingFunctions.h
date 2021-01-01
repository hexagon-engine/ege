/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
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
