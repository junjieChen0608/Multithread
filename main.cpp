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

#include "TaskQueue.h"

int main() {

  return 0;
}