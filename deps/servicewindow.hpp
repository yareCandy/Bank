#pragma once
#include "node.hpp"

enum class WindowStatus { BUSY, IDLE };

class ServiceWindow
{
private:
    Customer customer;
    WindowStatus status;
public:
    ServiceWindow() { this->status = WindowStatus::IDLE; }
    ~ServiceWindow() {};
    bool isIdle() { return this->status == WindowStatus::IDLE; }
    void serverCustomer(Customer& cus) { this->customer = cus; }
    int getArrivalTime() const {
        return (this->customer).arrivalTime;
    }
    int getDuration() const {
        return (this->customer).serviceDuration;
    }
    void setBusy() { this->status = WindowStatus::BUSY; }
    void setIdle() { this->status = WindowStatus::IDLE; }
};
