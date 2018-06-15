echo "type relative path to DS1ops, if in same directory as DUBSAT 1 it should be ../../../../../DS1Ops"
read ds1ops
ds1ops=${ds1ops:-../../../../../DS1Ops}
py CANCHeaderTest.py ../CANDB-MASTER-DS1.dbc
cp codeGenOutput/canwrap.h ../../../../src/dsbase/interfaces/canwrap.h
cp codeGenOutput/canwrap.c ../../../../src/dsbase/interfaces/canwrap.c
cp ../CANDB-MASTER-DS1.dbc $ds1ops/COSMOS/CanCosmosGenerator/CANDB-MASTER-DS1.dbc
cd $ds1ops/COSMOS/CanCosmosGenerator/
py CANTelemetry_Generator.py CANDB-MASTER-DS1.dbc
cp tlm.txt ../flatsat/config/targets/CAN_LOCAL/cmd_tlm/tlm.txt
cp cmd.txt ../flatsat/config/targets/CAN_LOCAL_CMD/cmd_tlm/cmd.txt
