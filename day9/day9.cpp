#include <fstream>
#include <iostream>

#include <fmt/format.h>

using namespace std;

static int cur;
static size_t discarded;

static void scan_next(istream& input)
{
	enum {OUT, GARBAGE, IGNORE} state = OUT;
	while ((cur = input.get()) != EOF)
	{
		if (state == OUT)
		{
			if (cur == '<')
			{
				state = GARBAGE;
			}
			else if (cur == '{' || cur == '}')
			{
				break;
			}
		}
		else if (state == GARBAGE)
		{
			if (cur == '!')
			{
				state = IGNORE;
			}
			else if (cur == '>')
			{
				state = OUT;
			}
			else
			{
				// count the discarded
				discarded++;
			}
		}
		else if (state == IGNORE)
		{
			state = GARBAGE;
		}
		else
		{
			cerr << "Unknown state" << endl;
			abort();
		}
	}
}

static void match(istream& in, int val)
{
	if (val != cur)
	{
		cerr << fmt::format("Expected {}, found {} instead.", val, cur) << endl;
		abort();
	}
	scan_next(in);
}

static int group(istream& in, int points)
{
	int r = 0;
	while (cur == '{')
	{
		r += points;
		scan_next(in);
		r += group(in, points+1);
		match(in, '}');
	}
	return r;
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

	scan_next(input);
	int points = group(input, 1);
	match(input, EOF);
	input.close();

	cout << fmt::format("Part1: {}", points) << endl
	     << fmt::format("Part2: {}", discarded) << endl;
	return 0;
}
