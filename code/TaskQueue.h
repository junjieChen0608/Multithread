#ifndef MULTITHREAD_TASKQUEUE_H
#define MULTITHREAD_TASKQUEUE_H

#pragma once

#include <mutex>
#include <queue>

template<typename T>
class TaskQueue {
private:
  std::queue<T> internal_queue;
  std::mutex internal_mutex;

public:
  TaskQueue() {}
  ~TaskQueue() {}

  bool empty() {
    std::lock_guard<std::mutex> lock(internal_mutex);
    return internal_queue.empty();
  }

  size_t size() {
    std::lock_guard<std::mutex> lock(internal_mutex);
    return internal_queue.size();
  }

  void enqueue(const T& task) {
    std::lock_guard<std::mutex> lock(internal_mutex);
    internal_queue.push(task);
  }

  bool dequeue(T& outputTask) {
    std::lock_guard<std::mutex> lock(internal_mutex);

    if (internal_queue.empty()) {
      return false;
    }

    outputTask = std::move(internal_queue.front());
    internal_queue.pop();

    return true;
  }
};

#endif //MULTITHREAD_TASKQUEUE_H

