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

#include "Types.h"

#include <memory>

namespace EGE
{

// Generic randomizer
class Randomizer
{
public:
    virtual ~Randomizer() = default;

    // Generate new (pseudo)random integer.
    virtual MaxUint nextInt() = 0;
};

// Implementations
namespace Internal
{

MaxUint lcg_gen(MaxUint seed, MaxUint a, MaxUint c, MaxUint m);

}

// LCG
template<MaxUint A, MaxUint C, MaxUint M = 1ULL << 32>
class LCGRandomizer : public Randomizer
{
public:
    LCGRandomizer(MaxUint seed)
    : m_seed(seed) {}

    virtual MaxUint nextInt()
    {
        static_assert(M != 0);
        return m_seed = Internal::lcg_gen(m_seed, A, C, M);
    }

private:
    MaxUint m_seed;
};

typedef LCGRandomizer<1103515211, 12347> DefaultLCGRandomizer;

class Random final
{
public:
    // Create new, empty (invalid) random.
    Random() = default;

    // The default randomizer is LCG with specified seed.
    Random(MaxUint seed)
    : m_randomizer(std::make_unique<DefaultLCGRandomizer>(seed)) {}

    MaxUint nextInt(MaxUint range);

    void setRandomizer(UniquePtr<Randomizer> rand) { m_randomizer = std::move(rand); }

    // precision - the count of "steps".
    // Number is generated using formula ((nextInt() % precision) / precision) * range.
    float nextFloat(float range = 1.f, MaxUint precision = 1024);
    double nextDouble(double range = 1.f, MaxUint precision = 1024);

    MaxInt nextIntRanged(MaxInt begin, MaxInt end);
    float nextFloatRanged(float begin, float end);
    double nextDoubleRanged(double begin, double end);

    // The global source of random numbers.
    // Use it when you need good random numbers
    // and don't need to have control of them.
    // The seed of that is system time by default.
    // NOTE: It's still not good for cryptography!
    static Random& goodRandom();

    // Use it when you care about performance
    // and don't need to have control of numbers.
    // The seed of that is system time by default.
    static Random& fastRandom();

private:
    UniquePtr<Randomizer> m_randomizer;
};

MaxInt randomInt(MaxInt begin, MaxInt end);
float randomFloat(float begin, float end);
double randomDouble(double begin, double end);

}
