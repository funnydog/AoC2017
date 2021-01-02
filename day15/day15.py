#!/usr/bin/env python3

def parse(txt):
    lst = txt.splitlines()
    a = int(lst[0].split("with ")[1])
    b = int(lst[1].split("with ")[1])
    return a, b

def part1(a, b):
    count = 0
    for i in range(40000000):
        a = (a * 16807) % 2147483647
        b = (b * 48271) % 2147483647
        if (a & 0xffff) == (b & 0xffff):
            count += 1
    return count

def part2(a, b):
    count = 0
    for i in range(5000000):
        while True:
            a = (a * 16807) % 2147483647
            if a & 3 == 0:
                break

        while True:
            b = (b * 48271) % 2147483647
            if b & 7 == 0:
                break

        if (a & 0xffff) == (b & 0xffff):
            count += 1
    return count

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

    a, b = parse(txt)
    print("Part1:", part1(a, b))
    print("Part2:", part2(a, b))
