#include <iostream>
#include <thread>

#include "NiFpga.h"
#include "NiFpga_Toplevel.h"

#define str(x) #x
#define CheckCall(call) if((status = call) != 0) throw std::runtime_error("Error during call to " ## str(call))

int main()
{
	try {
		NiFpga_Session session;

		uint32_t T2H = NiFpga_Toplevel_TargetToHostFifoI32_ResourcesgrscT2H_2;
		uint32_t H2T = NiFpga_Toplevel_HostToTargetFifoI32_ResourcesgrscH2T_2;



		const char* path_to_bitfile = "C:\\Users\\admin\\Documents\\LabVIEW Projects\\SharedFIFOs\\Builds\\USRP-SharedFifos-0001.lvbitx";

		NiFpga_Status status;
		CheckCall(NiFpga_Initialize());
		std::cout << status << std::endl;

		CheckCall(NiFpga_Open(path_to_bitfile, NiFpga_Toplevel_Signature, "RIO0", NiFpga_OpenAttribute_NoRun, &session));

		CheckCall(NiFpga_ConfigureFifo(session, T2H, 10000));
		CheckCall(NiFpga_StartFifo(session, T2H));

		CheckCall(NiFpga_ConfigureFifo(session, H2T, 10000));
		CheckCall(NiFpga_StartFifo(session, H2T));

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		while(true)
		{
			int32_t input[5] = { 1,2,3,4,5 }, output[5] = { 0 };
			NiFpga_WriteFifoI32(session, H2T, input, 5, 100, NULL);
			NiFpga_ReadFifoI32(session, T2H, output, 5, 100, NULL);

			for (int j = 0; j < 5; j++)
				std::cout << input[j] << " -> " << output[j] << "\n";
			std::cout << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		CheckCall(NiFpga_StopFifo(session, T2H));
		CheckCall(NiFpga_StopFifo(session, H2T));
		CheckCall(NiFpga_Close(session, 0));

		return 0;
	}
	catch (std::runtime_error e)
	{
		std::cout << "ERROR! " << e.what() << std::endl;
		return 1;
	}


}
