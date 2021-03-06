from typing import List, Tuple
import time
import tqdm
import sys
import os
import saleae

# States: 0: not listening for int fall, 1: listening for int fall, 2: listening for spi flip
state: int = 0
flip_state: int = 0
timer: float = 0

tmpcapturepath = '/tmp/'
if os.name == 'nt':
    tmpcapturepath = 'C:\\tmp\\'

currentmax: float = 0.
currentmaxperiod: float = 0.

times: List = []
periods: List = []

s = saleae.Saleae()

try:
    counter: int = 0
    while True:
        s.capture_to_file('/tmp/emoriessaleaedump' + str(counter) + '.logicdata')
        s.export_data2("/tmp/emoriessaleaedump.csv",
                       digital_channels=[0, 1],
                       format='csv',
                       display_base="SEPARATE",
                       column_headers=False)

        f = open("/tmp/emoriessaleaedump.csv")
        content: List[str] = f.readlines()
        content: List[str] = [x.strip() for x in content]
        content: List[List[str]] = [x.split(",") for x in content]
        content: List[Tuple[float, bool, bool]] = [[float(x[0]), bool(int(x[1])), bool(int(x[2]))] for x in content]

        state = 0
        # timer = 0.
        counter += 1

        for x in content:
            if state == 0:
                if x[1]:
                    state = 1
            if state == 1:
                if not x[1]:
                    state = 2
                    timer = x[0]
            if state == 2:
                if x[1]:
                    state = 1
                    times.append(x[0] - timer)
                    if x[0] - timer > currentmax:
                        print(" New Max Service Time: " + str(x[0] - timer))
                        currentmax = x[0] - timer

        # for x in content:
        #     if state == 0:
        #         if x[1]:
        #             state = 1
        #     if state == 1:
        #         if not x[1]:
        #             state = 2
        #             flip_state = x[2]
        #             timer = x[0]
        #     if state == 2:
        #         if not x[2] == flip_state:
        #             state = 0
        #             times.append(x[0] - timer)
        #             if x[0] - timer > currentmax:
        #                 print(" New Max: " + str(x[0] - timer))
        #                 currentmax = x[0] - timer
        print("Buffer: " + str(len(times)))
except:
    # print("Max delay: " + str(max(times)))
    # print("Min delay: " + str(min(times)))
    # print("Avg delay: " + str(sum(times)/len(times)))
    print("Max service period: " + str(max(times)))
    print("Min service period: " + str(min(times)))
    print("Avg service period: " + str(sum(times)/len(times)))
