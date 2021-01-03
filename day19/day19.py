#!/usr/bin/env python3

def left(delta):
    return (-delta[1], delta[0])

def right(delta):
    return (delta[1], -delta[0])

def add(a, b):
    return (a[0]+b[0], a[1]+b[1])

def follow(txt):
    m = txt.splitlines()

    # find the starting point
    for x, val in enumerate(m[0]):
        if val == "|":
            break
    p = (x, 0)
    d = (0, 1)

    steps = 0
    letters = []
    while True:
        x, y = p

        # collect the letters
        if not m[y][x] in "+|- ":
            letters.append(m[y][x])

        if m[y][x] == "+":
            # find a new direction
            for nd in (left(d), right(d)):
                np = add(p, nd)
                nx, ny = np
                if 0 <= ny < len(m) and 0 <= nx < len(m[ny]) and m[ny][nx] != " ":
                    d = nd
                    break
        elif m[y][x] == " ":
            # end of the path
            break

        p = add(p, d)
        steps += 1

    return "".join(letters), steps

import sys
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    part1, part2 = follow(txt)
    print("Part1:", part1)
    print("Part2:", part2)
