#!/usr/bin/env python3

def parse(txt):
    begin = None
    steps = None
    states = {}
    state, current, new, move, nxt = None, None, None, None, None
    for line in txt.splitlines():
        if line.startswith("Begin in state "):
            begin = line[15]
        elif line.startswith("Perform a diagnostic checksum after "):
            steps = int(line[36:].split(" ")[0])
        elif line.startswith("In state "):
            state = line[9]
        elif line.startswith("  If the current value is "):
            current = int(line[26:-1])
        elif line.startswith("    - Write the value "):
            new = int(line[22:-1])
        elif line.startswith("    - Move one slot to the "):
            move = (line[27:] == "right.") and 1 or -1
        elif line.startswith("    - Continue with state "):
            nxt = line[26]
            states[state, current] = (new, move, nxt)

    return begin, steps, states

def run(txt):
    curstate, steps, states = parse(txt)
    v = {}
    p = 0
    for i in range(steps):
        curval = v.get(p, 0)
        newval, move, nxt = states[curstate, curval]
        v[p] = newval
        p += move
        curstate = nxt

    return sum(1 for i in v.values() if i == 1)

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

    print(run(txt))
    # TODO: implement the code
    # print("Part1:", tls)
    # print("Part2:", ssl)
