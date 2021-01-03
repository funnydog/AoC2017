#!/usr/bin/env python3

import sys

def load_map(txt):
    m = {}
    rows = txt.splitlines()
    pos = (len(rows[0])//2,len(rows)//2)
    for y, row in enumerate(rows):
        for x, val in enumerate(row):
            if val == "#":
                m[x, y] = "#"

    return m, pos

def left(d):
    return (d[1], -d[0])

def right(d):
    return (-d[1], d[0])

def reverse(d):
    return (-d[0], -d[1])

def add(a, b):
    return (a[0]+b[0], a[1]+b[1])

def move(txt, rounds):
    m, p = load_map(txt)
    d = (0, -1)
    infections = 0
    for i in range(rounds):
        if p in m:
            d = right(d)
            del m[p]
        else:
            d = left(d)
            m[p] = "#"
            infections += 1

        p = add(p, d)
    return infections

def move2(txt, rounds):
    infections = 0
    m, p = load_map(txt)
    d = (0, -1)
    for i in range(rounds):
        value = m.get(p, ".")
        if value == ".":
            m[p] = "W"
            d = left(d)
        elif value == "W":
            m[p] = "#"
            infections += 1
        elif value == "#":
            m[p] = "F"
            d = right(d)
        elif value == "F":
            del m[p]
            d = reverse(d)

        p = add(p, d)
    return infections

def print_map(m, p):
    minx = miny = 1e12
    maxx = maxy = -1e12
    for x, y in m.keys():
        if minx > x: minx = x
        if maxx < x: maxx = x
        if miny > y: miny = y
        if maxy < y: maxy = y
    for y in range(miny, maxy+1):
        for x in range(minx, maxx+1):
            if (x, y) == p:
                print("@", end="")
            elif (x, y) in m:
                print("#", end="")
            else:
                print(".", end="")
        print()

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

    print("Part1:", move(txt, 10000))
    print("Part2:", move2(txt, 10000000))
