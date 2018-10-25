cmake_minimum_required(VERSION 3.5)
project(Multithread)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -pthread")
find_package(PCL 1.5 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(Multithread main.cc task_queue.h master.h slave.h task_queue.cc master.cc slave.cc)
target_link_libraries(Multithread ${PCL_LIBRARIES})