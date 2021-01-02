#!/usr/bin/env python3

def part1(steps):
    lst = [0]
    pos = 0
    for i in range(1, 2017+1):
        pos = (pos + steps) % len(lst) + 1
        lst.insert(pos, i)
    return lst[(pos+1)%len(lst)]

def part2(steps):
    lstlen = 1
    pos = 0
    last = None
    for i in range(1, 50000000+1):
        pos = (pos + steps) % lstlen + 1
        lstlen += 1
        if pos == 1:
            last = i
    return last

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

    print("Part1:", part1(int(txt)))
    print("Part2:", part2(int(txt)))
