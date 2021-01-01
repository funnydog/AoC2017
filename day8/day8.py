#!/usr/bin/env python3

def parse(txt):
    lst = []
    for line in txt.splitlines():
        lst.append(line.split(" "))
    return lst

def test(regs, r, cm, arg):
    val = regs.get(r, 0)
    arg = int(arg)
    if cm == ">":
        return val > arg
    elif cm == "<":
        return val < arg
    elif cm == ">=":
        return val >= arg
    elif cm == "==":
        return val == arg
    elif cm == "<=":
        return val <= arg
    elif cm == "!=":
        return val != arg
    else:
        print("ERROR")
        exit(0)

def exec_instr(regs, r, op, arg):
    val = regs.get(r, 0)
    arg = int(arg)
    if op == "inc":
        val += arg
    elif op == "dec":
        val -= arg
    else:
        print("ERROR")
        exit(0)
    regs[r] = val

def max_values(txt):
    highest = -1e12
    regs = {}
    for r1, op, arg1, _, r2, cm, arg2 in parse(txt):
        if test(regs, r2, cm, arg2):
            exec_instr(regs, r1, op, arg1)
            if highest < regs[r1]:
                highest = regs[r1]
    return max(regs.values()), highest

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

    part1, part2 = max_values(txt)
    print("Part1:", part1)
    print("Part2:", part2)
