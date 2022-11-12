from sqlite3 import Time
import psutil
from pynvml import *
import serial
import time


while True:
    try:
        ser = serial.Serial(
        port="COM4", 
        baudrate=9600
        )
        print("opened " + ser.name)

        #use ram percent instead
        while True:
            try:
                nvmlInit()
                handle = nvmlDeviceGetHandleByIndex(0)

                #gpu
                GpuUsage = nvmlDeviceGetUtilizationRates(handle)

                #vram
                GpuMemory = nvmlDeviceGetMemoryInfo(handle)
                TotalGpuMemory = GpuMemory.total / 1073741824
                TotalGpuMemoryUsage = GpuMemory.used / 1073741824

            except ValueError:
                print(ValueError)

            #cpu
            CPU_USAGE = psutil.cpu_percent(0.1)


             

            #ram
            Memory = psutil.virtual_memory()
            TotalMemory = int((Memory.total / 1073741824) * 10) / 10
            UsedMemory = int((Memory.used / 1073741824) * 10) / 10


            OutputValue  = str(GpuUsage.gpu)  + "," + str(int(TotalGpuMemoryUsage * 10)/10)  + "," + str(int(TotalGpuMemory * 10)/10)  + "," + str(CPU_USAGE)  + "," + str(UsedMemory)  + "," + str(TotalMemory) + ","
            ser.write(OutputValue.encode())
            
            time.sleep(0.9)

    except:
        pass
    time.sleep(10)
