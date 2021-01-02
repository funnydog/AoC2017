#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include <fmt/format.h>

using namespace std;

static vector<pair<int, int>> load(istream& in)
{
	vector<pair<int,int>> v;
	while (true)
	{
		int a, b;
		in >> a;
		in.get();
		in >> b;
		if (in.fail())
		{
			break;
		}
		v.push_back(make_pair(a, b));
	}
	sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
		return a.second < b.second;
	});
	return v;
}

static size_t part1(const vector<pair<int, int>>& fw)
{
	return accumulate(
		fw.begin(),
		fw.end(),
		0ULL,
		[](auto acc, const auto& p) {
			return (p.first%(2 * p.second-2) == 0)
				? (acc + p.first * p.second)
				: acc;
		});
}

static size_t part2(const vector<pair<int,int>>& fw)
{
	int j = 0;
	while (true)
	{
		if (!any_of(fw.begin(), fw.end(), [j](const auto& p) {
			return (p.first + j) % (2*p.second -2) == 0;
		})) {
			break;
		}
		j++;
	}
	return j;
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
	auto fw = load(input);
	input.close();

	cout << fmt::format("Part1: {}", part1(fw)) << endl
	     << fmt::format("Part2: {}", part2(fw)) << endl;
	return 0;
}
