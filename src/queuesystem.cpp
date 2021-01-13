#include "queuesystem.h"
using namespace std;

// 构造：服务时间、窗口数、总的等待时间、总客户数，并建立窗口
QueueSystem::QueueSystem(int ttime, int wnum) :
    total_time(ttime),
    window_num(wnum),
    total_stay_time(0),
    total_customer(0) {
    this ->windows = new ServiceWindow[wnum];
}

// init: 每一次run前的初始化，创建一个到达事件，并设置为当前事件
void QueueSystem::init() {
    curEvent = new Event();
    Customer cus(curEvent->occur_time);
    waitQueue.push(cus);
}

// 析构：回收动态分配的空间
QueueSystem::~QueueSystem() {
    delete [] this->windows;
}

// 获取平均每分钟到达的顾客数
double QueueSystem::getAvgcustomers() {
    return avg_customers;
}

// 获取平均的等待时长
double QueueSystem::getAvgStayTime() {
    return avg_stay_time;
}

// 总的模拟程序，调用times次run，并统计顾客数、等待时间等数据
void QueueSystem::simulate(int times) {
    int i = times, sum = 0;
    while(i--) {
        sum += run(); 
    }
    // sum 每次的平均等待时间之和，除以模拟次数就是平均等待时间
    avg_stay_time = sum/times;
    // total_total是每一次模拟的客户的综合
    avg_customers = (double)total_customer/(times*total_time);
}


// 负责运行一次，并返回这一次顾客的平均等待时间
// total_stay_time/total_customer
// total_stay_time: 当前次运行时，顾客总的等待时间
double QueueSystem::run() {
    init();
    while(curEvent) {
        if(curEvent ->event_type == -1) {
            customerArrived(); // 处理顾客到达事件
        }
        else {
            customerDeparture();
        }
        // 回收空间
        delete curEvent; 
        // 从事件队列中取一个事件，可能是 nullptr空事件
        curEvent = new Event(eventQueue.top());
    }
    end(); // 清空这一次运行的转态

    return (double)total_stay_time/total_customer;
}

/* 清空这一次运行的状态，包括：
 * 1. 将所有的窗口改为闲置
 * 2. 将顾客全部撵出去
 * 3. 将事件全部忽略
 * */
void QueueSystem::end() {
    for(int i = 0; i < window_num; i++) {
        windows[i].setIdle();
    }
    clear(waitQueue);
    clear(eventQueue);
}

// 清空队列：使用stl标准的swap函数，更加高效
template<class T>
void QueueSystem::clear(priority_queue<T>& que) {
    while (!que.empty()) {
        que.pop();
    }
}

/* 处理客户到达事件，主要负责：
 * 1. 顾客数量加 1
 * 2. 生成下一个到达事件
 * 3. 若有空闲窗口，就取出一个客户进行服务
 * */
void QueueSystem::customerArrived() {
    ++total_customer;
    // 随机生成下一个到达事件的到达事件
    int time = curEvent->occur_time + Random::uniform(50);
    if(time < total_time) {
        Event event(time, -1);
        eventQueue.push(event);
    }
    // 将当前的到达顾客放置到等待队列中
    Customer cus(curEvent->occur_time);
    waitQueue.push(cus);

    // 若有空闲窗口，则将当前事件中的顾客服务
    int idleWindow = getIdleServiceWindow();
    if(idleWindow != -1) { // 有空闲窗口
        // 从等待队列中取出一个顾客进行服务，此顾客一定与当前时间的顾客一致（时间相同）
        Customer cus = waitQueue.top();
        waitQueue.pop();
        windows[idleWindow].serverCustomer(cus);
        windows[idleWindow].setBusy();

        // 客户被服务，还需要另外创建一个客户离开事件
        Event tmp_event(cus.arrivalTime + cus.serviceDuration, idleWindow);
        eventQueue.push(tmp_event);
    }
}

// 寻找一个空闲窗口，找不到返回-1，
int QueueSystem::getIdleServiceWindow() {
    int i = window_num - 1;
    for(; i >= 0; --i) {
        if(windows[i].isIdle()) {
            return i;
        }
    }
    return i;
}

// 处理客户离开事件
void QueueSystem::customerDeparture() {
    int occur_time = curEvent->occur_time;
    if(occur_time >= total_time) return;
    // 计算总的等待时间
    int pos = curEvent ->event_type;
    total_stay_time += occur_time - windows[pos].getArrivalTime();
    
    if(waitQueue.empty()) {
        windows[pos].setIdle(); 
        return;
    }

    // 有顾客在等待，直接开始服务
    Customer cus = waitQueue.top();
    waitQueue.pop();
    windows[pos].serverCustomer(cus);

    // 添加一个离开事件
    Event tmp_event(occur_time + cus.serviceDuration, pos);
    eventQueue.push(tmp_event);
}