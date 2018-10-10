#include "Slave.h"

Slave::Slave(Master& master, const int& id)
    : master_(master), slaveId_(id) {}

Slave::~Slave() {}

void Slave::operator()() {
  std::function<void()> task;
  bool jobAllocated = false;

  while (!master_.isNeedShutdown()) {
    {
      std::unique_lock<std::mutex> lock(master_.getMutex());
      if (master_.getTaskQueue().empty()) {
        master_.getWakeupCondition().wait(lock);
      }
      jobAllocated = master_.getTaskQueue().dequeue(task);
    }

    if (jobAllocated) {
      task();
    }
  }
}