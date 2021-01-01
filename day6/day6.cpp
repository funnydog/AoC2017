#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static void next(vector<int>& banks)
{
	auto mit = max_element(banks.begin(), banks.end());
	size_t size = banks.size();
	int m = *mit;
	*mit = 0;
	size_t i = mit - banks.begin();
	while (m)
	{
		i = (i+1) % size;
		banks[i]++;
		m--;
	}
}

static pair<size_t, size_t> floyd(const vector<int>& banks)
{
	vector<int> tortoise = banks;
	vector<int> hare = banks;
	do
	{
		next(tortoise);
		next(hare);
		next(hare);
	} while (tortoise != hare);

	size_t mu = 0;
	tortoise = banks;
	while (tortoise != hare)
	{
		next(tortoise);
		next(hare);
		mu++;
	}

	size_t lam = 0;
	hare = tortoise;
	do
	{
		next(hare);
		lam++;
	} while (hare != tortoise);

	return make_pair(mu+lam, lam);
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
	vector<int> banks;
	int bank;
	while(input>>bank)
	{
		banks.push_back(bank);
	}
	input.close();

	auto [part1, part2] = floyd(banks);
	cout << fmt::format("Part1: {}", part1) << endl
	     << fmt::format("Part2: {}", part2) << endl;
	return 0;
}
