#!/usr/bin/env python3

def count_steps(to):
    x, y = 0, 0
    dx, dy = 0, 1
    last = 0
    while True:
        last += 1
        if last == to:
            return abs(x) + abs(y)

        if x == -y or (x < 0 and y == x) or (x > 0 and x - 1 == y):
            dx, dy = dy, -dx
        x += dx
        y += dy

adjacent = [(1,0), (1,-1), (0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1)]

def first_larger(to):
    m = {}
    m[0, 0] = 1
    x, y = 1, 0
    dx, dy = 1, 0
    while True:
        adj = 0
        for ax, ay in adjacent:
            adj += m.get((x+ax, y+ay), 0)

        if adj > to:
            return adj

        m[x, y] = adj
        if x == -y or (x < 0 and y == x) or (x > 0 and x - 1 == y):
            dx, dy = dy, -dx
        x += dx
        y += dy

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

    print("Part1:", count_steps(int(txt)))
    print("Part2:", first_larger(int(txt)))
