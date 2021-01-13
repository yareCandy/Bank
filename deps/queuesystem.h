// queuesystem.hpp
#ifndef QUEUESYSTEM_HPP
#define QUEUESYSTEM_HPP

#include <iostream>
#include <queue>
#include "servicewindow.hpp"
#include "event.hpp"
#include "node.hpp"
using namespace std;

class QueueSystem {
private:
    int total_time; // 服务总时间
    int window_num; // 窗口数
    int total_stay_time;//总的等待时间
    int total_customer;//总的服务顾客总数
	double avg_stay_time;//平均时间
	double avg_customers;//平均顾客数目
    ServiceWindow* windows; // 各窗口
    priority_queue<Customer> waitQueue; // 等待队列
    priority_queue<Event> eventQueue; // 事件队列
    Event* curEvent; // 当前事件
public:
    QueueSystem(int ttime, int wnum);
    ~QueueSystem();
    double getAvgcustomers();
    double getAvgStayTime();
    void simulate(int times);
private:
    void init();
    double run();
    void end();
    template<class T> void clear(priority_queue<T>& que);
    int getIdleServiceWindow(); // 获得空闲窗口索引  
    void customerArrived();  // 处理顾客到达事件
    void customerDeparture(); // 处理顾客离开事件
};

#endif