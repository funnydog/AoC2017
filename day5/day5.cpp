#include <fstream>
#include <iostream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static size_t count_steps(vector<int> offsets, bool strange)
{
	size_t steps = 0;
	int i = 0;
	while (0 <= i && (size_t)i < offsets.size())
	{
		int n = offsets[i];
		if (strange && n >= 3)
		{
			offsets[i]--;
		}
		else
		{
			offsets[i]++;
		}

		i += n;
		steps++;
	}
	return steps;
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
	vector<int> offsets;
	int offset;
	while (input>>offset)
	{
		offsets.push_back(offset);
	}
	input.close();

	cout << fmt::format("Part1: {}", count_steps(offsets, false)) << endl
	     << fmt::format("Part2: {}", count_steps(offsets, true)) << endl;
	return 0;
}
