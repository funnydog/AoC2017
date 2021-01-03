#!/usr/bin/env python3

import math

class Instruction(object):
    def __init__(self, txt):
        self.op, self.arg1, self.arg2 = txt.split(" ")

class Coprocessor(object):
    def __init__(self, debug = True):
        self.reset(debug)

    def reset(self, debug):
        self.debug = debug
        self.register = {v: 0 for v in "abcdefgh"}
        self.ip = 0
        if not debug:
            self.register["a"] = 1

    def resolve(self, value):
        v = self.register.get(value)
        if not v is None:
            return v
        else:
            return int(value)

    def execute(self, program):
        mulcount = 0
        while 0 <= self.ip < len(program):
            if not self.debug and self.ip == 9:
                # rough primality test
                srcreg = program[0].arg1
                dstreg = program[8].arg1
                value = self.register[srcreg]
                for i in range(2, int(math.sqrt(value)+1)):
                    if value % i == 0:
                        self.register[dstreg] = 0
                        break
                self.ip = 24
            i = program[self.ip]
            if i.op == "set":
                self.register[i.arg1] = self.resolve(i.arg2)
                self.ip += 1
            elif i.op == "sub":
                self.register[i.arg1] -= self.resolve(i.arg2)
                self.ip += 1
            elif i.op == "mul":
                mulcount += 1
                self.register[i.arg1] *= self.resolve(i.arg2)
                self.ip += 1
            elif i.op == "jnz":
                if self.resolve(i.arg1) != 0:
                    self.ip += self.resolve(i.arg2)
                else:
                    self.ip += 1

        if self.debug:
            return mulcount
        else:
            return self.register["h"]

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

    program = [Instruction(row) for row in txt.splitlines()]
    c = Coprocessor()
    print("Part1:", c.execute(program))
    c.reset(False)
    print("Part2:", c.execute(program))
