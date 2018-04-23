from PyDAQmx import *
import numpy
import statistics as stat
from PyDAQmx.DAQmxConstants import *
from PyDAQmx.DAQmxFunctions import *
from PyDAQmx.DAQmxConfig import *
from PyDAQmx.DAQmxTypes import *

ai0 = Task()
ai1 = Task()
#ai2 = Task()

# Location to store number of bytes read
ai0_readbytes = int32()
ai1_readbytes = int32()
#ai2_readbytes = int32()

# Read buffers
ai0_data = numpy.zeros((1,), dtype=numpy.float64)
ai1_data = numpy.zeros((1,), dtype=numpy.float64)
#ai2_data = numpy.zeros((1,), dtype=numpy.float64)

ai0.CreateAIVoltageChan("Dev1/ai0", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, None)
ai1.CreateAIVoltageChan("Dev1/ai1", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, None)
#ai2.CreateAIVoltageChan("Dev1/ai2", "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, None)

ai0.StartTask()
ai1.StartTask()
#ai2.StartTask()

while True:
    #ai0.ReadAnalogF64(1, 10.0, DAQmx_Val_GroupByChannel, ai0_data, 1, byref(ai0_readbytes), None)
    ai1.ReadAnalogF64(1, 10.0, DAQmx_Val_GroupByChannel, ai1_data, 1, byref(ai1_readbytes), None)
    #ai2.ReadAnalogF64(1, 10.0, DAQmx_Val_GroupByChannel, ai2_data, 1, byref(ai2_readbytes), None)
    #print("Voltage:  %f/%f/%f" % (ai0_data, ai1_data, ai2_data))
    print("Voltage:  %f" % (ai1_data))


