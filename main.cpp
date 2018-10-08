/*
 * The client comes in and submit Tasks to the Master.
 * Master is going to spawn N slaves, initialize all necessary flags and variables, then all go to sleep.
 * Master maintains a queue of Tasks, and a Slave pool internally.
 * Master is wake up by incoming Tasks via condition_variable, then master will allocate available Slave to handle Tasks
 * Once a Slave finished its Task, it will notify the Master and return to Slave pool
 * Master will go back to sleep once the Task queue is empty
 */

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>

#include "Master.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(-1000, 1000);
auto rnd = std::bind(dist, mt);


void simulate_hard_computation() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}

// Simple function that adds multiplies two numbers and prints the result
void multiply(const int a, const int b) {
  simulate_hard_computation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
}

// Same as before but now we have an output parameter
void multiply_output(int & out, const int a, const int b) {
  simulate_hard_computation();
  out = a * b;
  std::cout << a << " * " << b << " = " << out << std::endl;
}

// Same as before but now we have an output parameter
int multiply_return(const int a, const int b) {
  simulate_hard_computation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
  return res;
}

int main() {
  Master master(4);

  for (int i = 1; i < 3; ++i) {
    for (int j = 1; j < 3; ++j) {
      master.submit(multiply, i, j);
    }
  }

  int outputRef;
  auto future1 = master.submit(multiply_output, std::ref(outputRef), 1, 999);
  future1.get();
  std::cout << "Last operation result is " << outputRef << "\n";

  auto future2 = master.submit(multiply_return, 20, 30);
  int res = future2.get();
  std::cout << "Last operation result is " << res << "\n";

  return 0;
}