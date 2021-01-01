#include <climits>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <fmt/format.h>

using namespace std;

struct Program
{
	string name;
	int weight;
	int total_weight;
	vector<size_t> adj;
	bool visited;

	explicit Program(string&& name)
		: name(move(name)),
		  weight(0),
		  total_weight(0),
		  visited(false)
	{
	}
};

static vector<Program> load(istream& in)
{
	unordered_map<string, size_t> indices;
	vector<Program> progs;
	string line;
	while (getline(in,line))
	{
		enum {NAME, DEP} state = NAME;
		size_t cur = 0;
		size_t pos = 0;
		size_t next;
		do
		{
			next = line.find(' ', pos);
			string word = line.substr(pos, next-pos);
			if (state == NAME)
			{
				// add a new node (or weight)
				if (word[0] == '(')
				{
					progs[cur].weight = stoi(word.substr(1));
				}
				else if (word == "->")
				{
					state = DEP;
				}
				else
				{
					auto it = indices.find(word);
					if (it != indices.end())
					{
						cur = it->second;
					}
					else
					{
						cur = indices[word] = progs.size();
						progs.emplace_back(Program(move(word)));
					}
				}
			}
			else
			{
				// add a dependency
				if (word.back() == ',')
				{
					word.pop_back();
				}
				auto it = indices.find(word);
				if (it != indices.end())
				{
					progs[cur].adj.push_back(it->second);
				}
				else
				{
					indices[word] = progs.size();
					progs[cur].adj.push_back(progs.size());
					progs.emplace_back(Program(move(word)));
				}
			}
			pos = next + 1;
		} while (next != string::npos);
	}
	return progs;
}

static void dfs(vector<Program>& progs, size_t i, vector<size_t>& out)
{
	progs[i].visited = true;
	for (auto j: progs[i].adj)
	{
		if (!progs[j].visited)
		{
			dfs(progs, j, out);
		}
	}
	out.push_back(i);
}

static vector<size_t> topsort(vector<Program>& progs)
{
	vector<size_t> sort;
	for (size_t i = 0; i < progs.size(); i++)
	{
		if (!progs[i].visited)
		{
			dfs(progs, i, sort);
		}
	}
	return sort;
}

static int weight(vector<Program>& progs, const vector<size_t>& sort)
{
	for (auto i: sort)
	{
		progs[i].total_weight = progs[i].weight;
		vector<int> s;
		int min = INT_MAX;
		int max = INT_MIN;
		for (auto j: progs[i].adj)
		{
			int w = progs[j].total_weight;
			progs[i].total_weight += w;
			s.push_back(w);
			if (min > w) min = w;
			if (max < w) max = w;
		}
		if (max <= min)
		{
			continue;
		}

		if (count(s.begin(), s.end(), min) == 1)
		{
			size_t j = find(s.begin(), s.end(), min) - s.begin();
			return progs[progs[i].adj[j]].weight + (max - min);
		}
		else
		{
			size_t j = find(s.begin(), s.end(), max) - s.begin();
			return progs[progs[i].adj[j]].weight - (max - min);
		}
	}
	return 0;
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
	auto progs = load(input);
	input.close();

	auto sort = topsort(progs);
	cout << fmt::format("Part1: {}", progs[sort.back()].name) << endl
	     << fmt::format("Part2: {}", weight(progs, sort)) << endl;
	return 0;
}
