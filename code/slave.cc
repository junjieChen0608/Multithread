#include "code/slave.h"

Slave::Slave(Master* master, const int& id)
    : master_(master), slave_id_(id) {}

Slave::~Slave() {}

void Slave::operator()() {
  std::function<void()> task;
  bool task_allocated = false;

  while (!master_->NeedToShutdown()) {
    {
      std::unique_lock<std::mutex> lock(master_->GetMutex());
      if (master_->GetTaskQueue().empty()) {
        master_->GetWakeupCondition().wait(lock);
      }
      task_allocated = master_->GetTaskQueue().dequeue(task);
    }

    if (task_allocated) {
      task();
    }
  }
}
