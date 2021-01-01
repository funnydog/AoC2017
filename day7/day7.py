#!/usr/bin/env python3

class Program(object):
    def __init__(self, name):
        self.name = name
        self.weight = 0
        self.adj  = []
        self.visited = False

def parse(txt):
    m = {}
    for line in txt.splitlines():
        state = 0
        cur = None
        for word in line.split(" "):
            if state == 0:
                if word[0] == '(':
                    m[cur].weight = int(word[1:-1])
                elif word == "->":
                    state = 1
                else:
                    m[word] = Program(word)
                    cur = word
            else:
                if word[-1] == ",":
                    word = word[:-1]

                m[cur].adj.append(word)
    return  m

def dfs(m, k, path):
    m[k].visited = True
    for ka in m[k].adj:
        if not m[ka].visited:
            dfs(m, ka, path)
    path.append(k)

def topsort(m):
    path = []
    for k, p in m.items():
        if not p.visited:
            dfs(m, k, path)

    return path

def weights(m, path):
    weights = {}
    for pname in path:
        p = m[pname]
        weights[pname] = p.weight
        s = [weights[name] for name in p.adj]
        if not s:
            continue

        weights[pname] += sum(s)
        mins = min(s)
        maxs = max(s)
        if mins == maxs:
            continue

        cmin = sum(1 for i in s if i == mins)
        if cmin == 1:
            return m[p.adj[s.index(mins)]].weight + (maxs - mins)
        else:
            return m[p.adj[s.index(maxs)]].weight - (maxs - mins)

    return 0

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

    m = parse(txt)
    sort = topsort(m)
    print("Part1:", sort[-1])
    print("Part2:", weights(m, sort))
