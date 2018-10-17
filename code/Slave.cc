#include "Slave.h"

Slave::Slave(Master& master, const int& id)
    : master_(master), slave_id_(id) {}

Slave::~Slave() {}

void Slave::operator()() {
  std::function<void()> task;
  bool job_allocated = false;

  while (!master_.NeedToShutdown()) {
    {
      std::unique_lock<std::mutex> lock(master_.GetMutex());
      if (master_.GetTaskQueue().empty()) {
        master_.GetWakeupCondition().wait(lock);
      }
      job_allocated = master_.GetTaskQueue().dequeue(task);
    }

    if (job_allocated) {
      task();
    }
  }
}