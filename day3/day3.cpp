#include <fstream>
#include <iostream>
#include <unordered_map>

#include <fmt/format.h>

using namespace std;

static size_t count_steps(size_t to)
{
	size_t count = 0;
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 1;
	for (;;)
	{
		count++;
		if (count == to)
		{
			return (x>0?x:-x) + (y>0?y:-y);
		}

		if (x == -y || (x < 0 && x == y) || (x > 0 and x == y + 1))
		{
			swap(dx, dy);
			dy = -dy;
		}
		x += dx;
		y += dy;
	}
}

namespace std
{
	template<>
	struct hash<pair<int,int>>
	{
		size_t operator()(const pair<int,int>& p) const noexcept
		{
			return (hash<int>{}(p.first)<<4)^(hash<int>{}(p.second));
		}
	};
}

static size_t first_larger(size_t to)
{
	static const int ax[] = {1,1,0,-1,-1,-1,0,1};
	static const int ay[] = {0,-1,-1,-1,0,1,1,1};
	unordered_map<pair<int,int>, size_t> grid;
	grid[make_pair(0, 0)] = 1;
	int x = 1;
	int y = 0;
	int dx = 1;
	int dy = 0;
	for (;;)
	{
		size_t adj = 0;
		for (size_t i = 0; i < 8; i++)
		{
			adj += grid[make_pair(x+ax[i], y+ay[i])];
		}

		if (adj > to)
		{
			return adj;
		}

		grid[make_pair(x, y)] = adj;

		if (x == -y || (x < 0 && x == y) || (x > 0 and x == y + 1))
		{
			swap(dx, dy);
			dy = -dy;
		}
		x += dx;
		y += dy;
	}
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
	size_t to;
	input >> to;
	input.close();
	if (input.fail())
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}

	cout << fmt::format("Part1: {}", count_steps(to)) << endl
	     << fmt::format("Part2: {}", first_larger(to)) << endl;
	return 0;
}
