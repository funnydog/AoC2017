#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

#include <fmt/format.h>

using namespace std;

struct Vec
{
	int v[3];

	Vec& operator+=(const Vec& o) noexcept
	{
		v[0] += o.v[0];
		v[1] += o.v[1];
		v[2] += o.v[2];
		return *this;
	}

	bool operator==(const Vec& o) const noexcept
	{
		return v[0] == o.v[0] && v[1] == o.v[1] && v[2] == o.v[2];
	}

	size_t manhattan() const noexcept
	{
		return (v[0]>0?v[0]:-v[0]) +
			(v[1]>0?v[1]:-v[1]) +
			(v[2]>0?v[2]:-v[2]);
	}
};

namespace std
{
	template<>
	struct hash<Vec>
	{
		size_t operator()(const Vec& v) const noexcept
		{
			return (hash<int>{}(v.v[0]) << 32)
				^ (hash<int>{}(v.v[1]) << 16)
				^ (hash<int>{}(v.v[2]) << 0);
		}
	};
}

struct Particle
{
	size_t n;
	Vec p;
	Vec v;
	Vec a;

	void update() noexcept
	{
		v += a;
		p += v;
	}

	bool stable(int axis) const noexcept
	{
		return (a.v[axis] == 0 || (v.v[axis] ^ a.v[axis]) > 0)
			&& (v.v[axis] == 0 || (p.v[axis] ^ v.v[axis]) > 0);
	}
};

struct World
{
	vector<Particle> parts;

	void update(bool collide)
	{
		unordered_map<Vec, int> coll;
		for (auto& p: parts)
		{
			p.update();
			if (collide)
			{
				coll[p.p] += 1;
			}
		}
		if (collide)
		{
			parts.erase(
				remove_if(parts.begin(), parts.end(),
					  [&coll](const auto& p){
						  return coll[p.p] > 1;
					  }),
				parts.end()
				);
		}
	}

	bool stable(int axis) const
	{
		return all_of(
			parts.begin(),
			parts.end(),
			[axis](const auto& p){
				return p.stable(axis);
			});
	}
};

static size_t part1(World w)
{
	for (int axis = 0; axis < 3; axis++)
	{
		while (!w.stable(axis))
		{
			w.update(false);
		}
	}
	w.update(false);
	auto it = min_element(w.parts.begin(), w.parts.end(), [](const auto& a, const auto& b) {
		return a.p.manhattan() < b.p.manhattan();
	});
	return it->n;
}

static size_t part2(World w)
{
	for (int axis = 0; axis < 3; axis++)
	{
		while (!w.stable(axis))
		{
			w.update(true);
		}
	}
	w.update(true);
	return w.parts.size();
}

istream& operator>>(istream& input, World& w)
{
	static const regex pat("p=<\\s*(-?\\d+),(-?\\d+),(-?\\d+)>, v=<\\s*(-?\\d+),(-?\\d+),(-?\\d+)>, a=<\\s*(-?\\d+),(-?\\d+),(-?\\d+)>");

	string line;
	while (getline(input, line))
	{
		smatch sm;
		if (!regex_match(line, sm, pat))
		{
			continue;
		}
		Vec p{stoi(sm[1]), stoi(sm[2]), stoi(sm[3])};
		Vec v{stoi(sm[4]), stoi(sm[5]), stoi(sm[6])};
		Vec a{stoi(sm[7]), stoi(sm[8]), stoi(sm[9])};
		w.parts.emplace_back(Particle{w.parts.size(), p, v, a});
	}
	return input;
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
	World w;
	input>>w;
	input.close();
	if (w.parts.size() == 0)
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}
	cout << fmt::format("Part1: {}", part1(w)) << endl
	     << fmt::format("Part2: {}", part2(w)) << endl;
	return 0;
}
