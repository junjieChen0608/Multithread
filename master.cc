#include "code/master.h"

Master::Master(int n_slaves)
    : slave_pool_(std::vector<std::thread>(n_slaves)), need_shut_down_(false) {
  InitSlavePool();
  std::cout << "master CTOR\n";
}

Master::~Master() {
  // destroy all slaves here
  Shutdown();
  std::cout << "master DTOR\n";
}

void Master::InitSlavePool() {
  std::cout << "init " << slave_pool_.size() << " slaves\n";
  for (int i = 0; i < slave_pool_.size(); ++i) {
    slave_pool_[i] = std::thread(Slave(this, i));
  }
}

// wait for all Slaves to join
void Master::Shutdown() {
  need_shut_down_ = true;
  wakeup_condition_.notify_all();

  for (auto& slave : slave_pool_) {
    if (slave.joinable()) {
      slave.join();
    }
  }
}

bool Master::NeedToShutdown() const {
  return need_shut_down_;
}

TaskQueue& Master::GetTaskQueue() {
  return task_queue_;
}

std::mutex& Master::GetMutex() {
  return mutex_;
}

std::condition_variable& Master::GetWakeupCondition() {
  return wakeup_condition_;
}
