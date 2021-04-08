#include "SA.h"
#include "BP.h"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
using namespace std;

/**
Class for the Simulated Annealing of the TSP
After calling the TSP class, we have a tour for each truck
But this solution is not the best because we used algortihm that do not return
the best solution at each step.

The Simulated Annealing will start from a solution given by the TSP solving.
Then it will go from a solution to another by adding a truck, remove a truck,
change two clients positions in a tour, or change allocate a client to another tour

At the beginning, the SA will save the current solution determined even if
it is a worse solution than the last one. So that the SA can explore the solution
domain. Then it will start converge to a solution by keeping only the best solution.
**/
SA::SA()
{

}

/**
The main part of the algorithm
**/
std::vector<std::vector<int>> SA::simulatedAnnealing(Graph graph, EvaluateTour e, std::vector<std::vector<int>> nodes, int kmax, float t, float alpha, float energy_max)
{
	std::vector<std::vector<int>> s = nodes;
	std::vector<std::vector<int>> g = nodes;

	float energy_s = getEnergy(graph, e, s);
	float energy_g = energy_s;

	std::vector<std::vector<int>> sn;
	float energy_sn;

	/* this float represents the exploration, at the beginning, high temperature means exploration
	as the temperature decrease, less exploration */
	float temperature = t;

	int m = graph.vehicles + 2;

	srand(time(NULL));

	int k = 0;
	while(k < kmax && energy_s > energy_max)
	{
		/// Get the neighbor solution of the current one
		sn = getNeighbor(graph, s, m);
		/// evaluate it
		energy_sn = getEnergy(graph, e, sn);

		/// if the neighbor is better, we keep it
		/// or if the temperature allows us to keep it, we keep it (randomly)
		if(energy_sn < energy_s || ((double)rand() / (RAND_MAX)) < getProbability(energy_sn-energy_s, temperature))
		{
			/// change the current solution
			s = sn;
			energy_s = energy_sn;
		}

		if(energy_s < energy_g)
		{
			g = s;
			energy_g = energy_s;
		}
		k += 1;
		/// decrease the temperature
		temperature *= alpha;
	}

	g = eraseEmptyTruck(graph, g);

	return g;
}

/*
Function that returns the evaluation of a solution, the sum of the distances travelled
by each truck
*/
double SA::getEnergy(Graph graph, EvaluateTour e, std::vector<std::vector<int>> sn)
{
	return e.evaluate(graph, sn);
	//return e.evaluate(graph, sn) + 100 * std::max((int)(sn.size() - graph.vehicles), 0);
}

float SA::getProbability(float e, float t)
{
	return exp(-e / t);
}

/*
Returns a solution from another one by adding a truck, remove a truck, change a client from
a tour to another, or swap two clients in the same tour
*/
std::vector<std::vector<int>> SA::getNeighbor(Graph graph, std::vector<std::vector<int>> s, int m)
{
	double r = ((double) rand() / (RAND_MAX));

	if(r < 1/3.0)
	{
		return clientChange(graph, s);
	}

	if (r < 2/3.0)
	{
		return swapClient(graph, s);
	}

	if (r < 5/6.0)
	{
		return addTruck(graph, s, m);
	}

	if (r < 1.0)
	{
		return eraseEmptyTruck(graph, s);
	}

	return s;
}

std::vector<std::vector<int>> SA::swapClient(Graph graph, std::vector<std::vector<int>> s)
{
	int i;
	int j1;
	int j2;
	int tmp;

	do
	{
		i = rand() % s.size();
	} while (s[i].size() < 4);

	if (s[i].size() == 4)
	{
		tmp = s[i][1];
		s[i][1] = s[i][2];
		s[i][2] = tmp;

		return s;
	}

	j1 = rand() % (s[i].size() - 2) + 1;;
	do
	{
		j2 = rand() % (s[i].size() - 2) + 1;;
	} while (j2 == j1);


	tmp = s[i][j1];
	s[i][j1] = s[i][j2];
	s[i][j2] = tmp;

	return s;
}

std::vector<std::vector<int>> SA::addTruck(Graph graph, std::vector<std::vector<int>> s, int m)
{
	if (s.size() == m)
	{
		return s;
	}
	std::vector<int> v;
	v.push_back(1);
	v.push_back(1);
	s.insert(s.begin(), v);

	return s;
}

std::vector<std::vector<int>> SA::eraseEmptyTruck(Graph graph, std::vector<std::vector<int>> s)
{
	for (int k = 0; k < s.size(); ++k)
	{
		if (s[k].size() == 2)
		{
			s.erase(s.begin()+k); // this line could ruin the loop
			return s;
		}
	}

	return s;
}

std::vector<std::vector<int>> SA::clientChange(Graph graph, std::vector<std::vector<int>> s)
{
	std::vector<std::vector<int>> sn = s;
	bool neighborNotFound = true;
	int i1;
	int i2;
	int j1;
	int j2;
	std::vector<int> s1;
	std::vector<int> s2;

	while (neighborNotFound)
	{
		do
		{
			i1 = (int)rand() % s.size();
		} while (s[i1].size() < 3);
		do
		{
			i2 = (int) rand() % s.size();
		} while (i2 == i1);
		s1 = s[i1];
		s2 = s[i2];

		j1 = (rand() % (s1.size() - 2)) + 1;

		if (s2.size() == 2)
		{
			j2 = 1;
		}
		else
		{
			j2 = (rand() % (s2.size() - 2)) + 1;
		}

		s2.insert(s2.begin() + j2, s1[j1]);
		s1.erase(s1.begin() + j1);

		if (isValidTruck(graph, s2))
		{
			neighborNotFound = false;
		}
	}

	sn[i1] = s1;
	sn[i2] = s2;

	return sn;
}

bool SA::isValidTruck(Graph graph, std::vector<int> s)
{
	int sumD = 0;

	for (int k = 0; k < s.size(); ++k)
	{
		sumD += graph.demand[s[k]];

		if (sumD > graph.capacity)
		{
			return false;
		}
	}

	return true;
}
