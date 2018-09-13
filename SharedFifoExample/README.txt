A very rudimentary example on how to combine a LV VI with the C or Python API
==============================================================================

Maximilian Matthe, Jan 17th, 2018

This example loads a LV bitfile in LV and initializes its controls. Then, a pair of FIFOs in the FPGA
is read and written by either C-API Hostcode or Python Hostcode. This way, LV and C/Python can run 
coexistently on the same USRP.

Instructions:

Compiling C program: 
  Use CMake to generate your build files. In main.cpp, adapt the path to the bitfile 
    (it must match exactly the file that is loaded in LV)
	
	
Python side: Download/clone the NIFPGA python API from https://github.com/ni/nifpga-python
   In addition, you need to have "future" installed (e.g. pip install future)
   

Running the program:
1. Start LabView side
  - Open the LV project SharedFifos.lvproject
  - adapt the RIO address
  - Run HostMain.gvi. The numbers on the panel should change every few milliseconds
  
2. start either C or python code. 
  These programs sends data into the H2T_2 FIFO, and read back the result from T2H_2 . 
   The output is the product of the input with the HostParam control that is adapted in LV HostMain.gvi while running.
   
   

