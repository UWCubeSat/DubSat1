from PyDAQmx import *
import numpy
import statistics as stat

analog_input = Task()
read = int32()
data = numpy.zeros((1000,), dtype=numpy.float64)

# DAQmx Configure Code
analog_input.CreateAIVoltageChan("Dev1/ai0","",DAQmx_Val_Cfg_Default,-10.0,10.0,DAQmx_Val_Volts,None)
analog_input.CfgSampClkTiming("",10000.0,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,1000)

# DAQmx Start Code
analog_input.StartTask()

# DAQmx Read Code
analog_input.ReadAnalogF64(1000,10.0,DAQmx_Val_GroupByChannel,data,1000,byref(read),None)

print("Acquired %d points"%read.value)
print(data)
print("Average value:  %f" % stat.mean(data))