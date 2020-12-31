#include <cctype>
#include <fstream>
#include <iostream>

#include <fmt/format.h>

using namespace std;

static size_t captcha(const string& txt, size_t offset)
{
	size_t size = txt.size();
	size_t captcha = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (txt[i] == txt[(i+offset)%size] && isdigit(txt[i]))
		{
			captcha += txt[i]-'0';
		}
	}
	return captcha;
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
	string txt;
	getline(input, txt);
	input.close();
	if (input.fail())
	{
		cerr << "Cannot parse the data" << endl;
		return 1;
	}

	cout << fmt::format("Part1: {}", captcha(txt, 1)) << endl
	     << fmt::format("Part2: {}", captcha(txt, txt.size()/2)) << endl;

	return 0;
}
