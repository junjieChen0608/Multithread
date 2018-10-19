#pragma once

#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "code/slave.h"
#include "code/task_queue.h"

    class Slave;

class Master {
 private:
  bool need_shut_down_;
  TaskQueue<std::function<void()>> task_queue_;
  std::vector<std::thread> slave_pool_;
  std::mutex mutex_;
  std::condition_variable wakeup_condition_;

 public:
  explicit Master(int n_slaves);
  ~Master();

  // disable copy ctor
  Master(const Master& another) = delete;
  Master(Master&& another) = delete;

  // disable move assignment operator
  Master& operator=(const Master& another) = delete;
  Master& operator=(Master&& another) = delete;

  void InitSlavePool();

  // wait for all Slaves to join
  void Shutdown();

  template <typename F, typename... Args>
  auto SubmitTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
    // bind given function with given parameters
    std::function<decltype(f(args...))()> task =
        std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    // encapsulate it in a shared ptr to copy construct and assign
    auto task_ptr =
        std::make_shared<std::packaged_task<decltype(f(args...))()>>(task);

    // wrap packaged task into void function
    std::function<void()> wrapper_func = [task_ptr]() { (*task_ptr)(); };

    // put this task in queue
    task_queue_.enqueue(wrapper_func);

    // wake up one available slave
    wakeup_condition_.notify_one();

    // return future from promise
    return task_ptr->get_future();
  }

  bool NeedToShutdown() const;

  TaskQueue<std::function<void()>>& GetTaskQueue();

  std::mutex& GetMutex();

  std::condition_variable& GetWakeupCondition();
};
