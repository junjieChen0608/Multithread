#pragma once

#include <mutex>
#include <queue>
#include <functional>
#include <memory>

class TaskQueue {
 private:
  std::queue<std::unique_ptr<std::function<void()>>> internal_queue_;
  std::mutex internal_mutex_;

 public:
  TaskQueue();
  ~TaskQueue();

  bool empty();
  size_t size();
  void enqueue(std::unique_ptr<std::function<void()>> task);
  std::unique_ptr<std::function<void()>> dequeue();
};

