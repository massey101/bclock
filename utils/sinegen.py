#!/usr/bin/python3

import math

sine=[]

for i in range(0x100):
    x = 2.0 * math.pi * float(i) / 255.0
    output = math.sin(x)
    sine.append(int(round((output + 1) * 127.5)))

print(", ".join(["0x{:2x}".format(val) for val in sine[:64]]))
