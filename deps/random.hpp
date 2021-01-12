// random.hpp
#ifndef RANDOM_HPP
#define RANDOM_HPP
#include <cstdlib>

class Random {
public:
    static double uniform(double max = 1) {
        return (std::rand() / RAND_MAX) * max;
    }
};

#endif;