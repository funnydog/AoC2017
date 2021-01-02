#!/usr/bin/env python3

import sys

cur = 0
discarded = 0

def scan_next(f):
    global cur, discarded
    state = 0
    while True:
        cur = f.read(1)
        if not cur:
            break

        if state == 0:
            if cur == '<':
                state = 1
            elif cur in "{}":
                break
            else:
                pass
        elif state == 1:
            if cur == "!":
                state = 2
            elif cur == ">":
                state = 0
            else:
                discarded += 1
        elif state == 2:
            state = 1
        else:
            print("Error: unknown state", file=sys.stderr)
            sys.exit(1)

def match(f, val):
    global cur
    if val != cur:
        print("Error: expected {}, found {}".format(val, cur), file=sys.stderr)
        sys.exit(1)
    scan_next(f)

def group(f, points):
    global cur
    p = 0
    while cur == "{":
        p += points
        scan_next(f)
        p += group(f, points+1)
        match(f, "}")
    return p

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            scan_next(f)
            points = group(f, 1)
            match(f, "")
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    print("Part1:", points)
    print("Part2:", discarded)
