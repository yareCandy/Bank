// random.hpp
#pragma once
#include <cstdlib>

class Random {
public:
    static int uniform(int max = 1) {
        return (std::rand() / RAND_MAX) * max;
    }
};