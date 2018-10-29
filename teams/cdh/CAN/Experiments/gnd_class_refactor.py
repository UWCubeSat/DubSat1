import sys

mask = 7340032
eid_offset = 0x80000000
translation = {0x100000: 0x500000, 0x200000: 0x600000, 0x300000: 0x700000, 0x400000: 0x100000, 0x000000 : 0x000000}

def main(file):
    with open(file) as fp:
        line = fp.readline()
        newfile = ""
        while line:
            lineArray = line.split(" ")
            id = -1
            ididx = -1
            if len(lineArray) > 1 and lineArray[0] in ["SIG_VALTYPE_", "VAL_", "BO_"]:
                id = int(lineArray[1])
                ididx = 1
            if len(lineArray) > 2 and lineArray[0] == "CM_" and lineArray[1] in ["SG_", "BO_"]:
                id = int(lineArray[2])
                ididx = 2
            if not id == -1:
                maskbit = (id - eid_offset) & mask
                if maskbit in translation:
                    # print(hex((int(id) - eid_offset) & ~mask))
                    id = ((id - eid_offset) & ~mask) | translation[maskbit]
                    # print(hex(id))
                    lineArray[ididx] = str(id + eid_offset)
                else:
                    raise Exception("Uh oh something went wrong with a signal")
            print(" ".join(lineArray).replace("\n",""), end=" \n")
            line = fp.readline()



if __name__ == "__main__":
    # execute only if run as a script
    if not len(sys.argv) == 2:
        print("Usage: python3 gnd_class_refactor.py path/to/dbc.dbc")
    main(sys.argv[1])
