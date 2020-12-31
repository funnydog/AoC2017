#!/usr/bin/env python3

def parse_data(txt):
    rows = []
    for line in txt.splitlines():
        rows.append(list(map(int, line.split("\t"))))
    return rows

def checksum1(rows):
    value = 0
    for row in rows:
        value += max(row) - min(row)
    return value

def checksum2(rows):
    value = 0
    for row in rows:
        for i in range(1,len(row)):
            for j in range(i):
                if row[i] % row[j] == 0:
                    value += row[i] // row[j]
                elif row[j] % row[i] == 0:
                    value += row[j] // row[i]
    return value

# assert checksum1(parse_data("""5\t1\t9\t5
# 7\t5\t3
# 2\t4\t6\t8""")) == 18

# assert checksum2(parse_data("""5\t9\t2\t8
# 9\t4\t7\t3
# 3\t8\t6\t5""")) == 9

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

    rows = parse_data(txt)
    print("Part1:", checksum1(rows))
    print("Part2:", checksum2(rows))
