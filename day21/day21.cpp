#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

#include <fmt/format.h>

using namespace std;

static string rotate(const string& src)
{
	string dst(src);
	size_t l = src.find('/');
	size_t p = 0;
	for (size_t y = 0; y < l; y++)
	{
		for (size_t x = 0; x < l; x++)
		{
			dst[p++] = src[x*(l+1) + l-y-1];
		}
		p++;
	}
	return dst;
}

static string flip(const string &src)
{
	string dst(src);
	size_t l = src.find('/');
	size_t p = 0;
	for (size_t y = 0; y < l; y++)
	{
		for (size_t x = 0; x < l; x++)
		{
			dst[p++] = src[y*(l+1)+l-x-1];
		}
		p++;
	}
	return dst;
}

static unordered_map<string, string> load_rules(istream& in)
{
	unordered_map<string,string> rules;
	string src, dst;
	while (true)
	{
		in >> src;
		in >> dst;		// separator
		in >> dst;
		if (in.fail())
		{
			break;
		}
		for (int i = 0; i < 8; i++)
		{
			if (i == 4) src = flip(src);
			src = rotate(src);
			rules[src] = dst;
		}
	}
	return rules;
}

static string get_square(const vector<string>& map, size_t px, size_t py, size_t l)
{
	string dst;
	for (size_t y = py; y < py+l; y++)
	{
		for (size_t x = px; x < px+l; x++)
		{
			dst.push_back(map[y][x]);
		}
		dst.push_back('/');
	}
	dst.pop_back();
	return dst;
}

static void set_square(vector<string>& map, size_t px, size_t py, const string& dst)
{
	size_t l = dst.find('/');
	assert(l != string::npos);
	size_t p = 0;
	for (size_t y = py; y < py+l; y++)
	{
		for (size_t x = px; x < px+l; x++)
		{
			map[y][x] = dst[p];
			p++;
		}
		p++;
	}
}

static vector<string> step(const vector<string>& map,
			   const unordered_map<string,string>& rules)
{
	size_t srclen, dstlen;
	if (map.size() % 2)
	{
		srclen = 3;
		dstlen = 4;
	}
	else
	{
		srclen = 2;
		dstlen = 3;
	}

	size_t r = map.size() / srclen;
	vector<string> newmap(r*dstlen);
	for (auto& row: newmap)
	{
		row.resize(r*dstlen);
	}

	for (size_t y = 0; y < r; y++)
	{
		for (size_t x = 0; x < r; x++)
		{
			string src = get_square(map, x * srclen, y * srclen, srclen);
			auto dst = rules.find(src);
			if (dst != rules.end())
			{
				set_square(newmap, x * dstlen, y * dstlen, dst->second);
			}
			else
			{
				cerr << "rule not found for" << src << endl;
			}
		}
	}
	return newmap;
}

static size_t count_lit(const unordered_map<string,string>& rules, size_t rounds)
{
	vector<string> map = { ".#.", "..#", "###"};
	while (rounds-->0)
	{
		map = step(map, rules);
	}
	return accumulate(
		map.begin(),
		map.end(),
		0ULL,
		[](auto acc, const auto& row) {
			return acc + count(row.begin(), row.end(), '#');
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
	auto rules = load_rules(input);
	input.close();

	cout << fmt::format("Part1: {}", count_lit(rules, 5)) << endl
	     << fmt::format("Part2: {}", count_lit(rules, 18)) << endl;
	return 0;
}
