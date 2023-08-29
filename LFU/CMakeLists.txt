cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS "-Wall")

project(LFU)

add_executable(LFU_cringe LFU_cringe.cpp)
add_executable(LFU_hashmap LFU_hashmap.cpp)
add_executable(perfect_cache perfect_cache.cpp)

set(INCLUDE_DIR ./include/)

message(STATUS "Building project...")

target_include_directories(LFU_cringe PUBLIC "${INCLUDE_DIR}")
target_include_directories(LFU_hashmap PUBLIC "${INCLUDE_DIR}")
target_include_directories(perfect_cache PUBLIC "${INCLUDE_DIR}")

