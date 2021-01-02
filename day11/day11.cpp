#include <fstream>
#include <iostream>
#include <unordered_map>

#include <fmt/format.h>

using namespace std;

// mapping between hex and 2d grid
//      [n] [ne]
//       | /
// [nw]-[0]-[se]
//     / |
// [sw] [s]
struct Pos
{
	int q;
	int r;
};

static size_t distance(Pos a)
{
	int z = a.q + a.r;
	return ((a.q>0?a.q:-a.q)
		+ (a.r>0?a.r:-a.r)
		+ (z>0?z:-z)) / 2;
}

static pair<size_t, size_t> walk(const string& dir)
{
	Pos p{0,0};
	size_t maxdist = 0;
	size_t dist = 0;
	size_t pos = 0;
	while (pos < dir.size())
	{
		if (dir.rfind("ne", pos) == pos)
		{
			p.q++; p.r--;
			pos+=3;
		}
		else if (dir.rfind("se", pos) == pos)
		{
			p.q++;
			pos+=3;
		}
		else if (dir.rfind("nw", pos) == pos)
		{
			p.q--;
			pos+=3;
		}
		else if (dir.rfind("sw", pos) == pos)
		{
			p.q--; p.r++;
			pos+=3;
		}
		else if (dir.rfind("s", pos) == pos)
		{
			p.r++;
			pos+=2;
		}
		else if (dir.rfind("n", pos) == pos)
		{
			p.r--;
			pos+=2;
		}
		else
		{
			break;
		}

		dist = distance(p);
		if (maxdist < dist)
		{
			maxdist = dist;
		}
	}

	return make_pair(dist, maxdist);
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
	string line;
	getline(input, line);
	input.close();
	if (input.fail())
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}

	auto [part1, part2] = walk(line);
	cout << fmt::format("Part1: {}", part1) << endl
	     << fmt::format("Part2: {}", part2) << endl;
	return 0;
}
