cmake_minimum_required(VERSION 3.10)

project(LFU)

add_executable(main main.cpp)

set(INCLUDE_DIR ./include/)

message(STATUS "Building project...")

target_include_directories(main PUBLIC "${INCLUDE_DIR}")

