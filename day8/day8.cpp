#include <climits>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>

#include <fmt/format.h>

using namespace std;

struct Instruction
{
	string dst;
	enum {INC, DEC} op;
	int arg1;
	string src;
	enum {GT, LT, GE, LE, EQ, NE} cmp;
	int arg2;

	bool condition(unordered_map<string, int>& regs) const
	{
		switch (cmp)
		{
		case GT: return regs[src] > arg2;
		case LT: return regs[src] < arg2;
		case GE: return regs[src] >= arg2;
		case LE: return regs[src] <= arg2;
		case EQ: return regs[src] == arg2;
		case NE: return regs[src] != arg2;
		}
		return false;
	}

	void execute(unordered_map<string, int>& regs) const
	{
		switch(op)
		{
		case INC: regs[dst] += arg1; break;
		case DEC: regs[dst] -= arg1; break;
		}
	}
};

istream& operator>>(istream& input, Instruction& i)
{
	static const regex pat("(\\w+) (inc|dec) (-?\\d+) if (\\w+) ([<>=!]+) (-?\\d+)");
	string line;
	if (!getline(input, line))
	{
		return input;
	}

	smatch sm;
	if (!regex_match(line, sm, pat))
	{
		input.setstate(ios::failbit);
		return input;
	}

	i.dst = sm[1];
	i.op = (sm[2] == "inc") ? Instruction::INC : Instruction::DEC;
	i.arg1 = stoi(sm[3]);
	i.src = sm[4];
	if (sm[5] == ">")
	{
		i.cmp = Instruction::GT;
	}
	else if (sm[5] == "<")
	{
		i.cmp = Instruction::LT;
	}
	else if (sm[5] == ">=")
	{
		i.cmp = Instruction::GE;
	}
	else if (sm[5] == "<=")
	{
		i.cmp = Instruction::LE;
	}
	else if (sm[5] == "!=")
	{
		i.cmp = Instruction::NE;
	}
	else if (sm[5] == "==")
	{
		i.cmp = Instruction::EQ;
	}
	i.arg2 = stoi(sm[6]);
	return input;
}

static pair<int,int> max_values(const vector<Instruction>& instr)
{
	int highest = INT_MIN;
	unordered_map<string, int> regs;
	for (auto& i: instr)
	{
		if (i.condition(regs))
		{
			i.execute(regs);
			if (highest < regs[i.dst])
			{
				highest = regs[i.dst];
			}
		}
	}
	auto it = max_element(
		regs.begin(),
		regs.end(),
		[](const auto& a, const auto& b)
		{
			return a.second < b.second;
		});
	return make_pair(it->second, highest);
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

	vector<Instruction> instr;
	Instruction i;
	while (input>>i)
	{
		instr.emplace_back(move(i));
	}
	input.close();

	auto [part1, part2] = max_values(instr);

	cout << fmt::format("Part1: {}", part1) << endl
	     << fmt::format("Part2: {}", part2) << endl;

	return 0;
}
