#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include <iostream>

#include "code/master.h"
#include "code/task_queue.h"

class Master;

class Slave {
 private:
  int slave_id_;
  Master* master_;

 public:
  Slave(Master* master, const int& id);
  ~Slave();

  void operator()();
};
