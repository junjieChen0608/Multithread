#ifndef MULTITHREAD_SLAVE_H
#define MULTITHREAD_SLAVE_H

#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include <iostream>

#include "Master.h"
#include "TaskQueue.h"

class Master;

class Slave {
private:
  int slave_id_;
  Master& master_;

public:
  Slave(Master& master, const int& id);
  ~Slave();

  void operator()();
};

#endif //MULTITHREAD_SLAVE_H