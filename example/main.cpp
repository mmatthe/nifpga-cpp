#include <iostream>

#include "nifpga-cpp.hpp"

#include "bitfile.hpp"

int main(int argc, char *argv[])
{
	

	try {
		nifpga::initialize();

			
		NiFpga_Session session = nifpga::open(bitfile_filepath, bitfile_signature, "RIO0", 0);
		//nifpga::download(session);
		//nifpga::run(session, 0);

		nifpga::writeRegister(session, reg_I32in, 15);

		for (int i = 0; i < 100; i++)
			std::cout << nifpga::readRegister(session, reg_I32out);	
			
		

		nifpga::close(session, NiFpga_CloseAttribute_NoResetIfLastSession);	
	}
	catch (nifpga::fpga_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");		
  return 0;
}
