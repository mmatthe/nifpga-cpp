cmake_minimum_required (VERSION 2.6)
project (CAPISharedFifo)
add_executable(CAPISharedFifo main.cpp NiFpga.c NiFpga.h NiFpga_Toplevel.h)