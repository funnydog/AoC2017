#!/usr/bin/env python3

def captcha(txt, offset):
    size = len(txt)
    return sum(int(txt[i]) for i in range(size) if txt[i] == txt[(i+offset)%size])

# assert captcha("1122", 1) == 3
# assert captcha("1111", 1) == 4
# assert captcha("1234", 1) == 0
# assert captcha("91212129", 1) == 9

# assert captcha("1212", 2) == 6
# assert captcha("1221", 2) == 0
# assert captcha("123425", 3) == 4
# assert captcha("123123", 3) == 12
# assert captcha("12131415", 4) == 4

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

    print("Part1:", captcha(txt, 1))
    print("Part2:", captcha(txt, len(txt)//2))
