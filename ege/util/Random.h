/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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

class Random
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

}
