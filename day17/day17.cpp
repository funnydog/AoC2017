#include <fstream>
#include <iostream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static size_t part1(size_t steps)
{
	vector<size_t> v(1);
	size_t pos = 0;
	for (size_t i = 1; i <= 2017; i++)
	{
		pos = (pos + steps) % v.size() + 1;
		v.insert(v.begin()+pos, i);
	}
	return v[(pos+1) % v.size()];
}

static size_t part2(size_t steps)
{
	size_t pos = 0;
	size_t size = 1;
	size_t last = -1;
	for (size_t i = 1; i <= 50000000ULL; i++)
	{
		pos = (pos + steps) % size + 1;
		size++;
		if (pos == 1)
		{
			last = i;
		}
	}
	return last;
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
	size_t steps;
	input>>steps;
	if (input.fail())
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}
	input.close();

	cout << fmt::format("Part1: {}", part1(steps)) << endl
	     << fmt::format("Part2: {}", part2(steps)) << endl;
	return 0;
}
