#include "code/slave.h"

Slave::Slave(Master* master, const int& id)
    : master_(master), slave_id_(id) {}

Slave::~Slave() {}

void Slave::operator()() {
  std::unique_ptr<std::function<void()>> task;

  while (!master_->NeedToShutdown()) {
    {
      std::unique_lock<std::mutex> lock(master_->GetMutex());
      if (master_->GetTaskQueue().empty()) {
        master_->GetWakeupCondition().wait(lock);
      }
      task = master_->GetTaskQueue().dequeue();
    }

    if (task) {
      (*task)();
    }
  }
}
