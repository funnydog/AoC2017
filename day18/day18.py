#!/usr/bin/env python3

import string

class Instruction(object):
    def __init__(self, txt):
        lst = txt.split(" ")
        self.op = lst[0]
        if lst[0] in ("snd", "rcv"):
            self.arg1 = lst[1]
            self.arg2 = None
        elif lst[0] in ("set", "add", "mul", "mod", "jgz"):
            self.arg1 = lst[1]
            self.arg2 = lst[2]

class Duet(object):
    def __init__(self, p = None):
        self.naive = p is None
        self.other = None
        self.send_count = 0
        self.reset(p)

    def reset(self, p):
        self.register = {v: 0 for v in string.ascii_lowercase}
        if not p is None:
            self.register["p"] = p
        self.queue = []
        self.ip = 0
        self.freq = None
        self.exit = False

    def connect(self, other):
        self.other = other

    def resolve(self, value):
        v = self.register.get(value)
        if not v is None:
            return v
        else:
            return int(value)

    def send(self, x):
        self.send_count += 1
        if self.other:
            self.other.queue.append(x)
            self.other.exit = False

    def receive(self):
        if self.queue:
            return self.queue.pop(0)
        self.exit = True

    def play(self, x):
        self.freq = x

    def recover(self):
        self.exit = True

    def execute(self, program):
        while 0 <= self.ip < len(program) and not self.exit:
            i = program[self.ip]
            if i.op == "snd":
                if self.naive:
                    self.play(self.resolve(i.arg1))
                else:
                    self.send(self.resolve(i.arg1))
                self.ip += 1
            elif i.op == "set":
                self.register[i.arg1] = self.resolve(i.arg2)
                self.ip += 1
            elif i.op == "add":
                self.register[i.arg1] += self.resolve(i.arg2)
                self.ip += 1
            elif i.op == "mul":
                self.register[i.arg1] *= self.resolve(i.arg2)
                self.ip += 1
            elif i.op == "mod":
                self.register[i.arg1] %= self.resolve(i.arg2)
                self.ip += 1
            elif i.op == "rcv":
                if self.naive:
                    if self.resolve(i.arg1):
                        self.recover()
                    self.ip += 1
                else:
                    value = self.receive()
                    if not value is None:
                        self.register[i.arg1] = value
                        self.ip += 1
            elif i.op == "jgz":
                if self.resolve(i.arg1) > 0:
                    self.ip += self.resolve(i.arg2)
                else:
                    self.ip += 1

def part1(program):
    d = Duet()
    d.execute(program)
    return d.freq

def part2(program):
    a = Duet(0)
    b = Duet(1)
    a.connect(b)
    b.connect(a)
    while not a.exit or not b.exit:
        a.execute(program)
        b.execute(program)
    return b.send_count

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

    program = [Instruction(v) for v in txt.splitlines()]
    print("Part1:", part1(program))
    print("Part2:", part2(program))
