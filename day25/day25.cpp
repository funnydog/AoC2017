#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <fmt/format.h>

using namespace std;

struct Instruction
{
	int curstate;
	int curval;
	int newval;
	int move;
	int nxtstate;
};

struct Machine
{
	int state;
	size_t checksum;
	unordered_map<int, int> tape;
	vector<Instruction> instructions;

	size_t run() noexcept
	{
		int p = 0;
		for (size_t j = 0; j < checksum; j++)
		{
			auto value = tape[p];
			auto it = find_if(instructions.begin(),
					  instructions.end(),
					  [this, value](const auto& i) {
						  return i.curstate == state && i.curval == value;
					  });
			if (it != instructions.end())
			{
				tape[p] = it->newval;
				p += it->move;
				state = it->nxtstate;
			}
		}
		return count_if(
			tape.begin(),
			tape.end(),
			[](const auto& p) {
				return p.second == 1;
			});
	}
};

istream& operator>>(istream& in, Machine& m)
{
	Instruction i;
	string line;
	while (getline(in, line))
	{
		if (line.rfind("Begin in state ", 0) == 0)
		{
			m.state = line[15];
		}
		else if (line.rfind("Perform a diagnostic checksum after ", 0) == 0)
		{
			m.checksum = stoull(line.substr(36));
		}
		else if (line.rfind("In state ", 0) == 0)
		{
			i.curstate = line[9];
		}
		else if (line.rfind("  If the current value is ", 0) == 0)
		{
			i.curval = stoi(line.substr(26));
		}
		else if (line.rfind("    - Write the value ", 0) == 0)
		{
			i.newval = stoi(line.substr(22));
		}
		else if (line.rfind("    - Move one slot to the ", 0) == 0)
		{
			i.move = line.substr(27) == "right." ? 1 : -1;
		}
		else if (line.rfind("    - Continue with state ", 0) == 0)
		{
			i.nxtstate = line[26];
			m.instructions.emplace_back(move(i));
		}
	}
	return in;
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
	Machine m;
	input >> m;
	input.close();

	cout << fmt::format("Part1: {}", m.run()) << endl;
	return 0;
}
