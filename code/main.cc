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
 * Master will destroy all Slaves when it is output_parameter of scope, or user can explicitly call Shutdown() function
 */

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <vector>
#include <algorithm>

#include "code/master.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(-1000, 1000);
auto rnd = std::bind(dist, mt);


void SimulateHardComputation() {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000 + rnd()));
}

// Simple function that adds multiplies two numbers and prints the result
void MultiplyPrint(const int& a, const int& b) {
  SimulateHardComputation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
}

// Same as before but now we have an output parameter
void MultiplyOutput(const double& a, const double& b,
                    double* output_parameter) {
  SimulateHardComputation();
  *output_parameter = a * b;
  std::cout << a << " * " << b << " = " << *output_parameter << std::endl;
}

// Same as before but now we have a return value
int MultiplyReturn(const int& a, const int& b) {
  SimulateHardComputation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
  return res;
}

// modify parameter reference
void Swap(int& a, int& b) {
  int c = a;
  a = b;
  b = c;
}

// modify parameter pointer
void SwapPtr(int* a, int* b) {
  int c = *a;
  *a = *b;
  *b = c;
}

int main() {
  constexpr int kNumThread = 4;
  Master master(kNumThread);

  /********** submit tasks first **********/
//  int num_one = 90;
//  int num_two = 100;
//  auto future_multiply_print = master.SubmitTask(MultiplyPrint, std::cref(num_one), std::cref(num_two));
//  future_multiply_print.get();

  double output_parameter = 0.0;
  double input_double_one = 11.11;
  double input_double_two = 888.888;
  auto future_output_ref = master.SubmitTask(MultiplyOutput,
                                             std::cref(input_double_one),
                                             std::cref(input_double_two),
                                             &output_parameter);
//
//  int input_int_one = 20;
//  int input_int_two = 30;
//  auto future_multiply_return = master.SubmitTask(MultiplyReturn,
//                                                  std::cref(input_int_one),
//                                                  std::cref(input_int_two));

  /********** get results later **********/
  future_output_ref.get();
  std::cout << "MultiplyOutput result is " << output_parameter << "\n";
//
//  int output_return = future_multiply_return.get();
//  std::cout << "MultiplyReturn result is " << output_return << "\n";
//
//  int a = 10;
//  int b = 100;
//  std::cout << "before swap a " << a << ", b: " << b << "\n";
//  auto future_swap = master.SubmitTask(Swap, std::ref(a), std::ref(b));
//  future_swap.get();
//  std::cout << "after swap a: " << a << ", b: " << b << "\n";
//
//  int c = 999;
//  int d = 0;
//  std::cout << "before swap c: " << c << ", d: " << d << "\n";
//  auto future_swap_ptr = master.SubmitTask(SwapPtr, &c, &d);
//  future_swap_ptr.get();
//  std::cout << "after swap c: " << c << ", d: " << d << "\n";

  return 0;
}
