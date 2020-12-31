#!/usr/bin/env python3

def is_valid(passphrase, anagram):
    m = set()
    for w in passphrase.split(" "):
        if anagram:
            w = "".join(sorted(w))
        if w in m:
            return False
        m.add(w)
    return True

def count_valid(phrases, anagram):
    return sum(1 for phrase in phrases if is_valid(phrase, anagram))

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

    phrases = txt.splitlines();
    print("Part1:", count_valid(phrases, False))
    print("Part2:", count_valid(phrases, True))
