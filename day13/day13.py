#!/usr/bin/env python3

def parse(txt):
    fw = []
    for line in txt.splitlines():
        fw.append(tuple(map(int, line.split(": "))))
    fw.sort()
    return fw

def part1(fw):
    return sum(d*r for d, r in fw if d%(2*r-2) == 0)

def part2(fw):
    j = 0
    while True:
        for d, r in fw:
            if (d+j) % (2*r - 2) == 0:
                break
        else:
            break

        j += 1

    return j

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

    f = parse(txt)
    print("Part1:", part1(f))
    print("Part2:", part2(f))
