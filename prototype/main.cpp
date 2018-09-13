#include "nifpga-cpp.hpp"

nifpga::Register<uint8_t, nifpga::control_tag> U8Reg_control(5, "bla");
nifpga::Register<uint8_t, nifpga::indicator_tag> U8Reg_indicator(5, "bla");
nifpga::Register<float, nifpga::control_tag> FloatReg_control(5, "bla");
nifpga::Register<float, nifpga::indicator_tag> FloatReg_indicator(5, "bla");

nifpga::Fifo<uint8_t, nifpga::host_to_target_tag> U8Fifo_h2t(5, "bla");
nifpga::Fifo<uint32_t, nifpga::target_to_host_tag> U8Fifo_t2h(5, "bla");

int main(int argc, char *argv[])
{
  uint8_t u8 = nifpga::readRegister(0, U8Reg_indicator);
  uint32_t u32;
  // float ug = nifpga::readRegister(0, FloatReg_indicator);


  nifpga::writeFifo(0, U8Fifo_h2t, &u8, 0, 5, NULL);
  nifpga::readFifo(0, U8Fifo_t2h, &u32, 0, 5, NULL);

  nifpga::writeRegister(0, U8Reg_control, (unsigned char)1);
  return 0;
}
