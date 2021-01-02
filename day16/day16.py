#!/usr/bin/env python3

import string
import sys

def dance(progs, move):
    if move[0] == "s":
        pos = len(progs) - int(move[1:])
        progs = progs[pos:] + progs[:pos]
    elif move[0] == "x":
        a, b = map(int, move[1:].split("/"))
        progs[a], progs[b] = progs[b], progs[a]
    elif move[0] == "p":
        a, b = map(progs.index, move[1:].split("/"))
        progs[a], progs[b] = progs[b], progs[a]
    return progs

def step(progs, txt):
    for move in txt.split(","):
        progs = dance(progs, move)
    return progs

def part1(txt, l):
    progs = list(string.ascii_lowercase[:l])
    return "".join(step(progs, txt))

def part2(txt, l):
    initial = list(string.ascii_lowercase[:l])
    tortoise = step(initial[:], txt)
    hare = step(step(initial[:], txt), txt)
    while tortoise != hare:
        tortoise = step(tortoise, txt)
        hare = step(hare, txt)
        hare = step(hare, txt)

    mu = 0
    tortoise = initial[:]
    while tortoise != hare:
        tortoise = step(tortoise, txt)
        hare = step(hare, txt)
        mu += 1

    lam = 1
    hare = step(tortoise[:], txt)
    while tortoise != hare:
        hare = step(hare, txt)
        lam += 1

    cycles = (1000000000 - mu) % lam + mu
    progs = initial
    for i in range(cycles):
        progs = step(progs, txt)

    return "".join(progs)

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

    #txt = "s1,x3/4,pe/b"
    print("Part1:", part1(txt, 16))
    print("Part2:", part2(txt, 16))
