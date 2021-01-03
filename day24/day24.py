#!/usr/bin/env python3

import sys

class Component(object):
    def __init__(self, txt):
        self.ends = sorted(map(int, txt.split("/")))
        self.score = sum(self.ends)

    def __repr__(self):
        return repr(self.ends)

def load_components(txt):
    return [Component(c) for c in txt.splitlines()]

maxlen = 0
maxlenstrength = 0
maxstrength = 0
def backtrack(strength, length, free, comps):
    global maxstrength, maxlen, maxlenstrength
    if maxlen <= length:
        maxlen = length
        if maxlenstrength < strength:
            maxlenstrength = strength
    if maxstrength < strength:
        maxstrength = strength

    for i in range(len(comps)):
        if free in comps[i].ends:
            c = comps.pop(i)
            other = (free == c.ends[0]) and c.ends[1] or c.ends[0]
            backtrack(strength + free + other, length + 1, other, comps)
            comps.insert(i, c)

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

    c = load_components(txt)
    backtrack(0, 0, 0, c)
    print("Part1:", maxstrength)
    print("Part2:", maxlenstrength)
