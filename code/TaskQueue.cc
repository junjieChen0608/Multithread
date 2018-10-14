#include "TaskQueue.h"

TaskQueue::TaskQueue() {}
TaskQueue::~TaskQueue() {}

bool TaskQueue::empty() {
  std::lock_guard<std::mutex> lock(internalMutex);
  return internalQ.empty();
}

size_t TaskQueue::size() {
  std::lock_guard<std::mutex> lock(internalMutex);
  return internalQ.size();
}

void TaskQueue::enqueue(const T& task) {
  std::lock_guard<std::mutex> lock(internalMutex);
  internalQ.push(task);
}

bool TaskQueue::dequeue(T& outputTask) {
  std::lock_guard<std::mutex> lock(internalMutex);

  if (internalQ.empty()) {
    return false;
  }

  outputTask = std::move(internalQ.front());
  internalQ.pop();

  return true;
}