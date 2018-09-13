cmake_minimum_required(VERSION 3.10)
project(nifpga-prototype)


add_executable(prototype main.cpp nifpga-cpp.hpp NiFpga.c NiFpga.h)
target_link_libraries(prototype dl)
