#!/usr/bin/env python3

def count_steps(txt, strange):
    lst = list(map(int, txt.splitlines()))
    steps = 0
    i = 0
    size = len(lst)
    while 0 <= i < size:
        n = lst[i]
        if strange and n >= 3:
            lst[i] -= 1
        else:
            lst[i] += 1

        i += n
        steps += 1

    return steps

# assert count_steps("""0
# 3
# 0
# 1
# -3""", False) == 5
# assert count_steps("""0
# 3
# 0
# 1
# -3""", True) == 10

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

    print("Part1:", count_steps(txt, False))
    print("Part2:", count_steps(txt, True))
