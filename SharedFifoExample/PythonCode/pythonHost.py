import sys
sys.path.append(r"C:\Users\admin\Documents\Python\nifpga-python")

import time

bitfile = "C:\\Users\\admin\\Documents\\LabVIEW Projects\\SharedFIFOs\\Builds\\USRP-SharedFifos-0001.lvbitx"
import nifpga

with nifpga.Session(bitfile, "RIO0") as session:
	H2T = session.fifos['Resources.grsc\\H2T_2']
	T2H = session.fifos['Resources.grsc\\T2H_2']
	input = [1,2,3,4,5]
	
	for i in range(10):
		H2T.write(input)
		print (T2H.read(5).data)
		time.sleep(1)
