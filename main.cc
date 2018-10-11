/*
 * The client comes in and submit functions to the Master.
 *
 * Master will spawn N slaves upon construction, initializing all necessary flags and variables
 * then all Slaves go to sleep.
 *
 * Master maintains a queue of functions, and a Slave pool internally.
 *
 * Master is waked up by incoming functions via condition_variable
 * then Master will allocate functions to available Slave
 *
 * Once a Slave finished its job, it will check if there is any work to be done, if YES, do more work, ELSE Slave sleep
 *
 * Master will destroy all Slaves when it is out of scope, or user can explicitly call shutdown() function
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
  std::cout << "thread " << std::this_thread::get_id() << " working\n";
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

  for (int i = 3; i < 6; ++i) {
    for (int j = 3; j < 6; ++j) {
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