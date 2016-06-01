/// Please to this file instead of generate random numbers
/// by yourself. It provide a consistent control of the seed.

#ifndef RANDOM_H_
#define RANDOM_H_

#include <cstdlib>
#include <random>

/// Random number generator.
/// Define NDEBUG to enable generating seed accroding to time.
class Random
{
public:
    /// You can change this to other engines.
    typedef std::default_random_engine engine_t;

    static Random &getInstance();

    /// Generate random integer in [lo,hi]
    int getRandomInt(int lo, int hi);

    /// Get random engine
    engine_t &getEngine() { return engine; }

    Random(const Random &) = delete;
    Random &operator=(const Random &) = delete;
private:
    Random();

    static Random *instance;

    engine_t engine;
};

inline Random &Random::getInstance()
{
    if (! instance)
        instance = new Random();
    return *instance;
}

inline int Random::getRandomInt(int lo, int hi)
{
    return std::uniform_int_distribution<int>(lo, hi)(engine);
}

#endif // RANDOM_H_
