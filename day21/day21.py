#!/usr/bin/env python3

import sys

def rotate(rule):
    table = [row for row in rule.split("/")]
    corner = len(table)
    ntable = []
    for y in range(corner):
        row = []
        for x in range(corner):
            row.append(table[x][len(table) - y - 1])
        ntable.append("".join(row))
    return "/".join(ntable)

def flip(rule):
    return "/".join("".join(reversed(row)) for row in rule.split("/"))

def parse(txt):
    rules = {}
    for row in txt.splitlines():
        src, dst = row.split(" => ")
        for i in range(8):
            if i == 4:
                src = flip(src)
            src = rotate(src)
            if src not in rules:
                rules[src] = dst
    return rules

def get_square(table, x, y, step):
    return "/".join(("".join(table[ty][tx] for tx in range(x, x+step)) for ty in range(y, y+step)))

def save_square(table, x, y, rule, rlen):
    pos = 0
    for ty in range(y, y+rlen):
        for tx in range(x, x+rlen):
            if rule[pos] == "/":
                pos += 1
            table[ty][tx] = rule[pos]
            pos += 1

def step(table, rules):
    corner = len(table)
    if corner % 2:
        srclen = 3
        dstlen = 4
    else:
        srclen = 2
        dstlen = 3

    r = corner // srclen
    ntable = [[' ' for i in range(r*dstlen)] for j in range(r*dstlen)]
    for ty in range(r):
        for tx in range(r):
            src = get_square(table, tx*srclen, ty*srclen, srclen)
            dst = rules[src]
            save_square(ntable, tx*dstlen, ty*dstlen, dst, dstlen)

    return ntable

def count_lit(rules, rounds):
    txt =""".#.
..#
###
"""
    table = [list(i) for i in txt.splitlines()]

    for i in range(rounds):
        table = step(table, rules)
    return sum("".join(row).count("#") for row in table)

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

    rules = parse(txt)
    print("Part1:", count_lit(rules, 5))
    print("Part2:", count_lit(rules, 18))
