cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS "-Wall")

set(INCLUDE_DIR ./include/)
set(SRC_DIR ./src/)

project(Cache_)

add_executable(LFU_hashmap)
add_executable(LFU_slow)
add_executable(perfect_cache)

message(STATUS "Building project...")

target_sources(LFU_hashmap PRIVATE "${SRC_DIR}/LFU_hashmap.cpp")
target_include_directories(LFU_hashmap PUBLIC "${INCLUDE_DIR}")

target_sources(LFU_slow PRIVATE "${SRC_DIR}/LFU_slow.cpp")
target_include_directories(LFU_slow PUBLIC "${INCLUDE_DIR}")

target_sources(perfect_cache PRIVATE "${SRC_DIR}/perfect_cache.cpp")
target_include_directories(perfect_cache PUBLIC "${INCLUDE_DIR}")

