#include <fstream>
#include <iostream>

#include <fmt/format.h>

using namespace std;

static pair<size_t, size_t> load(istream &input)
{
	size_t r[2] = {};
	for (size_t i = 0; i < 2; i++)
	{
		string line;
		getline(input, line);
		if (input.fail())
		{
			break;
		}
		size_t pos = 0;
		size_t next;
		while ((next = line.find(' ', pos)) != string::npos)
		{
			pos = next + 1;
		}
		r[i] = stoi(line.substr(pos));
	}
	return make_pair(r[0], r[1]);
}

static size_t part1(size_t a, size_t b)
{
	size_t count = 0;
	for (size_t i = 0; i < 40000000ULL; i++)
	{
		a = (a * 16807) % 2147483647ULL;
		b = (b * 48271) % 2147483647ULL;
		count += (a & 0xffff) == (b & 0xffff);
	}
	return count;
}

static size_t part2(size_t a, size_t b)
{
	size_t count = 0;
	for (size_t i = 0; i < 5000000ULL; i++)
	{
		do
		{
			a = (a * 16807) % 2147483647ULL;
		} while (a & 3);

		do
		{
			b = (b * 48271) % 2147483647ULL;
		} while (b & 7);

		count += (a & 0xffff) == (b & 0xffff);
	}
	return count;
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
	auto [a, b] = load(input);
	input.close();
	if (input.fail())
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}

	cout << fmt::format("Part1: {}", part1(a, b)) << endl
	     << fmt::format("Part2: {}", part2(a, b)) << endl;
	return 0;
}
