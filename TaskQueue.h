#pragma once

#include <mutex>
#include <queue>

template<typename T>
class TaskQueue {
private:
  std::queue<T> internalQ;
  std::mutex internalMutex;

public:
  TaskQueue() {}
  ~TaskQueue() {}

  bool empty() {
    std::lock_guard<std::mutex> lock(internalMutex);
    return internalQ.empty();
  }

  size_t size() {
    std::lock_guard<std::mutex> lock(internalMutex);
    return internalQ.size();
  }

  void enqueue(T& task) {
    std::lock_guard<std::mutex> lock(internalMutex);
    internalQ.push(task);
  }

  bool dequeue(T& task) {
    std::lock_guard<std::mutex> lock(internalMutex);

    if (internalQ.empty()) {
      return false;
    }

    task = std::move(internalQ.front());
    internalQ.pop();

    return true;
  }
};