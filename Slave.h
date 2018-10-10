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
  int slaveId_;
  Master& master_;

public:
  Slave(Master& master, const int id);
  ~Slave();

  void operator()();
};