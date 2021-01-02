#include <fstream>
#include <iostream>

#include <fmt/format.h>

using namespace std;

struct Move
{
	enum {SPIN, EXCHANGE, PARTNER} type;
	int arg1;
	int arg2;

	void dance(string& progs) const
	{
		switch (type)
		{
		case SPIN: {
			for (int i = 0; i < arg1; i++)
			{
				int c = progs.back();
				progs.pop_back();
				progs.insert(progs.begin(), c);
			}
			break;
		}

		case EXCHANGE:{
			swap(progs[arg1], progs[arg2]);
			break;
		}

		case PARTNER: {
			size_t a = progs.find(arg1);
			size_t b = progs.find(arg2);
			swap(progs[a], progs[b]);
			break;
		}
		}
	}
};

istream& operator>>(istream& in, Move& m)
{
	switch(in.get())
	{
	case 's':
		m.type = Move::SPIN;
		in >> m.arg1;
		break;
	case 'x':
		m.type = Move::EXCHANGE;
		in >> m.arg1;
		in.get();
		in >> m.arg2;
		break;
	case 'p':
		m.type = Move::PARTNER;
		m.arg1 = in.get();
		in.get();
		m.arg2 = in.get();
		break;
	default:
		in.setstate(ios::failbit);
		break;
	}
	return in;
}

static void step(const vector<Move>& moves, string& progs)
{
	for (auto& m: moves)
	{
		m.dance(progs);
	}
}

static string part1(const vector<Move>& moves)
{
	string initial;
	for (int i = 0; i < 16; i++)
	{
		initial.push_back('a' + i);
	}
	step(moves, initial);
	return initial;
}

static string part2(const vector<Move>& moves)
{
	string initial;
	for (int i = 0; i < 16; i++)
	{
		initial.push_back('a' + i);
	}

	// floyd algorithm
	string tortoise(initial);
	string hare(initial);
	do
	{
		step(moves, tortoise);
		step(moves, hare);
		step(moves, hare);
	} while (tortoise != hare);

	// find mu
	size_t mu = 0;
	tortoise = initial;
	while (tortoise != hare)
	{
		step(moves, tortoise);
		step(moves, hare);
		mu++;
	}

	// find lambda
	size_t lam = 0;
	hare = tortoise;
	do
	{
		step(moves, hare);
		lam++;
	} while (tortoise != hare);

	size_t cycles = (1000000000ULL - mu) % lam + mu;
	while (cycles-->0)
	{
		step(moves, initial);
	}
	return initial;
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
	vector<Move> moves;
	Move m;
	while (input>>m)
	{
		input.get();
		moves.emplace_back(move(m));
	}
	input.close();

	cout << fmt::format("Part1: {}", part1(moves)) << endl
	     << fmt::format("Part2: {}", part2(moves)) << endl;
	return 0;
}
