import os
import math

FILE_NAME = "vline.txt"
TEXTURE_HEIGHT = 64

def scale_code(line_length):
    if os.path.exists(FILE_NAME):
        os.remove(FILE_NAME)

    with open(FILE_NAME, "a") as f:
        f.write("    .assume adl=1\n\n")

        for i in range(0, line_length, 2):
            f.write("vline_" + str(i) + ":\n")

            if(i == 0):
                f.write("    ret\n")
                continue

            step_size = float(TEXTURE_HEIGHT / i)
            texture_first = 0.0
            texture_second = step_size/2.0

            for j in range(0, i):
                texture_delta = int(texture_second) - int(texture_first)

                if(texture_delta == 1):
                    f.write("    inc bc\n")
                    f.write("    ld a,(bc)\n")
                if(texture_delta == 2):
                    f.write("    inc bc\n    inc bc\n")
                    f.write("    ld a,(bc)\n")
                if(texture_delta == 3):
                    f.write("    inc bc\n    inc bc\n    inc bc\n")
                    f.write("    ld a,(bc)\n")
                if(texture_delta > 3):
                    f.write("    ld a,c\n")
                    f.write("    add a," + str(texture_delta) + "\n")
                    f.write("    ld c,a\n")
                    f.write("    ld a,(bc)\n")

                f.write("    ld (hl),a\n")
                f.write("    add hl,de\n")

                texture_first = texture_second
                texture_second += step_size
            f.write("    ret\n")



scale_code(180)
