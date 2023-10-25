#include <vector>
#include <iostream>

// build your own lib from https://github.com/arminbiere/kissat
// note that the SAT solver is written in pure C
// to use with C++ (and g++), wrap the header in extern "C"{...}
#include "kissat.h"

using namespace std;

void CNF_to_kissat(vector<vector<int> >&, int);

int main(int argc, char** argv) 
{
	int colors_nr = 3;
	int vertices = 4;

	vector<pair<int,int> > edges = {{0,1},{0,2},{0,3},{1,3},{2,3}};

	vector<vector<int> > CNF;

	// each vertex has at least 1 color
	for (int v = 0; v < vertices; v++)
	{
		vector<int> clause;
		for (int c = 0; c < colors_nr; c++)
		{
			clause.push_back(v*colors_nr + c + 1);
		}
		CNF.push_back(clause);
	}

	// each vertex has at most 1 color
	for (int v = 0; v < vertices; v++)
	{
		for (int c1 = 0; c1 < colors_nr; c1++)
		{
			for (int c2 = c1+1; c2 < colors_nr; c2++)
			{
				vector<int> clause;
				clause.push_back(-(v*colors_nr + c1 + 1));
				clause.push_back(-(v*colors_nr + c2 + 1));
				CNF.push_back(clause);
			}
		}
	}

	// edges are not monochrome
	for (size_t e = 0; e < edges.size(); e++)
	{
		int v = edges[e].first;
		int u = edges[e].second;
		for (int c = 0; c < colors_nr; c++)
		{
			vector<int> clause;
			clause.push_back(-(v*colors_nr + c + 1));
			clause.push_back(-(u*colors_nr + c + 1));
			CNF.push_back(clause);
		}
	}

	CNF_to_kissat(CNF, vertices * colors_nr);

	return 0;
}

void CNF_to_kissat(vector<vector<int> >& CNF, int vars)
{
	kissat* solver = kissat_init();
    kissat_set_option(solver, "quiet", 1);
	
	// CNF to solver
	for (size_t i = 0; i < CNF.size(); i++)
	{
		for (size_t j = 0; j < CNF[i].size(); j++)
			kissat_add(solver, CNF[i][j]);
		kissat_add(solver, 0);
	}

    int res = kissat_solve(solver);

	if (res != 10)
	{
		cout << "no solution" << endl;
		return;
	}

	for (int var = 0; var < vars; var++)
	{
		cout << "variable " << var + 1 << " has value " << kissat_value (solver, var + 1) << endl;
	}
}