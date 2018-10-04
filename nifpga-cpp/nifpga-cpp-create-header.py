#!/usr/bin/env python

from __future__ import print_function

import argparse
from io import BytesIO as StringIO
import os, sys
import datetime

import nifpga



def export_bitfile(stream, filepath, signature):
    filepath = os.path.basename(filepath)
    text = '''

// Bitfile
const char* bitfile_filepath  = "%s";
const char* bitfile_signature = "%s";

''' % (filepath, signature)
    print(text, file=stream)

data_type_mapping = {}
for w in [8,16,32,64]:
    data_type_mapping["U%d" % w] = "uint%d_t" % w
    data_type_mapping["I%d" % w] = "int%d_t" % w
data_type_mapping["Bool"] = "bool"
data_type_mapping["Cluster"] = None

def create_cppsafe_name(name):
    patterns = [(" ", ""),
                ("-", "_"),
                (".", "_"),
                (",", "_"),
                ("\\", "_"),
                ("/", "_"),
                ("(", "_"),
                (")", "_"),
                ("[", "_"),
                ("]", "_")]
    for pat, to in patterns:
        name = name.replace(pat, to)
    return name


def export_register(stream, bitfile, register):
    if register.is_internal():
        return

    name = register.name
    safename = create_cppsafe_name(name)
    name = name.replace("\\", "\\\\")
    datatype = data_type_mapping[register.datatype.name]
    if datatype is not None:
        address = bitfile.base_address_on_device() + register.offset
        text = 'nifpga::Register<%s> reg_%s(%s, "%s");' % (datatype, safename, address, name)
        print (text, file=stream)

def extractLeafFifoName(fifoname):
    pos = fifoname.rfind("\\")
    if pos == -1 or pos + 1 == len(fifoname):
        return fifoname
    else:
        return fifoname[pos+1:]


def export_fifo(stream, bitfile, fifo):
    number = fifo.number
    name = fifo.name
    name = extractLeafFifoName(fifo.name)
    safename = create_cppsafe_name(name)
    datatype = data_type_mapping[fifo.datatype.name]

    text = 'nifpga::Fifo<%s> fifo_%s(%s, "%s");' % (datatype, safename, number, name)
    print (text, file=stream)




def main():
    parser = argparse.ArgumentParser(description="This script creates a C++ header file for usage with the NIFPGA C-API")
    parser.add_argument("--bitfile", required=True)

    args = parser.parse_args()

    bitfile_name = args.bitfile
    bitfile = nifpga.Bitfile(bitfile_name)

    filepath = bitfile.filepath
    signature = bitfile.signature

    finaloutput = StringIO()
    print("// Automatically generated at %s" % datetime.datetime.now(), file=finaloutput)

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





if __name__ == '__main__':
    main()
