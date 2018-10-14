#ifndef MULTITHREAD_TASKQUEUE_H
#define MULTITHREAD_TASKQUEUE_H

#pragma once

#include <mutex>
#include <queue>

template<typename T>
class TaskQueue {
private:
  std::queue<T> internalQ;
  std::mutex internalMutex;

public:
  TaskQueue();
  ~TaskQueue();

  bool empty();

  size_t size();

  void enqueue(const T& task);

  bool dequeue(T& outputTask);
};

#endif //MULTITHREAD_TASKQUEUE_H

