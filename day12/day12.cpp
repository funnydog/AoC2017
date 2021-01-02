#include <fstream>
#include <iostream>
#include <vector>

#include <fmt/format.h>

using namespace std;

static vector<vector<int>> load_graph(istream& input)
{
	vector<vector<int>> graph;
	string line;
	while (getline(input, line))
	{
		enum {NODE, DEPS} state = NODE;
		size_t pos = 0;
		size_t next;
		vector<int> deps;
		do
		{
			next = line.find(" ", pos);
			if (state == NODE)
			{
				if (line.rfind("<->", pos) == pos)
				{
					state = DEPS;
				}
			}
			else if (state == DEPS)
			{
				deps.push_back(stoi(line.substr(pos, next-pos)));
			}
			pos = next + 1;
		} while (next != string::npos);
		graph.emplace_back(move(deps));
	}
	return graph;
}

static void dfs(const vector<vector<int>>& g, int i, vector<bool>& visited)
{
	visited[i] = true;
	for (auto j: g[i])
	{
		if (!visited[j])
		{
			dfs(g, j, visited);
		}
	}
}

static size_t part1(const vector<vector<int>>& g)
{
	vector<bool> visited(g.size(), false);
	dfs(g, 0, visited);
	return count(visited.begin(), visited.end(), true);
}

static size_t part2(const vector<vector<int>>& g)
{
	vector<bool> visited(g.size(), false);
	size_t groups = 0;
	for (size_t i = 0; i < g.size(); i++)
	{
		if (!visited[i])
		{
			dfs(g, i, visited);
			groups++;
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
	auto graph = load_graph(input);
	input.close();

	cout << fmt::format("Part1: {}", part1(graph)) << endl
	     << fmt::format("Part2: {}", part2(graph)) << endl;
	return 0;
}
