#include <cmath>
#include <fstream>
#include <iostream>
#include <fmt/format.h>

using namespace std;

struct Operand
{
	enum { REG, LIT } type;
	int64_t value;
};

istream& operator>>(istream& in, Operand& o)
{
	string tmp;
	in >> tmp;
	if (!in)
	{
		return in;
	}
	if ('a' <= tmp[0] && tmp[0] <= 'h')
	{
		o.type = Operand::REG;
		o.value = tmp[0]-'a';
	}
	else
	{
		o.type = Operand::LIT;
		o.value = stoi(tmp);
	}
	return in;
}

struct Instruction
{
	enum { SET, SUB, MUL, JNZ } op;
	Operand arg1;
	Operand arg2;
};

istream& operator>>(istream& in, Instruction& i)
{
	string tmp;
	in >> tmp;
	if (!in)
	{
		return in;
	}
	if (tmp == "set") i.op = Instruction::SET;
	else if (tmp == "sub") i.op = Instruction::SUB;
	else if (tmp == "mul") i.op = Instruction::MUL;
	else if (tmp == "jnz") i.op = Instruction::JNZ;
	else
	{
		in.setstate(ios::failbit);
		return in;
	}
	in >> i.arg1;
	in >> i.arg2;
	return in;
}

struct Coprocessor
{
	int64_t regs['h'-'a'+1];

	int64_t resolve(const Operand& o)
	{
		return (o.type == Operand::REG) ? regs[o.value] : o.value;
	}

	size_t execute(const vector<Instruction>& program, bool debug)
	{
		size_t mulcount = 0;
		size_t ip = 0;
		fill(regs, regs + ('h'-'a'+1), 0);
		regs[0] = !debug;
		while (ip < program.size())
		{
			if (!debug && ip == 9)
			{
				int src = program[0].arg1.value;
				int dst = program[8].arg1.value;
				int64_t value = regs[src];
				for (int64_t i = 2; i < static_cast<int64_t>(sqrt(value)); i++)
				{
					if (value % i == 0)
					{
						regs[dst] = 0;
						break;
					}
				}
				ip = 24;
			}
			auto &i = program[ip];
			switch (i.op)
			{
			case Instruction::SET:
				regs[i.arg1.value] = resolve(i.arg2);
				ip++;
				break;

			case Instruction::SUB:
				regs[i.arg1.value] -= resolve(i.arg2);
				ip++;
				break;

			case Instruction::MUL:
				regs[i.arg1.value] *= resolve(i.arg2);
				ip++;
				mulcount++;
				break;

			case Instruction::JNZ:
				if (resolve(i.arg1))
				{
					ip += resolve(i.arg2);
				}
				else
				{
					ip++;
				}
				break;
			}
		}
		return debug ? mulcount : regs['h'-'a'];
	}
};

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cerr << fmt::format("Usage: {} <filename>", argv[0]) << endl;
		return 1;
	}

	ifstream input(argv[1]);
	if (!input)
	{
		cerr << fmt::format("Cannot open {}", argv[1]) << endl;
		return 1;
	}
	vector<Instruction> program;
	Instruction instr;
	while (input>>instr)
	{
		program.emplace_back(move(instr));
	}
	input.close();

	Coprocessor c;
	cout << fmt::format("Part1: {}", c.execute(program, true)) << endl;
	cout << fmt::format("Part2: {}", c.execute(program, false)) << endl;
	return 0;
}
