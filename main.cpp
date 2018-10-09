/*
 * The client comes in and submit Tasks to the Master.
 * Master is going to spawn N slaves, initialize all necessary flags and variables, then all go to sleep.
 * Master maintains a queue of Tasks, and a Slave pool internally.
 * Master is wake up by incoming Tasks via condition_variable, then master will allocate available Slave to handle Tasks
 * Once a Slave finished its Task, it will check if there is any work to be done, if YES, do more work, ELSE Slave sleep
 *
 * Master will destroy all Slaves when it is out of scope, or user explicitly call shutdown() function
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


void simulateHardComputation() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000 + rnd()));
}

// Simple function that adds multiplies two numbers and prints the result
void multiply(const int a, const int b) {
  simulateHardComputation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
}

// Same as before but now we have an output parameter
void multiplyOutput(int &out, const int a, const int b) {
  simulateHardComputation();
  out = a * b;
  std::cout << a << " * " << b << " = " << out << std::endl;
}

// Same as before but now we have an output parameter
int multiplyReturn(const int a, const int b) {
  simulateHardComputation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
  return res;
}

void swap(int& a, int& b) {
  int c = a;
  a = b;
  b = c;
}

void swapPtr(int *a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}

int main() {
  Master master(4);

  for (int i = 1; i < 3; ++i) {
    for (int j = 1; j < 3; ++j) {
      master.submit(multiply, i, j);
    }
  }

  int outputRef;
  auto future1 = master.submit(multiplyOutput, std::ref(outputRef), 1, 999);

  auto future2 = master.submit(multiplyReturn, 20, 30);

  int a = 10;
  int b = 100;
  auto future3 = master.submit(swap, std::ref(a), std::ref(b));

  int c = 999;
  int d = 0;
  auto future4 = master.submit(swapPtr, &c, &d);

  future1.get();
  std::cout << "Last operation result is " << outputRef << "\n";

  int res = future2.get();
  std::cout << "Last operation result is " << res << "\n";

  future3.get();
  std::cout << "after swap a: " << a << ", b: " << b << "\n";

  future4.get();
  std::cout << "after swap c: " << c << ", d: " << d << "\n";

  return 0;
}