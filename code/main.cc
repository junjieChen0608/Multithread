/*
 * The client comes in and Submit functions to the Master.
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
 * Master will destroy all Slaves when it is out of scope, or user can explicitly call Shutdown() function
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


void SimulateHardComputation() {
  std::cout << "thread " << std::this_thread::get_id() << " working\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000 + rnd()));
}

// Simple function that adds multiplies two numbers and prints the result
void Multiply(const int a, const int b) {
  SimulateHardComputation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
}

// Same as before but now we have an output parameter
void MultiplyOutput(int &out, const int a, const int b) {
  SimulateHardComputation();
  out = a * b;
  std::cout << a << " * " << b << " = " << out << std::endl;
}

// Same as before but now we have a return value
int MultiplyReturn(const int a, const int b) {
  SimulateHardComputation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
  return res;
}

// modify parameter reference
void Swap(int &a, int &b) {
  int c = a;
  a = b;
  b = c;
}

// modify parameter pointer
void SwapPtr(int *a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}

void ConstRefFunction(const std::string &name, const int &age) {
  std::cout << name << " is " << age << " years old\n";
}

int main() {
  constexpr int N_THREAD = 4;
  Master master(N_THREAD);

  for (int i = 1; i < 3; ++i) {
    for (int j = 1; j < 3; ++j) {
      master.SubmitTask(Multiply, i, j);
    }
  }

  int outputRef;
  auto future1 = master.SubmitTask(MultiplyOutput, std::ref(outputRef), 1, 999);

  auto future2 = master.SubmitTask(MultiplyReturn, 20, 30);

  int a = 10;
  int b = 100;
  auto future3 = master.SubmitTask(Swap, std::ref(a), std::ref(b));

  int c = 999;
  int d = 0;
  auto future4 = master.SubmitTask(SwapPtr, &c, &d);

  future1.get();
  std::cout << "Last operation result is " << outputRef << "\n";

  int res = future2.get();
  std::cout << "Last operation result is " << res << "\n";

  future3.get();
  std::cout << "after swap a: " << a << ", b: " << b << "\n";

  future4.get();
  std::cout << "after swap c: " << c << ", d: " << d << "\n";

  std::string name = "John Doe";
  int age = 99;
  auto future5 = master.SubmitTask(ConstRefFunction, std::cref(name), std::cref(age));
  future5.get();

  return 0;
}