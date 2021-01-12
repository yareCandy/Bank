# C++ 实现银行排队服务模拟

## 问题简述
已知：
+ 银行有多个窗口
+ 存在不同优先级的客户
  
要求：
1. 模拟计算客户的到达，服务，离开的过程
2. 模拟计算客户的总逗留时间

## 要解决的问题

### 模拟的方法
使用`蒙特卡洛方法`模拟银行排队系统，蒙特卡洛方法是指使用计算机的方法对问题进行模拟和复现。用蒙特卡洛方法模拟此问题：
> 假设某银行早上八点开始营业，且银行有 w 个服务窗口，计算：
> + 平均每分钟到达的顾客数
> + 每个顾客平均服务时间

### 主要思路分析
银行服务模型中，服务窗口为 w 个，但客户等待队列只有一个。每当有窗口空闲，且有客服正在等待服务，就可以从等待队列中删去一个记录，并将其移交空闲窗口进行服务。当一个新的客户到达时，系统需要记录用户的到达时间，并将一个服务请求添加到队列的尾部。

其次，从每个客户中，可以抽象出两个属性：
  1. 到达时间
  2. 需要的服务时长

并且这两个属性随机产生。此时，可以得到服务模型如下：
![框架](./pic/框架.png)

## 银行排队系统的设计与建模

#### 随机数的生产
C++中的 `std::rand()` 并不是完全随机生成，不服从数学上的均匀分布，为此分析 `std::rand()` 的原理如下：

`std::rand()` 生成的是一个随机的二进制序列，序列的每一位0或者1的概率都是相等的。而对于 `std::rand()%n` 这个运算，会在 [0, n-1] 之间生成随机数，如果 n-1 的二进制表示中不全是 1，那么里面的数不均匀分布的。仅当 n-1 的二进制数全为 1 时，0，1出现的概率才是均等的，才是真正随机的。

因此，重新实现一个随机数生成器：

```c++
//Random.hpp
#ifndef Random_hpp
#define Random_hpp
#include <cstdlib>
#include <cmath>
// [0, 1) 之间的服从均匀分布的随机值
class Random {
public:  
    static double uniform(double max = 1) {
        return ((double)std::rand() / (RAND_MAX))*max;
    }
};
#endif; /*Random_hpp*/;

/*
 * hpp 文件：
 * 实质就是将.cpp的实现代码混入.h头文件当中
 * 调用者只需要include该hpp文件即可，无需再将cpp加入到project中进行编译
 * 实现代码将直接编译到调用者的obj文件中，不再生成单独的obj
 * 减少调用项目中的cpp文件数与编译次数，减少lib与dll,适合编写公用的开源库
 * 1. 是Header Plus Plus 的简写
 * 2. 与*.h类似，hpp是C++程序头文件
 * 3. 是VCL专用的头文件,已预编译
 * 4. 是一般模板类的头文件
 * 5. *.h里面只有声明，没有实现，而*.hpp里声明实现都有
 * 6. *.h里面可以有using namespace std，而*.hpp里则无
 * 7. *.hpp要注意的问题有：
      a)不可包含全局对象和全局函数
     由于hpp本质上是作为.h被调用者include，当hpp文件中存在全局对象或全局函数，而该hpp被多个调用者include时，
     将在链接时导致符号重定义错误。要避免这种情况，需要去除全局对象，将全局函数封装为类的静态方法。

      b)类之间不可循环调用
      因为hpp必须明确知道应用对象的全部定义，即使不在同一个文件内的循环调用也是不被允许的

      c)不可使用静态成员
       静态成员的使用限制在于如果类含有静态成员，则在hpp中必需加入静态成员初始化代码，
       当该hpp被多个文档include时，将产生符号重定义错误。
       但是可以通过某些方法使得不会错误
*/
```

### 主函数设计逻辑

对外界来说，系统只需要两个参数，即：
1. 总的服务时间
2. 窗口数量

大致的框架如下：
```c
// main.cpp
...
QueueSystem system(total_time, window_num);
system.simulate(simulate_num);
system.getAverageStayTime();
system.getAvgCustomers();    
...
```

### 对象设计模型

分析我们的模型，可以知道我们需要实现：
1. 服务窗口对象
2. 客户请求对象
3. 客户等待队列

由此初步确定这样一些类的设计需求：
+ `QueueSystem` 类: 负责整个队列系统的模拟
+ `ServiceWindow` 类: 队列系统的服务窗口对象，每当一个银行创建时，服务窗口会被创建，假设需要创建 w 个窗口
+ `Random` 类: 生成随机数

