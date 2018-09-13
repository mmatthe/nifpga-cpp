#!/usr/bin/env python

import argparse
from io import StringIO
import os, sys
import datetime

import nifpga



def export_bitfile(stream, filepath, signature):
    filepath = os.path.basename(filepath)
    text = f'''

// Bitfile
const char* bitfile_filepath  = "{filepath}";
const char* bitfile_signature = "{signature}";

'''
    print(text, file=stream)

data_type_mapping = {}
for w in [8,16,32,64]:
    data_type_mapping["U%d" % w] = "uint%d_t" % w
    data_type_mapping["I%d" % w] = "int%d_t" % w

def create_cppsafe_name(name):
    patterns = [(" ", ""),
                ("-", "_"),
                (".", "_"),
                (",", "_"),
                ("\\", "_"),
                ("/", "_")]
    for pat, to in patterns:
        name = name.replace(pat, to)
    return name


def export_register(stream, bitfile, register):
    if register.is_internal():
        return

    name = register.name
    safename = create_cppsafe_name(name)
    datatype = data_type_mapping[register.datatype.name]
    address = bitfile.base_address_on_device() + register.offset
    text = f'nifpga::Register<{datatype}> reg_{safename}({address}, "{name}");'
    print (text, file=stream)

def export_fifo(stream, bitfile, fifo):
    number = fifo.number
    name = fifo.name
    safename = create_cppsafe_name(fifo.name)
    datatype = data_type_mapping[fifo.datatype.name]

    text = f'nifpga::Fifo<{datatype}> fifo_{safename}({number}, "{name}");'
    print (text, file=stream)




def main():
    parser = argparse.ArgumentParser(description="This script creates a C++ header file for usage with the NIFPGA C-API")
    parser.add_argument("--bitfile")

    args = parser.parse_args()

    bitfile_name = args.bitfile
    bitfile = nifpga.Bitfile(bitfile_name)

    filepath = bitfile.filepath
    signature = bitfile.signature

    finaloutput = StringIO()
    print("Automatically generated at %s" % datetime.datetime.now(), file=finaloutput)

    export_bitfile(finaloutput, filepath, signature)

    print("// Registers", file=finaloutput)
    for R in bitfile.registers.values():
        export_register(finaloutput, bitfile, R)

    print ("\n// FIFOS", file=finaloutput)
    for F in bitfile.fifos.values():
        export_fifo(finaloutput, bitfile, F)


    registers = bitfile.registers
    fifos = bitfile.fifos

    print (finaloutput.getvalue())


    # import ipdb; ipdb.set_trace()


if __name__ == '__main__':
    main()
