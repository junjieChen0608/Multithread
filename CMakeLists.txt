cmake_minimum_required(VERSION 3.5)
project(Multithread)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -pthread")

add_executable(Multithread main.cc TaskQueue.h Master.h Slave.h Master.cc Slave.cc)