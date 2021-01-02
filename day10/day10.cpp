#include <fstream>
#include <iostream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static const size_t SIZE = 256U;

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

static size_t part1(const string& line)
{
	vector<size_t> lengths;
	size_t pos = 0;
	size_t next;
	do
	{
		next = line.find(',', pos);
		lengths.push_back(stoull(line.substr(pos, next-pos)));
		pos = next + 1;
	} while (next != string::npos);

	unsigned char buf[SIZE];
	hashfn(lengths, 1, buf);
	return (unsigned)buf[0] * buf[1];
}

static string part2(const string& line)
{
	vector<size_t> lengths(line.begin(), line.end());
	const size_t seq[] = {17, 31, 73, 47, 23};
	lengths.insert(lengths.end(), seq, seq+5);

	unsigned char buf[SIZE];
	hashfn(lengths, 64, buf);

	string digest;
	const char *hex = "0123456789abcdef";
	for (size_t i = 0; i < 256; i += 16)
	{
		unsigned char v = buf[i];
		for (size_t j = i+1; j < i+16; j++)
		{
			v ^= buf[j];
		}
		digest.push_back(hex[v>>4]);
		digest.push_back(hex[v&0x0f]);
	}
	return digest;
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
	string line;
	getline(input, line);
	input.close();
	if (input.fail())
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}

	cout << fmt::format("Part1: {}", part1(line)) << endl
	     << fmt::format("Part2: {}", part2(line)) << endl;
	return 0;
}
