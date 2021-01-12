// node.hpp
#ifndef NODE_HPP
#define NODE_HPP
#include "random.hpp"
#define RANDOM_PARAMETER 100

struct Node
{
    int arrivalTime;
    int serviceDuration;
    Node *next;
    Node(int at = 0, int sd = Random::uniform(RANDOM_PARAMETER)):
        arrivalTime(at),
        serviceDuration(sd),
        next(nullptr){}
};

typedef struct Node Node;
typedef struct Node Customer;

#endif;