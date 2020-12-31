#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static size_t checksum1(const vector<vector<int>>& rows)
{
	return accumulate(
		rows.begin(),
		rows.end(),
		0ULL,
		[](auto acc, const auto& row) {
			auto [min, max] = minmax_element(row.begin(), row.end());
			if (min != row.end())
			{
				return acc + *max - *min;
			}
			else
			{
				return acc;
			}
		});
}

static size_t checksum2(const vector<vector<int>>& rows)
{
	return accumulate(
		rows.begin(),
		rows.end(),
		0ULL,
		[](auto acc, const auto& row) {
			for (size_t i = 1; i < row.size(); i++)
			{
				for (size_t j = 0; j < i; j++)
				{
					if (row[i] % row[j] == 0)
					{
						return acc + row[i] / row[j];
					}
					else if (row[j] % row[i] == 0)
					{
						return acc + row[j] / row[i];
					}
				}
			}
			return acc;
		});
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

	vector<vector<int>> rows;
	string line;
	while (getline(input, line))
	{
		istringstream sin(line);
		vector<int> numbers;
		int number;
		while (sin>>number)
		{
			numbers.push_back(number);
		}
		rows.emplace_back(move(numbers));
	}
	input.close();

	cout << fmt::format("Part1: {}", checksum1(rows)) << endl
	     << fmt::format("Part2: {}", checksum2(rows)) << endl;
	return 0;
}
