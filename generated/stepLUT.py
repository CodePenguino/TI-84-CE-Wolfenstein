import os
import math

FIX_SHIFT      = 8
FIX_SCALE      = (1<<FIX_SHIFT)
NUM_ELEMENTS   = 255
TEXTURE_HEIGHT = 64

def fxdiv(a, b):
    return int(((a) * FIX_SCALE) / (b))

def float2fx(a):
    return int(a*FIX_SCALE);

if os.path.exists("delta.txt"):
    os.remove("delta.txt")

with open("delta.txt", "a") as f:
    f.write("const uint24_t delta_lut[")
    f.write(str(NUM_ELEMENTS))
    f.write("] = {\n")

    for x in range(1, NUM_ELEMENTS):
        f.write("   ")
        f.write(str(fxdiv(TEXTURE_HEIGHT, x)))
        f.write(",\n")

    f.write("};\n\n")
# ----------------------------------------
    f.write("const uint24_t texture_offset_lut[")
    f.write(str(NUM_ELEMENTS-180))
    f.write("] = {\n")

    for x in range(181, NUM_ELEMENTS):
        f.write("   ")
        # f.write(str( float2fx( ((x - (64/x))-180)/8 ) ) )
        step_size = float(TEXTURE_HEIGHT / x)
        f.write(str ( float2fx( ((x-180)/2)*step_size) ))
        # f.write(str( float2fx( (x-180)/8 ) ) )
        # f.write(str( float( (64-(45/x))/2) ) )
        f.write(",\n")

    f.write("};\n")
