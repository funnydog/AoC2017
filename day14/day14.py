#!/usr/bin/env python3

SIZE = 256

def hash(lengths, rounds):
    lst = [i for i in range(SIZE)]

    cur = 0
    skip = 0
    for j in range(rounds):
        for l in lengths:
            # reverse from cur to l-1
            for i in range(l//2):
                a = (cur + i) % SIZE
                b = (cur + l - 1 - i) % SIZE
                lst[a], lst[b] = lst[b], lst[a]

            cur = (cur + l + skip) % SIZE
            skip = (skip + 1) % SIZE

    return lst

def knot(txt):
    l = [ord(i) for i in txt]
    l.extend([17,31,73,47,23])
    h = hash(l, 64)
    lst = []
    for i in range(0, SIZE, 16):
        v = h[i]
        for j in range(i+1, i+16):
            v ^= h[j]
        lst.append("{:02x}".format(v))
    return "".join(lst)

def make_map(txt):
    m = []
    hexs = "0123456789abcdef"
    bits = 0
    for i in range(128):
        row = []
        for c in knot("{}-{}".format(txt, i)):
            i = hexs.find(c)
            for j in range(4):
                if i & 1<<3:
                    row.append("#")
                else:
                    row.append(".")
                i <<= 1
        m.append("".join(row))
    return m

def part1(m):
    return sum(row.count("#") for row in m)

def dfs(m, x, y, visited):
    visited[y][x] = True
    for dx, dy in ((0,-1), (1, 0), (0, 1), (-1, 0)):
        nx = x + dx
        ny = y + dy
        if nx < 0 or nx >= 128 or ny < 0 or ny >= 128:
            continue
        if m[y][x] != "#" or visited[ny][nx]:
            continue
        dfs(m, nx, ny, visited)

def part2(m):
    visited = [[False for v in r] * 128 for r in m]
    groups = 0
    for y, row in enumerate(m):
        for x, val in enumerate(row):
            if val == "#" and not visited[y][x]:
                dfs(m, x, y, visited)
                groups += 1
    return groups

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

    m = make_map(txt)
    for i in range(8):
        print(m[i][:8])
    print("Part1:", part1(m))
    print("Part2:", part2(m))
