#include <fstream>
#include <iostream>
#include <unordered_map>

#include <fmt/format.h>

using namespace std;

struct Vec
{
	int x;
	int y;

	bool operator==(const Vec& o) const noexcept
	{
		return x == o.x && y == o.y;
	}

	Vec& operator+=(const Vec& o) noexcept
	{
		x += o.x;
		y += o.y;
		return *this;
	}
};

static Vec left(Vec d)
{
	return Vec{d.y, -d.x};
}

static Vec right(Vec d)
{
	return Vec{-d.y, d.x};
}

static Vec reverse(Vec d)
{
	return Vec{-d.x, -d.y};
}

struct HashVec
{
	size_t operator()(const Vec& v) const noexcept
	{
		return (hash<int>{}(v.x) << 16)
			^ (hash<int>{}(v.y));
	}
};

using Map = unordered_map<Vec, int, HashVec>;

static Map make_map(const vector<string>& rows)
{
	Map m;
	for (int y = 0; (size_t)y < rows.size(); y++)
	{
		for (int x = 0; (size_t)x < rows[y].size(); x++)
		{
			if (rows[y][x] == '#')
			{
				m[Vec{x, y}] = '#';
			}
		}
	}
	return m;
}

static int part1(const vector<string>& initial, size_t rounds)
{
	Map m = make_map(initial);
	Vec p{(int)initial[0].size()/2, (int)initial.size()/2};
	Vec d{0, -1};
	size_t infections = 0;
	while (rounds-->0)
	{
		auto it = m.find(p);
		if (it != m.end())
		{
			m.erase(it);
			d = right(d);
		}
		else
		{
			m[p] = '#';
			d = left(d);
			infections++;
		}
		p += d;
	}
	return infections;
}

static int part2(const vector<string>& initial, size_t rounds)
{
	Map m = make_map(initial);
	Vec p{(int)initial[0].size()/2, (int)initial.size()/2};
	Vec d{0, -1};
	size_t infections = 0;
	while (rounds-->0)
	{
		auto it = m.find(p);
		if (it == m.end())
		{
			m[p] = 'W';
			d = left(d);
		}
		else if (it->second == 'W')
		{
			it->second = '#';
			infections++;
		}
		else if (it->second == '#')
		{
			it->second = 'F';
			d = right(d);
		}
		else if (it->second == 'F')
		{
			m.erase(it);
			d = reverse(d);
		}
		p += d;
	}
	return infections;
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
	vector<string> rows;
	string row;
	while (getline(input, row))
	{
		rows.emplace_back(move(row));
	}
	input.close();

	cout << fmt::format("Part1: {}", part1(rows, 10000ULL)) << endl
	     << fmt::format("Part2: {}", part2(rows, 10000000ULL)) << endl;
	return 0;
}
