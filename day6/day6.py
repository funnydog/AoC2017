#!/usr/bin/env python3

def next_step(banks):
    m = max(banks)
    i = banks.index(m)
    banks[i] = 0
    while m:
        i = (i + 1) % len(banks)
        banks[i] += 1
        m -= 1
    return banks

def floyd(fun, banks):
    tortoise = fun(banks[:])
    hare = fun(fun(banks[:]))
    while tortoise != hare:
        tortoise = fun(tortoise)
        hare = fun(fun(hare))

    mu = 0
    tortoise = banks[:]
    while tortoise != hare:
        tortoise = fun(tortoise)
        hare = fun(hare)
        mu += 1

    lam = 1
    hare = fun(tortoise[:])
    while tortoise != hare:
        hare = fun(hare)
        lam += 1

    return mu+lam, lam

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

    banks = list(map(int, txt.split("\t")))
    part1, part2 = floyd(next_step, banks)
    print("Part1:", part1)
    print("Part2:", part2)
