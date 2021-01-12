#include <iostream>
#include <ctime>
#include <cstdlib>
#include "queuesystem.h"
using namespace std;

int main()
{
	srand((unsigned)std::time(0)); // 使用当前时间作为随机数种子
	int total_service_time = 240;  // 按分钟计算
	int window_num = 4;
	int simulate_num = 100000;
 
	QueueSystem system(total_service_time, window_num);
	system.simulate(simulate_num);
 
	cout << "The average time of customer stay in bank: "
		<< system.getAvgStayTime() << endl;
	cout << "The number of customer arrive bank per minute: "
		<< system.getAvgCostomers() << endl;
	getchar();

	return 0;
}