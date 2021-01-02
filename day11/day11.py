#!/usr/bin/env python3

# mapping between hex and 2d grid
#      [n] [ne]
#       | /
# [nw]-[0]-[se]
#     / |
# [sw] [s]

DIR = {
    'n': (0, -1),
    'ne': (1, -1),
    'se': (1, 0),
    's': (0, 1),
    'sw': (-1, 1),
    'nw': (-1, 0)
}

def distance(a):
    return (abs(a[0]) + abs(a[1]) + abs(a[0]+a[1])) // 2

def walk(txt):
    pos = (0,0)
    maxd = 0
    d = 0
    for d in txt.split(","):
        dq, dr = DIR[d]
        pos = (pos[0]+dq, pos[1]+dr)
        d = distance(pos)
        if maxd < d:
            maxd = d

    return d, maxd

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read().strip()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    part1, part2 = walk(txt)
    print("Part1:", part1)
    print("Part2:", part2)
