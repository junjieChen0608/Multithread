#ifndef MULTITHREAD_TASKQUEUE_H
#define MULTITHREAD_TASKQUEUE_H

#include <mutex>
#include <queue>

template<typename T>
class TaskQueue {
private:
  std::queue<T> internal_queue_;
  std::mutex internal_mutex_;

public:
  TaskQueue() {}
  ~TaskQueue() {}

  bool empty() {
    std::lock_guard<std::mutex> lock(internal_mutex_);
    return internal_queue_.empty();
  }

  size_t size() {
    std::lock_guard<std::mutex> lock(internal_mutex_);
    return internal_queue_.size();
  }

  void enqueue(const T& task) {
    std::lock_guard<std::mutex> lock(internal_mutex_);
    internal_queue_.push(task);
  }

  bool dequeue(T& outputTask) {
    std::lock_guard<std::mutex> lock(internal_mutex_);

    if (internal_queue_.empty()) {
      return false;
    }

    outputTask = std::move(internal_queue_.front());
    internal_queue_.pop();

    return true;
  }
};

#endif //MULTITHREAD_TASKQUEUE_H

