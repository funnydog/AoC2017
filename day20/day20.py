#!/usr/bin/env python3
import re
import sys

def add(a, b):
    return (a[0]+b[0], a[1]+b[1], a[2]+b[2])

def distance(a):
    return sum(map(abs, a))

class Particle(object):
    def __init__(self, n, p, v, a):
        self.n = n
        self.p = p
        self.v = v
        self.a = a

    def update(self):
        self.v = add(self.v, self.a)
        self.p = add(self.p, self.v)

    def stable(self, axis):
        return self.v[axis] * self.a[axis] >= 0 and self.v[axis] * self.p[axis] >= 0

    def __repr__(self):
        return "{}: p={}, v={}, a={}".format(self.n, self.p, self.v, self.a)

def update(particles, collision):
    # update the world
    coll = {}
    for p in particles:
        p.update()
        coll[p.p] = coll.get(p.p, 0) + 1

    if collision:
        return [p for p in particles if coll[p.p] == 1]
    else:
        return particles

def part1(parts):
    for axis in range(3):
        while not all(p.stable(axis) for p in parts):
            parts = update(parts, False)

    parts = update(parts, False)
    p = min(parts, key = lambda x: distance(x.p))
    return p.n

def part2(parts):
    for axis in range(3):
        while not all(p.stable(axis) for p in parts):
            parts = update(parts, True)

    parts = update(parts, True)
    return len(parts)

def parse(txt):
    parts = []
    pat = re.compile(r"[pva]=<\s*(-?\d+),(-?\d+),(-?\d+)>")
    for row in txt.splitlines():
        lst = []
        for c in row.split(", "):
            m = pat.match(c)
            if m:
                lst.append(tuple(map(int, m.groups())))
        parts.append(Particle(len(parts), *lst))
    return parts

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

    p = parse(txt)
    print("Part1:", part1(p))
    p = parse(txt)
    print("Part2:", part2(p))
