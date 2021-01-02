#!/usr/bin/env python3

SIZE = 256

def hash(lengths, rounds):
    lst = [i for i in range(SIZE)]

    cur = 0
    skip = 0
    for j in range(rounds):
        for l in lengths:
            # reverse from cur to l-1
            for i in range(l//2):
                a = (cur + i) % SIZE
                b = (cur + l - 1 - i) % SIZE
                lst[a], lst[b] = lst[b], lst[a]

            cur = (cur + l + skip) % SIZE
            skip = (skip + 1) % SIZE

    return lst

def part1(txt):
    h = hash(map(int, txt.split(",")), 1)
    return h[0] * h[1]

def part2(txt):
    l = [ord(i) for i in txt]
    l.extend([17,31,73,47,23])
    h = hash(l, 64)
    lst = []
    for i in range(0, SIZE, 16):
        v = h[i]
        for j in range(i+1, i+16):
            v ^= h[j]
        lst.append("{:02x}".format(v))
    return "".join(lst)

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

    print("Part1:", part1(txt))
    print("Part2:", part2(txt))
