#include <fstream>
#include <iostream>
#include <vector>

#include <fmt/format.h>

using namespace std;

using Component = pair<int,int>;

static vector<pair<int,int>> load(istream& input)
{
	vector<pair<int,int>> comps;
	int a, b;
	while (true)
	{
		input >> a;
		input.get();
		input >> b;
		if (!input)
		{
			break;
		}
		comps.emplace_back(make_pair(a, b));
	}
	return comps;
}

static size_t maxstrength;
static size_t maxlen;
static size_t maxlenstrength;

static void backtrack(size_t strength, size_t len, int free, vector<pair<int,int>>& comps)
{
	if (maxlen <= len)
	{
		maxlen = len;
		if (maxlenstrength < strength)
		{
			maxlenstrength = strength;
		}
	}
	if (maxstrength < strength)
	{
		maxstrength = strength;
	}

	for (size_t i = 0; i < comps.size(); i++)
	{
		auto t = comps[i];
		if (t.first != free && t.second != free)
		{
			continue;
		}
		int other = t.first == free ? t.second : t.first;
		comps.erase(comps.begin() + i);
		backtrack(strength + free + other, len+1, other, comps);
		comps.insert(comps.begin() + i, t);
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
	auto comps = load(input);
	input.close();

	backtrack(0, 0, 0, comps);
	cout << fmt::format("Part1: {}", maxstrength) << endl
	     << fmt::format("Part2: {}", maxlenstrength) << endl;
	return 0;
}
