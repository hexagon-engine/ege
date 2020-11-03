/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <inttypes.h>
#include <memory>

namespace EGE
{

class Randomizer
{
public:
    // Generate new (pseudo)random integer.
    virtual uint64_t nextInt() = 0;
};

template<uint64_t A, uint64_t C, uint64_t M = 1ULL << 63>
class LCGRandomizer : public Randomizer
{
public:
    LCGRandomizer(uint64_t seed)
    : m_seed(seed) {}

    virtual uint64_t nextInt()
    {
        static_assert(M != 0);
        return m_seed = (A * m_seed + C) % M;
    }

private:
    uint64_t m_seed;
};

typedef LCGRandomizer<1103515245, 12345> DefaultLCGRandomizer;

class Random
{
public:
    // Create new, empty (invalid) random.
    Random() = default;

    // The default randomizer is LCG with specified seed.
    Random(uint64_t seed)
    : m_randomizer(std::make_unique<DefaultLCGRandomizer>(seed)) {}

    uint64_t nextInt(uint64_t range);

    void setRandomizer(std::unique_ptr<Randomizer> rand) { m_randomizer = std::move(rand); }

    // precision - the count of "steps".
    // Number is generated using formula ((nextInt() % precision) / precision) * range.
    float nextFloat(float range = 1.f, uint64_t precision = 1024);
    double nextDouble(double range = 1.f, uint64_t precision = 1024);

    int64_t nextIntRanged(int64_t begin, int64_t end);
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
    std::unique_ptr<Randomizer> m_randomizer;
};

}
