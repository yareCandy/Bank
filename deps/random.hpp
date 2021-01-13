// random.hpp
#pragma once
#include <cstdlib>

class Random {
public:
    static int uniform(double max = 1) {
        return ((double)std::rand() / RAND_MAX) * max;
    }
};