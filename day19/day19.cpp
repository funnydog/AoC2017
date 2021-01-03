#include <fstream>
#include <iostream>
#include <vector>

#include <fmt/format.h>

using namespace std;

struct Pos
{
	int x;
	int y;

	Pos left() const noexcept
	{
		return Pos{-y, x};
	}

	Pos right() const noexcept
	{
		return Pos{y, -x};
	}
};

static Pos operator+(Pos a, Pos b)
{
	return Pos{a.x+b.x, a.y+b.y};
}

static pair<string, size_t> follow(const vector<string>& m)
{
	// find the starting point
	Pos d{0, 1};
	Pos p{0, 0};
	for (size_t x = 0; x < m[0].size(); x++)
	{
		if (m[0][x] == '|')
		{
			p.x = x;
			break;
		}
	}

	string letters;
	size_t steps = 0;
	while (true)
	{
		if ("+|- "s.find(m[p.y][p.x]) == string::npos)
		{
			// collect the letters in the path
			letters.push_back(m[p.y][p.x]);
		}

		if (m[p.y][p.x] == '+')
		{
			// find the new direction
			Pos nds[2] = { d.left(), d.right() };
			for (auto& nd: nds)
			{
				Pos np = p + nd;
				if (0 <= np.y && (size_t)np.y < m.size()
				    && 0 <= np.x && (size_t)np.x < m[np.y].size()
				    && m[np.y][np.x] != ' ')
				{
					d = nd;
					break;
				}
			}
		}
		else if (m[p.y][p.x] == ' ')
		{
			// end of the path
			break;
		}

		p = p + d;
		steps++;
	}

	return make_pair(move(letters), steps);
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
	vector<string> m;
	string row;
	while (getline(input, row))
	{
		m.emplace_back(move(row));
	}
	input.close();

	auto [part1, part2] = follow(m);
	cout << fmt::format("Part1: {}", part1) << endl
	     << fmt::format("Part2: {}", part2) << endl;
	return 0;
}
