cmake_minimum_required(VERSION 3.10)
project(nifpga-prototype)

SET(nifpga_src "../src/nifpga-cpp.hpp" "../src/NiFpga.c" "../src/NiFpga.c")

add_executable(prototype main.cpp ${nifpga_src})
target_include_directories(prototype PUBLIC "../src/")
if(WIN32)
else(WIN32)
	target_link_libraries(prototype dl)
endif()
