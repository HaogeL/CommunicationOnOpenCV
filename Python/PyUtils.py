import numpy as np
import struct
#define python function to read matrix stored by C++
def GetMatFromBin(filepath):
    f = open(filepath, "rb")
    fcontent = f.read(16)
    [rows, cols, datatype, channels] = struct.unpack("iiii", fcontent[:16]) #datatype is not used
    data = np.fromfile(filepath, dtype="double", offset=16)
    mat = np.resize(data, (rows, cols, channels))
    return mat