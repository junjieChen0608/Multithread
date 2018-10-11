#include "Master.h"

Master::Master(int nSlaves)
    : slavePool(std::vector<std::thread>(nSlaves)), needShutdown(false) {
  initSlavePool();
  std::cout << "master CTOR\n";
}

Master::~Master() {
  // destroy all slaves here
  shutdown();
  std::cout << "master DTOR\n";
}

void Master::initSlavePool() {
  std::cout << "init " << slavePool.size() << " slaves\n";
  for (int i = 0; i < slavePool.size(); ++i) {
    slavePool[i] = std::thread(Slave(*this, i));
  }
}

// wait for all Slaves to join
void Master::shutdown() {
  needShutdown = true;
  wakeupCondition.notify_all();

  for (auto& slave : slavePool) {
    if (slave.joinable()) {
      slave.join();
    }
  }
}

bool Master::needToShutdown() const {
  return needShutdown;
}

TaskQueue<std::function<void()>>& Master::getTaskQueue() {
  return taskQueue;
}

std::mutex& Master::getMutex() {
  return mutex;
}

std::condition_variable& Master::getWakeupCondition() {
  return wakeupCondition;
}