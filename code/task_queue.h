#pragma once

#include <mutex>
#include <queue>
#include <memory>
#include <functional>

class TaskQueue {
 private:
  std::queue<std::function<void()>> internal_queue_;
  std::mutex internal_mutex_;

 public:
  TaskQueue();
  ~TaskQueue();

  bool empty();

  size_t size();

  void enqueue(const std::function<void()>& task);

  bool dequeue(std::function<void()>* output_task);
};