#### 顾客类型 `Customer（Node）`

对于顾客而言，只需要保存其到达时间、服务时长信息即可，不需要额外的方法，故将其用 `struct Customer` 类型表示。同时，考虑到每一个顾客也会成为等待队列中的一员。所以，Customer 也可以被称之为队列的一个 `Node`。此外，每个顾客说需要的服务时间是随机的，由此可设计出 `Customer` 类型。

```c
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

typedef struct Node Node; // 为顾客类型起别名
typedef struct Node Customer; // 为顾客类型起别名
```

#### `ServiceWindow` 类

服务窗口类 `ServiceWindow` 需要成员表示正在服务的顾客，需要一个表示当前窗口状态的成员。服务窗口有两种状态：`BUSY` 和 `IDLE`，分别代表`正在服务` 和 `空闲`状态。状态可以用枚举类型表示:
```c
enum Status {BUSY, IDLE};
```

结合前述分析，可以设计出 `ServiceWindow` 的框架如下：
```c
enum WindowStatus {BUSY, IDLE}; // 定义枚举类型

class ServiceWindow
{
private:
    Customer customer; 
    WindowStatus status;
public:
    ServiceWindow(); // 初始窗口空闲
    void serverCustomer(Customer& cus); // 服务指定顾客
};
```

#### 事件驱动设计

为了让整个系统『运行』起来，采用**事件驱动的设计**。在整个系统中，无非出现两种事件：
+ 有顾客到达
+ 有顾客离开

其中，**顾客离开的事件，同时还包含了窗口服务等待队列中的下一个顾客这个事件**。所以，够维护一个事件列表，就能够驱动整个队列系统的运行了。因为，当事件发生时，我们通知这个队列系统更新他自身的状态即可。

由此，我们定义事件如下：
```c
struct Event { 
    // -1 到达事件
    // >=0 离开事件, 数值表示所离开的服务窗口 
    int event_type;
    int occur_time; 
    Event* next;    // 默认为到达事件，发生事件随机

    Event(int event_type = -1, int occur_time = Random::uniform(RANDOM_PARAMETER)):
        occur_time(occur_time),
        event_type(event_type),
        next(nullptr) {}
};
```
**注意：** 这里使用了 `int` 整形表示事件的类型，而不是枚举型，因此我们有 w 个窗口，用数组模拟窗口，则下标即可对应到窗口。这里使用整数作为事件类型，可以一举两得。

#### 服务系统 `QueueSystem`

首先需要给外界提供的设置参数：
+ `window_num`：窗口的数量
+ `total_time`: 银行服务的总时长

其次，系统至少需要提供给外界以下接口：
1. `simulate`: 模拟
2. `getAverageStayTime`: 获取每个顾客平均逗留时间
3. `getAvgCustomers`: 获得平均每分钟到达的顾客数

第三，为了系统正常运行，需要设计如下的内部工具成员函数：
1. `init`: 负责系统的初始化
2. `run`：令系统开始运行
3. `end`: 清理一次运行后的状态

第四，整个系统需要的成员有：
1. `ServiceWindow* windows`：银行窗口
2. `queue<Customer&> waitQueue`： 等待服务的顾客队列
3. `queue<Event&>eventQueue`: 事件队列
4. `Event* curEvent`: 系统的当前时间

第五，处理时间的方法：
1. `void customerArrived()`: 处理顾客到达事件
2. `void customerLeaved()`: 处理顾客离开事件

最后，我们所希望的平均顾客逗留时间和平均每分钟的顾客数涉及的四个变量：
1. `int total_customer_stay_time`: 顾客的总逗留时间 
2. `int total_customer_num`: 一次运行中系统服务的顾客数量 
3. `double avg_customers`: 每分钟平均顾客数 
4. `double avg_stay_time`: 顾客平均逗留时间 

根据需求，可以设计出 `QueueSystem` 的框架：
```c
class QueueSystem {
private:
    int total_time;
    int window_num;
    ServiceWindow* windws;
    queue<Customer&> waitQueue;
    queue<Event&> eventQueue;
    Event* curEvent;
public:
    QueueSystem(int ttime, wnum);
    ~QueueSystem();
    double getAvgCostomers();
    double getAvgStayTime();
    void simulate();
private:
    void init();
    void run();
    void end();
    int getIdleServiceWindow(); // 获得空闲窗口索引  
    void customerArrived();  // 处理顾客到达事件
    void customerDeparture(); // 处理顾客离开事件
};
```