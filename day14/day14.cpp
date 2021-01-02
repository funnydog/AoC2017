#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include <fmt/format.h>

using namespace std;

static const size_t SIZE = 256;
static const size_t HASH_SIZE = 16;

static void hashfn(const vector<size_t>& lengths, size_t rounds, unsigned char *out)
{
	for (size_t i = 0; i < SIZE; i++)
	{
		out[i] = i;
	}

	size_t cur = 0;
	size_t skip = 0;
	while (rounds-->0)
	{
		for(auto l: lengths)
		{
			// reverse from cur to l-1
			for (size_t i = 0; i < l/2; i++)
			{
				size_t a = (cur + i) % SIZE;
				size_t b = (cur + l - i - 1) % SIZE;
				swap(out[a], out[b]);
			}
			cur = (cur + l + skip) % SIZE;
			skip++;
		}
	}
}

static void knot(const string& line, unsigned char *out)
{
	vector<size_t> lengths(line.begin(), line.end());
	const size_t seq[] = {17, 31, 73, 47, 23};
	lengths.insert(lengths.end(), seq, seq+5);

	unsigned char buf[SIZE];
	hashfn(lengths, 64, buf);

	for (size_t i = 0; i < 256; i += 16)
	{
		unsigned char v = buf[i];
		for (size_t j = i+1; j < i+16; j++)
		{
			v ^= buf[j];
		}
		*out++ = v;
	}
}

static vector<string> make_map(const string& salt)
{
	vector<string> map;
	for (int i = 0; i < 128; i++)
	{
		unsigned char buf[HASH_SIZE];
		knot(fmt::format("{}-{}", salt, i), buf);

		string s;
		for (size_t j = 0; j < HASH_SIZE; j++)
		{
			for (int k = 7; k >= 0; k--)
			{
				s.push_back((buf[j] & (1U<<k)) ? '#' : '.');
			}
		}
		map.emplace_back(move(s));
	}
	return map;
}

static size_t part1(const vector<string>& map)
{
	return accumulate(
		map.begin(),
		map.end(),
		0ULL,
		[](auto acc, const auto& row) {
			return acc + count(row.begin(), row.end(), '#');
		});
}

static void dfs(const vector<string>&m, int x, int y, vector<vector<bool>>& visited)
{
	static const int dx[] = {0, 1, 0, -1};
	static const int dy[] = {1, 0, -1, 0};
	visited[y][x] = true;
	for (size_t i = 0; i < 4; i++)
	{
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (0 <= nx && nx < 128 && 0 <= ny && ny < 128
		    && m[ny][nx] == '#' && !visited[ny][nx])
		{
			dfs(m, nx, ny, visited);
		}
	}
}

static size_t part2(const vector<string>& m)
{
	vector<vector<bool>> visited(m.size());
	for (auto &v: visited)
	{
		v.resize(m.size());
	}

	size_t groups = 0;
	for (int y = 0; y < 128; y++)
	{
		for (int x = 0; x < 128; x++)
		{
			if (m[y][x] == '#' && !visited[y][x])
			{
				dfs(m, x, y, visited);
				groups++;
			}
		}
	}
	return groups;
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
	string salt;
	getline(input, salt);
	input.close();
	if (input.fail())
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}

	auto m = make_map(salt);
	cout << fmt::format("Part1: {}", part1(m)) << endl
	     << fmt::format("Part2: {}", part2(m)) << endl;
	return 0;
}
