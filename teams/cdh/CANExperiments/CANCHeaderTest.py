#!/usr/bin/env python3

# Copyright (c) 2013, Eduard Broecker
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that
# the following conditions are met:
#
#    Redistributions of source code must retain the above copyright notice, this list of conditions and the
#    following disclaimer.
#    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#    following disclaimer in the documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.

from __future__ import print_function
from __future__ import absolute_import

# from .log import setup_logger, set_log_level
#logger = setup_logger('root')
import sys
import os
sys.path.append('..')
import canmatrix.formats
import canmatrix.canmatrix as cm
import canmatrix.copy as cmcp

def toPyObject(infile, outfileName, **options):
    dbs = {}
    dbs = canmatrix.formats.loadp(infile, **options)
    outdbs = {}
    for name in dbs:
        db = None
        if db is None:
            db = dbs[name]
        outdbs[name] = db
    return outdbs['']

def createCHeaderBackup(candb, cFileName):
    cFile = open(cFileName, "w")
    cFile.write("#ifndef CANDB_HEADER\n#define CANDB_HEADER\n\n")
    cFile.write("#include <stdint.h>\n\n")
    for frame in candb.frames:
        cFile.write("typedef struct {\n")
        for sig in frame:
            if sig.is_signed:
                cFile.write("\tint")
            else:
                cFile.write("\tuint")
            if sig.signalsize == 8:
                cFile.write("8_t ")
            elif sig.signalsize == 16:
                cFile.write("16_t ")
            elif sig.signalsize == 32:
                cFile.write("32_t ")
            else:
                cFile.write("64_t ")
            cFile.write(sig.name + ";\n")
        cFile.write("} " + frame.name + ";\n\n")
    cFile.write("\n#endif")
    cFile.close()

def getSignalSize(sig, frame, floatList):
    if ((frame.id, sig.name, 1) in floatList):
        return "float"
    elif ((frame.id, sig.name, 2) in floatList):
        return "double"
    elif sig.min >= 0 and sig.max <= 2 ** 8 - 1:
        return "uint8_t"
    elif sig.min >= 0 and sig.max <=  2 ** 16 - 1:
        return "uint16_t"
    elif sig.min >= 0 and sig.max <=  2 ** 32 - 1:
        return "uint32_t"
    elif sig.min >= 0 and sig.max <=  2 ** 64 - 1:
        return "uint64_t"
    elif sig.min >= - (2 ** (8-1)) and sig.max <=  2 ** (8-1) - 1:
        return "int8_t"
    elif sig.min >= - (2 ** (16-1)) and sig.max <=  2 ** (16-1) - 1:
        return "int16_t"
    elif sig.min >= - (2 ** (32-1)) and sig.max <=  2 ** (32-1) - 1:
        return "int32_t"
    elif sig.min >= - (2 ** (64-1)) and sig.max <=  2 ** (64-1) - 1:
        return "int64_t"
    else:
        # print("Warning: " + sig.name + " type unknown. Using uint8_t instead.")
        return "uint8_t"

def createCHeader(candb, cFileName, floatList):
    cFile = open(cFileName, "w")
    for frame in candb.frames:
        cFile.write("typedef struct " + frame.name + " {\n")
        for sig in frame:
            cFile.write("    "
                + getSignalSize(sig, frame, floatList)
                + " " + sig.name
                + "; // "
                + (sig.unit if sig.unit != "" else " (No Units)")
                + "\n"
            )
        cFile.write("} " + frame.name + ";\n\n")
    for frame in candb.frames:
        cFile.write("void encode"
            + frame.name + "(" + frame.name + " *input, CANPacket* output);\n")
        cFile.write("void decode" + frame.name + "(CANPacket *input, "
            + frame.name + " *output);\n\n")
    cFile.close()

