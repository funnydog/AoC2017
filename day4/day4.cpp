#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

using namespace std;

static bool is_valid(const string& str, bool anagram)
{
	unordered_set<string> m;
	size_t pos = 0;
	size_t next;
	do
	{
		next = str.find(' ', pos);
		// TODO: processing
		string word = str.substr(pos, next-pos);
		if (anagram)
		{
			sort(word.begin(), word.end());
		}
		if (m.find(word) != m.end())
		{
			return false;
		}
		m.insert(move(word));
		pos = next + 1;
	} while (next != string::npos);

	return true;
}

static size_t count_valid(const vector<string>& phrases, bool anagram)
{
	return count_if(
		phrases.begin(),
		phrases.end(),
		[anagram](const auto& str) {
			return is_valid(str, anagram);
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

	vector<string> phrases;
	string phrase;
	while (getline(input, phrase))
	{
		phrases.emplace_back(move(phrase));
	}
	input.close();

	cout << fmt::format("Part1: {}", count_valid(phrases, false)) << endl
	     << fmt::format("Part2: {}", count_valid(phrases, true)) << endl;
	return 0;
}
