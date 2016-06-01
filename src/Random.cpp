#include <ctime>
#include "random.h"

#ifdef NDEBUG
    #define SEED (time(0))
#else
    #define SEED 0
#endif

Random::Random() : engine(SEED) {}

#undef SEED

Random *Random::instance = NULL;

