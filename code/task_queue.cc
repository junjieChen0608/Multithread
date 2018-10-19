#include "code/task_queue.h"

TaskQueue::TaskQueue() {}
TaskQueue::~TaskQueue() {}

bool TaskQueue::empty() {
  std::lock_guard<std::mutex> lock(internal_mutex_);
  return internal_queue_.empty();
}

size_t TaskQueue::size() {
  std::lock_guard<std::mutex> lock(internal_mutex_);
  return internal_queue_.size();
}

void TaskQueue::enqueue(std::unique_ptr<std::function<void()>> task) {
  std::lock_guard<std::mutex> lock(internal_mutex_);
  internal_queue_.push(std::move(task));
}

std::unique_ptr<std::function<void()>> TaskQueue::dequeue() {
  std::lock_guard<std::mutex> lock(internal_mutex_);

  if (internal_queue_.empty()) {
    return nullptr;
  }

  std::unique_ptr<std::function<void()>> output_task (std::move(internal_queue_.front()));
  internal_queue_.pop();

  return output_task;
}