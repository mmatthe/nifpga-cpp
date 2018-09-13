/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_Toplevel_h__
#define __NiFpga_Toplevel_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Toplevel_Bitfile;
 */
#define NiFpga_Toplevel_Bitfile "NiFpga_Toplevel.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Toplevel_Signature = "17DA30D372F2D83B3B25F31E158DC86B";

typedef enum
{
   NiFpga_Toplevel_ControlI32_HostParam = 0x0,
} NiFpga_Toplevel_ControlI32;

typedef enum
{
   NiFpga_Toplevel_TargetToHostFifoI32_ResourcesgrscT2H_1 = 3,
   NiFpga_Toplevel_TargetToHostFifoI32_ResourcesgrscT2H_2 = 0,
} NiFpga_Toplevel_TargetToHostFifoI32;

typedef enum
{
   NiFpga_Toplevel_HostToTargetFifoI32_ResourcesgrscH2T_1 = 2,
   NiFpga_Toplevel_HostToTargetFifoI32_ResourcesgrscH2T_2 = 1,
} NiFpga_Toplevel_HostToTargetFifoI32;

#endif
