//  Event.hpp 
#ifndef EVENT_HPP
#define EVENT_HPP
#include "Random.hpp"
#define RANDOM_PARAMETER 100

struct Event { 
    // 使用 -1 表示到达事件, >=0 表示离开事件, 同时数值表示所离开的服务窗口   
    int event_type;
    int occur_time; 

    Event* next;    // 默认为到达事件，发生事件随机
    Event(int occur_time = Random::uniform(RANDOM_PARAMETER), int event_type = -1):
        occur_time(occur_time),
        event_type(event_type),
        next(nullptr) {}
};
#endif