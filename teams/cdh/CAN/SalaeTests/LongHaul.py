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

flip1: List[float] = []
flip2: List[float] = []
flip3: List[float] = []
flip4: List[float] = []
flip5: List[float] = []

flip1time: float = -1.0
flip2time: float = -1.0
flip3time: float = -1.0
flip4time: float = -1.0
flip5time: float = -1.0

flip1state: int = 0
flip2state: int = 0
flip3state: int = 0
flip4state: int = 0
flip5state: int = 0


s = saleae.Saleae()

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
  flip1state = content[0][1]
  flip2state = content[0][2]
  flip3state = content[0][3]
  flip4state = content[0][4]
  flip5state = content[0][5]
  flip1time = -1.0
  flip2time = -1.0
  flip3time = -1.0
  flip4time = -1.0
  flip5time = -1.0
  for x in content:
    if flip1time >= -0.5 and flip1state != x[1]:
      flip1.append(x[0] - flip1time)
      flip1state = x[1]
      flip1time = x[0]
    elif flip1time < -0.5 and flip1state != x[1]:
      flip1time = x[0]
      flip1state = x[1]
    if flip2time >= -0.5 and flip2state != x[2]:
      flip2.append(x[0] - flip2time)
      flip2state = x[2]
      flip2time = x[0]
    elif flip2time < -0.5 and flip2state != x[2]:
      flip2time = x[0]
      flip2state = x[2]
    if flip3time >= -0.5 and flip3state != x[3]:
      flip3.append(x[0] - flip3time)
      flip3state = x[3]
      flip3time = x[0]
    elif flip3time < -0.5 and flip3state != x[3]:
      flip3time = x[0]
      flip3state = x[3]
    if flip4time >= -0.5 and flip4state != x[4]:
      flip4.append(x[0] - flip4time)
      flip4state = x[4]
      flip4time = x[0]
    elif flip4time < -0.5 and flip4state != x[4]:
      flip4time = x[0]
      flip4state = x[4]
    if flip5time >= -0.5 and flip5state != x[5]:
      flip5.append(x[0] - flip5time)
      flip5state = x[5]
      flip5time = x[0]
    elif flip5time < -0.5 and flip5state != x[5]:
      flip5time = x[0]
      flip5state = x[5]
  counter += 1
  print("Min, Max, Avg, Count for pin x file {}:".format(counter))
  if len(flip1) > 0:
     print("1: {}, {}, {}, {}".format(min(flip1), max(flip1), sum(flip1) / len(flip1), len(flip1)))
  if len(flip2) > 0:
    print("2: {}, {}, {}, {}".format(min(flip2), max(flip2), sum(flip2) / len(flip2), len(flip2)))
  if len(flip3) > 0:
     print("3: {}, {}, {}, {}".format(min(flip3), max(flip3), sum(flip3) / len(flip3), len(flip3)))
  if len(flip4) > 0:
     print("4: {}, {}, {}, {}".format(min(flip4), max(flip4), sum(flip4) / len(flip4), len(flip4)))
  if len(flip5) > 0:
     print("5: {}, {}, {}, {}".format(min(flip5), max(flip5), sum(flip5) / len(flip5), len(flip5)))
