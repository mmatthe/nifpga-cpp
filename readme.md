# nifpga-cpp - A C++ binding for the [nifpga C-API](http://www.ni.com/product-documentation/9036/en/)

- requires python and the [nifpga-python bindings](https://nifpga-python.readthedocs.io/en/latest/)
- Use nifpga-cpp/nifpga-cpp-create-header.py script to extract the fifo and register information from a bitfile and print it to stdout. Copy the output to a header file which you use in your project to access the FPGA.
- Check example/main.cpp for an example how to use the API.