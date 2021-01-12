// queuesystem.hpp
#ifndef QUEUESYSTEM_HPP
#define QUEUESYSTEM_HPP

#include <iostream>
#include <queue>
#include "serviceWindows.hpp"
#include "event.hpp"
#include "node.hpp"

class QueueSystem {
private:
    int total_time; // 服务总时间
    int window_num; // 窗口数
    ServiceWindow* windws; // 各窗口
    std::queue<Customer&> waitQueue; // 等待队列
    std::queue<Event&> eventQueue; // 事件队列
    Event* curEvent;
public:
    QueueSystem(int ttime, int wnum);
    ~QueueSystem();
    double getAvgCostomers();
    double getAvgStayTime();
    void simulate(int times);
private:
    void init();
    void run();
    void end();
    int getIdleServiceWindow(); // 获得空闲窗口索引  
    void customerArrived();  // 处理顾客到达事件
    void customerDeparture(); // 处理顾客离开事件
};



#endif;