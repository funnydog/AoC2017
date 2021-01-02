#!/usr/bin/env python3

def parse(txt):
    g = []
    for row in txt.splitlines():
        _, deps = row.split(" <-> ")
        g.append([int(i) for i in deps.split(", ")])
    return g

def dfs(g, i, visited):
    visited[i] = True
    for j in g[i]:
        if not visited[j]:
            dfs(g, j, visited)

def part1(g):
    visited = [False] * len(g)
    dfs(g, 0, visited)
    return sum(1 for i in visited if i)

def part2(g):
    visited = [False] * len(g)
    group = 0
    for i in range(len(g)):
        if not visited[i]:
            dfs(g, i, visited)
            group += 1

    return group

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

    g = parse(txt)
    print("Part1:", part1(g))
    print("Part2:", part2(g))
