#pragma once

#include <mutex>
#include <queue>

template<typename T>
class TaskQueue {
private:
  std::queue<T> internal_q;
  std::mutex internal_mutex;

public:
  TaskQueue() {}
  ~TaskQueue() {}

  bool empty() {
    std::unique_lock<std::mutex> lock(internal_mutex);
    return internal_q.empty();
  }

  size_t size() {
    std::unique_lock<std::mutex> lock(internal_mutex);
    return internal_q.size();
  }

  void enqueue(T& task) {
    std::unique_lock<std::mutex> lock(internal_mutex);
    internal_q.push(task);
  }

  bool dequeue(T& task) {
    std::unique_lock<std::mutex> lock(internal_mutex);

    if (internal_q.empty()) {
      return false;
    }

    task = std::move(internal_q.front());
    internal_q.pop();

    return true;
  }
};