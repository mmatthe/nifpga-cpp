cmake_minimum_required(VERSION 3.10)
project(nifpga-prototype)

SET(nifpga_src "../nifpga-cpp/nifpga-cpp.hpp" "../nifpga-cpp/NiFpga.c" "../nifpga-cpp/NiFpga.c")

add_executable(prototype main.cpp ${nifpga_src})
target_include_directories(prototype PUBLIC "../nifpga-cpp/")
if(WIN32)
else(WIN32)
	target_link_libraries(prototype dl)
endif()
