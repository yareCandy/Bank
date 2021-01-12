#include <iostream>
#include "queuesystem.h"
using namespace std;

QueueSystem::QueueSystem(int ttime, int wnum) {
    this ->total_time = ttime;
    this ->window_num = wnum;

    init();
}

QueueSystem::~QueueSystem() {

}

double QueueSystem::getAvgCostomers() {

}

double QueueSystem::getAvgStayTime() {

}

