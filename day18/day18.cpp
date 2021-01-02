#include <fstream>
#include <iostream>
#include <deque>
#include <vector>

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
	if ('a' <= tmp[0] && tmp[0] <= 'z')
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
	enum { SND, SET, ADD, MUL, MOD, RCV, JGZ } op;
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
	if (tmp == "snd") i.op = Instruction::SND;
	else if (tmp == "set") i.op = Instruction::SET;
	else if (tmp == "add") i.op = Instruction::ADD;
	else if (tmp == "mul") i.op = Instruction::MUL;
	else if (tmp == "mod") i.op = Instruction::MOD;
	else if (tmp == "rcv") i.op = Instruction::RCV;
	else if (tmp == "jgz") i.op = Instruction::JGZ;
	else
	{
		in.setstate(ios::failbit);
		return in;
	}
	switch (i.op)
	{
	case Instruction::SND:
	case Instruction::RCV:
		in >> i.arg1;
		break;

	default:
		in >> i.arg1;
		in >> i.arg2;
		break;
	}

	return in;
}

struct Duet
{
	int64_t regs['z'-'a'+1];
	int64_t ip;
	int64_t freq;
	deque<int64_t> queue;
	struct Duet *other;
	bool naive;
	bool exit;
	size_t send_count;

	Duet(int64_t program_id = -1)
		: regs{},
		  ip(0),
		  freq(0),
		  other(nullptr),
		  naive(program_id==-1),
		  exit(false),
		  send_count(0)
	{
		if (program_id>=0)
		{
			regs['p'-'a'] = program_id;
		}
	}

	void connect(Duet& o)
	{
		other = &o;
	}

	int64_t resolve(const Operand& o)
	{
		return (o.type == Operand::REG) ? regs[o.value] : o.value;
	}

	void play(int64_t value)
	{
		freq = value;
	}

	void send(int64_t value)
	{
		send_count++;
		if (other)
		{
			other->queue.push_back(value);
			other->exit = false;
		}
	}

	bool receive(int64_t& value)
	{
		if (queue.empty())
		{
			return false;
		}
		value = queue.front();
		queue.pop_front();
		return true;
	}

	void execute(const vector<Instruction>& program)
	{
		while (!exit && 0 <= ip && (size_t)ip < program.size())
		{
			auto &i = program[ip];
			switch (i.op)
			{
			case Instruction::SND:
				naive ? play(resolve(i.arg1)) : send(resolve(i.arg1));
				ip++;
				break;

			case Instruction::SET:
				regs[i.arg1.value] = resolve(i.arg2);
				ip++;
				break;

			case Instruction::ADD:
				regs[i.arg1.value] += resolve(i.arg2);
				ip++;
				break;

			case Instruction::MUL:
				regs[i.arg1.value] *= resolve(i.arg2);
				ip++;
				break;

			case Instruction::MOD:
				regs[i.arg1.value] %= resolve(i.arg2);
				ip++;
				break;

			case Instruction::RCV:
				if (naive)
				{
					if (resolve(i.arg1))
					{
						exit = true;
					}
					ip++;
				}
				else
				{
					if (!receive(regs[i.arg1.value]))
					{
						exit = true;
					}
					else
					{
						ip++;
					}
				}
				break;

			case Instruction::JGZ:
				if (resolve(i.arg1) > 0)
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
	}
};

static size_t part1(const vector<Instruction>& program)
{
	Duet d;
	d.execute(program);
	return d.freq;
}

static size_t part2(const vector<Instruction>& program)
{
	Duet a(0), b(1);
	a.connect(b);
	b.connect(a);
	while (!a.exit || !b.exit)
	{
		a.execute(program);
		b.execute(program);
	}
	return b.send_count;
}

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

	cout << fmt::format("Part1: {}", part1(program)) << endl
	     << fmt::format("Part2: {}", part2(program)) << endl;
	return 0;
}
