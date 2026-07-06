import os

texture_file = "door_texture.c"
line = "_Alignas(256)"

if (os.path.exists(texture_file)):
    with open(texture_file, "r+") as f:
        content = f.read()
        f.seek(0,0)
        f.write(line.rstrip('\r\n') + '\n' + content)

    exit(0)
else:
    print("Missing files! Did you run convimg?")
    exit(1)
