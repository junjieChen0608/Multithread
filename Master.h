#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include <iostream>

#include "TaskQueue.h"

class Master {
private:
  class Slave {
  private:
    int slaveId_;
    Master* master_;

  public:
    Slave(Master* master, const int id)
      : master_(master), slaveId_(id) {
      std::cout << "slave CTOR\n";
    }

    ~Slave() {
      std::cout << "slave DTOR\n";
    }

    void operator()() {
      std::function<void()> func;
      bool jobAllocated = false;

      while (!master_->needShutdown) {
        {
          std::unique_lock<std::mutex> lock(master_->mutex);
          if (master_->taskQueue.empty()) {
            std::cout << "slave " << slaveId_ << " is sleeping...\n";
            master_->wakeupCondition.wait(lock);
          }
          jobAllocated = master_->taskQueue.dequeue(func);
        }

        if (jobAllocated) {
          // TODO need to mark this slave as busy
          std::cout << "slave " << slaveId_ << " is busy\n";
          func();
          // TODO need to modify available slave here
          std::cout << "slave " << slaveId_ << " is available\n";
        }
      }

    }
  };

  bool needShutdown;
  TaskQueue<std::function<void()>> taskQueue;
  std::vector<std::thread> slavePool;
  std::mutex mutex;
  std::condition_variable wakeupCondition;

public:
  Master(const int nSlaves)
    : slavePool(std::vector<std::thread>(nSlaves)), needShutdown(false) {
      initSlavePool();
      std::cout << "master CTOR\n";
    }

  ~Master() {
    // destroy all slaves here
    shutdown();
    std::cout << "master DTOR\n";
  }

  // disable copy ctor
  Master(const Master& another) = delete;
  Master(Master&& another) = delete;

  // disable assignment operator
  Master& operator=(const Master& another) = delete;
  Master& operator=(Master&& another) = delete;

  void initSlavePool() {
    std::cout << "init " << slavePool.size() << " slaves\n";
    for (int i = 0; i < slavePool.size(); ++i) {
      slavePool[i] = std::thread(Slave(this, i));
    }
  }

  // wait for all Slaves to join
  void shutdown() {
    needShutdown = true;
    wakeupCondition.notify_all();

    for (auto& slave : slavePool) {
      if (slave.joinable()) {
        slave.join();
      }
    }
  }

  template<typename F, typename... Args>
  auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
    // bind given function with given parameters
    std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f),
                                                           std::forward<Args>(args)...);

    // encapsulate it in a shared ptr in order to be able to copy construct / assign
    auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

    // wrap packaged task into void function
    std::function<void()> wrapperFunc = [taskPtr]() {
      (*taskPtr)();
    };

    // put this task in queue
    taskQueue.enqueue(wrapperFunc);

    // wake up one available slave
    wakeupCondition.notify_one();

    // return future from promise
    return taskPtr->get_future();

  }
};