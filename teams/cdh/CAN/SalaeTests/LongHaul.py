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

flip1: List[float]
flip2: List[float]
flip3: List[float]
flip4: List[float]
flip5: List[float]

flip1time: float = -1.0
flip2time: float = -1.0
flip3time: float = -1.0
flip4time: float = -1.0
flip5time: float = -1.0

flip1state: int
flip2state: int
flip3state: int
flip4state: int
flip5state: int


s = saleae.Saleae()

try:
  counter: int = 0
  while True:
    s.capture_to_file('/tmp/emoriessaleaedump' + str(counter) + '.logicdata')
    s.export_data2("/tmp/emoriessaleaedump.csv",
                   digital_channels=[0, 1, 2, 3, 4],
                   format='csv',
                   display_base="SEPARATE",
                   column_headers=False)

    f = open("/tmp/emoriessaleaedump.csv")
    content: List[str] = f.readlines()
    content: List[str] = [x.strip() for x in content]
    content: List[List[str]] = [x.split(",") for x in content]
    content: List[Tuple[float, int, int, int, int, int]] = [[float(x[0]), int(x[1]), int(x[2]), int(x[3]), int(x[4]), int(x[5])] for x in content]
    for x in content:
      if flip1time >= -0.5 and flip1state != x[1]:
        flip1.append(x[0] - flip1time)
        flip1state = x[1]
        flip1time = x[0]
      elif flip1time < -0.5:
        flip1time = x[0]
        flip1state = x[1]
      if flip2time >= -0.5 and flip2state != x[2]:
        flip2.append(x[0] - flip2time)
        flip2state = x[2]
        flip2time = x[0]
      elif flip2time < -0.5:
        flip2time = x[0]
        flip2state = x[2]
      if flip3time >= -0.5 and flip3state != x[3]:
        flip3.append(x[0] - flip3time)
        flip3state = x[3]
        flip3time = x[0]
      elif flip3time < -0.5:
        flip3time = x[0]
        flip3state = x[3]
      if flip4time >= -0.5 and flip4state != x[4]:
        flip4.append(x[0] - flip4time)
        flip4state = x[4]
        flip4time = x[0]
      elif flip4time < -0.5:
        flip4time = x[0]
        flip4state = x[4]
      if flip5time >= -0.5 and flip5state != x[5]:
        flip5.append(x[0] - flip5time)
        flip5state = x[5]
        flip5time = x[0]
      elif flip5time < -0.5:
        flip5time = x[0]
        flip5state = x[5]
    print("Min, Max, Avg for pin x:")
    print("1: {}, {}, {}".format(min(flip1), max(flip1), sum(flip1) / len(flip1)))
except:
  print("There was a problem.")
