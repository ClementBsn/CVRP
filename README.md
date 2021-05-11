CVRP

##### Introduction

  CVRP is a project made with a classmate during my master degree.
This project consist of solving a complex problem that combines two famous problems.
The Knapsack Problem and the Travelling Salesman Problem.

We have a warehouse and clients. Each client has to be delivered by trucks.
These trucks have to go and come back to the warehouse.
Our goal is to minimize the distance travelled by the trucks. (we use euclidian distance)

Each client has a demand that represents the weight of the package to be delivered.
As trucks are limited in demand capacity (they can not be overcharged),
we have to determine a way a allocate each client to a truck, and then the tour
of the truck (the order of each clients to deliver).

This project was in two parts, the first one is to get a satisfying solution quickly,
even if it is not the best by using approximation algorithms.
And the last part is to get the best solution by using a solver (here it is CPlex).

The instance of the problem can be found in data/A and the best solution for each one in data/P.

##### BP

  To solve the Knapsack Problem (also called Backpack Problem), I implemented
an algorithm in BP.cpp.

First step, we organise the clients by decreasing order of their demand.
Then, for each client, we allocate it to a truck. If the truck cannot take its
demand, we create a new truck and allocate this client to this truck.

At the end of the algorithm, we have each client allocated to a truck.
But we do not know the order to deliver the clients.

##### TSP

In this part, we have a list of clients to deliver for each truck.
And we want to know, for each truck, its tour.

For this, I implemented two algortihm to solve the Travelling Salesman Problem.

The first way is Nearest Neighbor. For this, each truck start from the warehouse.
Then we add as the first client the nearest one. Then we add as the second client the nearest
one from the first one, etc. And at the end we add the warehouse (come back).

The second way is Nearest Insertion. Each truck start with a tour composed with
the warehouse at the beginning and at the end. Then, for each client, we add it
between two destinations where it fits best (add the less distance to the global distance
of the tour).

Thanks to this algorithms, we can have the tours of each trucks. And we can deliver
every clients.

##### SA

With the solutions given by the algorithms from above, we have a satisfying solution
but it is far from the perfect one.

So I implemented an algorithm to improve the solutions returned by the algorithms
described above. The Simulated Annealing algorithm can improve our solution.

This algorithm take our solution, and then will try to search for a neighbor solution
that can be better. For this, the algorithm can either take a client from a truck and
give it to anther truck (if it is possible), swap the position of two clients in a
truck's tour, add a truck or erase a truck with no clients.
If the neighbor solution is better, we keep this solution.

This algorithm has a temperature t parameter that allows the algorithm to explore
the solution domain by accepting a new solution as the best (even if it is not better).
As the algorithm runs, the temperature will decrease and it will start digging for
the best solution.

At the end of a run, we have a solution that is a lot closer to the best solution.

##### Conclusion

This way of solving this problem allow us to have a good solution quickly.
This way, if the clients to deliver change every day (like amazon delivery),
it is better to solve this problem with the algorithm described above.

If the clients never change, it is better to solve with the CPLEX solution, it
returns the best solution but with an execution time much longer. But as the clients
do not change, there will be no use to solve this problem again.
