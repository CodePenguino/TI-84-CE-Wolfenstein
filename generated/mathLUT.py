import os
import math

FIX_SHIFT      = 8
FIX_SCALE      = (1<<FIX_SHIFT)

LUT_LENGTH     = 1024

def fxdiv(a, b):
    return int(((a) * FIX_SCALE) / (b))

def float2fx(a):
    return int(a*FIX_SCALE);

if os.path.exists("math.txt"):
    os.remove("math.txt")

with open("math.txt", "a") as f:
    f.write("const int24_t div_lut[")
    f.write(str(LUT_LENGTH))
    f.write("] = {\n")

    for x in range(0, LUT_LENGTH):
        f.write("    ")
        if(x != 0):
            f.write(str(int(65536 / x)))
        else:
            f.write(str(8388607))
        f.write(",\n")

    f.write("};\n")
