#ifndef MULTITHREAD_MASTER_H
#define MULTITHREAD_MASTER_H

#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include <iostream>

#include "Slave.h"
#include "TaskQueue.h"

class Slave;

class Master {
private:
  bool needShutdown;
  TaskQueue<std::function<void()>> taskQueue;
  std::vector<std::thread> slavePool;
  std::mutex mutex;
  std::condition_variable wakeupCondition;

public:
  Master(int nSlaves);
  ~Master();

  // disable copy ctor
  Master(const Master& another) = delete;
  Master(Master&& another) = delete;

  // disable move assignment operator
  Master& operator=(const Master& another) = delete;
  Master& operator=(Master&& another) = delete;

  void initSlavePool();

  // wait for all Slaves to join
  void shutdown();

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

  bool needToShutdown() const;

  TaskQueue<std::function<void()>>& getTaskQueue();

  std::mutex& getMutex();

  std::condition_variable& getWakeupCondition();
};

#endif //MULTITHREAD_MASTER_H

