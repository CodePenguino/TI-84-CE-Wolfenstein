import os
import math

FIX_SHIFT      = 8
FIX_SCALE      = (1<<FIX_SHIFT)
NUM_ELEMENTS   = 384
TEXTURE_HEIGHT = 64
WINDOW_HEIGHT  = 180

def fxdiv(a, b):
    return int(((a) * FIX_SCALE) / (b))

def float2fx(a):
    return int(a*FIX_SCALE);

if os.path.exists("delta.txt"):
    os.remove("delta.txt")

with open("delta.txt", "a") as f:
    f.write("const uint24_t texture_lut_u24[")
    f.write(str(180))
    f.write("] = {\n")

    for x in range(1, 180):
        f.write("   ")
        f.write(str(float2fx(TEXTURE_HEIGHT / x)))
        # f.write(str(fxdiv(TEXTURE_HEIGHT, x)))
        f.write(",\n")

    f.write("};\n\n")

# ----------------------------------------
    f.write("const uint8_t texture_lut_u8[")
    f.write(str(NUM_ELEMENTS-180))
    f.write("] = {\n")

    for x in range(181, NUM_ELEMENTS):
        f.write("   ")
        f.write(str(float2fx(TEXTURE_HEIGHT / x)))
        f.write(",\n")

    f.write("};\n\n")

# ----------------------------------------
    f.write("const uint24_t texture_offset_lut[")
    f.write(str(NUM_ELEMENTS-180))
    f.write("] = {\n")

    for x in range(181, NUM_ELEMENTS):
        f.write("   ")
        step_size = float(TEXTURE_HEIGHT / x)
        f.write(str(float2fx(((x-180)/2)*step_size)))
        f.write(",\n")

    f.write("};\n")
