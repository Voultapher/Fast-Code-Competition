#!/usr/bin/python3

import sys

x = []
for i in range(0, int(sys.argv[1])):
    x.append(i)

y = [str(i) for i in x] 

print("".join(y), end='')