def createCMain(candb, cFileName, floatList):
    cFile = open(cFileName, "w")
    for frame in candb.frames:
        # Decode Function Implementation
        cFile.write("void decode"
            + frame.name + "(CANPacket *input, " + frame.name + " *output){\n")
        cFile.write("    uint64_t *thePointer = (uint64_t *) input -> data;\n")
        cFile.write("    reverseArray(input -> data, 0, 7);\n")
        cFile.write("    const uint64_t fullData = *thePointer;\n")
        for sig in frame:
            sigType = getSignalSize(sig, frame, floatList)
            cFile.write("    output -> "
                + sig.name
                + " = ("
                + ("uint32_t" if (sigType == "float") else ("uint8_t" if (sigType == "double") else sigType))
                + ") (((fullData & ((uint64_t) "
                + str(hex(int(str(int((-1 + 10 ** sig.signalsize)//9)), 2)))
                + (" << " + str(int(frame.size * 8 - int(sig.getStartbit()) - sig.signalsize)) if int(frame.size * 8 - int(sig.getStartbit()) - sig.signalsize) != 0.0 else "")
                + (")) >> " + str(int(frame.size * 8 - int(sig.getStartbit()) - sig.signalsize)) if int(frame.size * 8 - int(sig.getStartbit()) - sig.signalsize) != 0.0 else "))")
                + (") * " + (str(int(sig.factor)) if int(sig.factor) - float(sig.factor) == 0.0 else str(sig.factor)) if sig.factor != 1.0 else ")")
                + (" + " + (str(int(sig.offset)) if int(sig.offset) - float(sig.offset) == 0.0 else str(sig.offset)) if sig.offset != 0.0 else "")
                + ");\n")
        cFile.write("}\n\n")
        # Encode Function Implementation
        cFile.write("void encode" + frame.name
            + "(" + frame.name + " *input, CANPacket *output){\n")
        # cFile.write("    CANPacket *output = malloc(sizeof(CANPacket));\n")
        cFile.write("    output -> id = "
            + (str(frame.id) if frame.id != 2147483648 else "0")
            + ";\n")
        cFile.write("    uint64_t fullPacketData = 0x0000000000000000;\n")
        for sig in frame:
            cFile.write("    fullPacketData |= ((uint64_t)((input -> "
                + sig.name
                + (" - " + (str(int(sig.offset)) if int(sig.offset) - float(sig.offset) == 0.0 else str(sig.offset)) if sig.offset != 0.0 else "")
                + ((") / " + (str(int(sig.factor)) if int(sig.factor) - float(sig.factor) == 0.0 else str(sig.factor))) if (sig.factor != 1.0) else (")"))
                + (")) << " + str(64 - int(sig.getStartbit()) - sig.signalsize) if 64 - int(sig.getStartbit()) - sig.signalsize != 0 else "))")
                + ";\n")
        cFile.write("    uint64_t *thePointer = (uint64_t *) (&(output -> data));\n")
        cFile.write("    *thePointer = fullPacketData;\n")
        cFile.write("    reverseArray((output->data), 0, 7);\n")
        cFile.write("}\n\n")
    cFile.close()

def createCMacros(candb, cFileName):
    cFile = open(cFileName, "w")
    # Add macros for packet IDs for filter purposes.
    for frame in candb.frames:
        cFile.write("#define CAN_ID_" + frame.name.upper() + " " + (str(frame.id) if frame.id != 2147483648 else "0") + "\n")
    # Add macros for ENUMS defined at the signal level.
    cFile.write("\n")
    for frame in candb.frames:
        for sig in frame:
            for a in sig.values.keys():
                cFile.write ("#define CAN_ENUM_" + sig.name.upper() + "_" + sig.values[a].upper().replace(" ", "") + " " + str(a) + "\n")
    # Add macros for ENUMS defined at the global level.
    cFile.write("\n")
    for vt in candb.valueTables:
        for a in candb.valueTables[vt].keys():
            cFile.write ("#define CAN_ENUM_" + vt.upper() + "_" + candb.valueTables[vt][a].upper().replace(" ", "") + " " + str(a) + "\n")
    cFile.close()
def handleFloats(infile):
    file = open(infile, "r")
    out = []
    for line in file.readlines():
        linearr = line.split()
        if len(linearr) > 4 and linearr[0] == "SIG_VALTYPE_":
            out.append(((int(linearr[1]) - 2 ** (32 - 1) if int(linearr[1]) != 2147483648 else 2147483648), linearr[2], int(linearr[4][0])))
    print (out)
    return out
def main():
    # Command line stuff
    from optparse import OptionParser
    usage = """\n  %prog import-file\n    import-file: *.dbc|*.dbf|*.kcd|*.arxml|*.json|*.xls(x)|*.sym"""
    parser = OptionParser(usage=usage)
    (cmdlineOptions, args) = parser.parse_args()
    if len(args) < 1:
        parser.print_help()
        sys.exit(1)
    infile = args[0]
    # Make the CAN Object from DBC
    CANObj = toPyObject(infile, "", **cmdlineOptions.__dict__)
    # Now, CANMatrix has an issue where it doesn't correctly id floats.
    # For now, we have to work around this manually

    # Make the directory and generate the output files.
    if not os.path.exists("codeGenOutput"):
        os.makedirs("codeGenOutput")
    floatList = handleFloats(infile)
    createCHeader(CANObj, "codeGenOutput/headerCode.c", floatList)
    createCMain(CANObj, "codeGenOutput/mainCode.c", floatList)
    createCMacros(CANObj, "codeGenOutput/macros.c")

if __name__ == '__main__':
    sys.exit(main())
