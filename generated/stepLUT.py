import os

FIX_SHIFT      = 8
FIX_SCALE      = (1<<FIX_SHIFT)
NUM_ELEMENTS   = 255
TEXTURE_HEIGHT = 64

def fxdiv(a, b):
    return int(((a) * FIX_SCALE) / (b))

if os.path.exists("delta.txt"):
    os.remove("delta.txt")

with open("delta.txt", "a") as f:
    f.write("const uint16_t delta_lut[")
    f.write(str(NUM_ELEMENTS))
    f.write("] = {\n")

    for x in range(1, NUM_ELEMENTS):
        f.write("  ")
        f.write(str(int(fxdiv(TEXTURE_HEIGHT, x))))
        f.write(",\n")

    f.write("};\n\n")
# ----------------------------------------
    f.write("const uint8_t texture_offset_lut[")
    f.write(str(NUM_ELEMENTS-180))
    f.write("] = {\n")

    for x in range(1, NUM_ELEMENTS-180):
        f.write("  ")
        f.write(str(int((64-fxdiv(45, x+180))>>1)))
        f.write(",\n")

    f.write("};\n")
