// serviceWindowss.hpp
#ifndef SERVICEWINDOWS_HPP
#define SERVICEWINDOWS_HPP
#include "node.hpp"

enum WindowStatus {BUSY, IDLE};

class ServiceWindow
{
private:
    Customer customer;
    WindowStatus status;
public:
    ServiceWindow(){this ->status = IDLE;}
    ~ServiceWindow();
    bool isIdle(){return this->status == IDLE;}
    void serverCustomer(Customer& cus) { this->customer = cus;}
    int getArrivalTime() const {
        return (this->customer).arrivalTime;
    }
    int getDuration() const {
        return (this->customer).serviceDuration;
    }
    void setBusy() {this->status = BUSY;}
    void setIdle() {this->status = IDLE;}
};

#endif;
