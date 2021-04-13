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

#include "Random.h"

#include <ege/main/Config.h>
#include <ege/util/system.h>

namespace EGE
{
    // Implementations
    namespace Internal
    {
        MaxUint lcg_gen(MaxUint seed, MaxUint a, MaxUint c, MaxUint m)
        {
            MaxUint val = (a * seed + c) % m / 4;
            return val;
        }
    }

    // Random
    MaxUint Random::nextInt(MaxUint range)
    {
        ASSERT(m_randomizer);
        ASSERT(range != 0);
        return m_randomizer->nextInt() % range;
    }

    float Random::nextFloat(float range, MaxUint precision)
    {
        return (nextInt(precision) / (float)precision) * range;
    }

    double Random::nextDouble(double range, MaxUint precision)
    {
        return (nextInt(precision) / (float)precision) * range;
    }

    MaxInt Random::nextIntRanged(MaxInt begin, MaxInt end)
    {
        if(begin == end)
            return begin; // Begin and end are equal; nothing to randomize :)

        if(begin > end)
            std::swap(begin, end); // Begin is bigger than end, swap them.
        return nextInt(end - begin + 1) + begin;
    }

    float Random::nextFloatRanged(float begin, float end)
    {
        if(begin == end)
            return begin; // Begin and end are equal; nothing to randomize :)

        if(begin > end)
            std::swap(begin, end); // Begin is bigger than end, swap them.
        return nextFloat(end - begin) + begin;
    }

    double Random::nextDoubleRanged(double begin, double end)
    {
        if(begin == end)
            return begin; // Begin and end are equal; nothing to randomize :)

        if(begin > end)
            std::swap(begin, end); // Begin is bigger than end, swap them.
        return nextDouble(end - begin) + begin;
    }

    Random& Random::goodRandom()
    {
        // TODO: replace it by better generator
        static Random random(System::exactTime().ns);
        return random;
    }

    Random& Random::fastRandom()
    {
        static Random random(System::exactTime().ns);
        return random;
    }

    MaxInt Random::randomInt(MaxInt begin, MaxInt end)
    {
        return Random::fastRandom().nextIntRanged(begin, end);
    }

    float Random::randomFloat(float begin, float end)
    {
        return Random::fastRandom().nextFloatRanged(begin, end);
    }

    double Random::randomDouble(double begin, double end)
    {
        return Random::fastRandom().nextDoubleRanged(begin, end);
    }

}
