CVRP

##### Introduction

  CVRP is a project made with a classmate during my master degree.
This project consist of solving a complex problem that combines two problems.
The Back packing Problem and the Travel Salesman Problem.

For this, we have a warehouse and clients to deliver thanks to trucks.
Our goal is to minimize the distance travelled by the trucks.
Each client has a demand that represents the weight of the package to be delivered.
As trucks are limited in demand capacity (they can not be overcharged),
we have to determine a way a allocate each client to a truck, and then the tour of the truck (the order of each clients to deliver).

This project was in two parts, the first one is to get a satisfying solution quickly, even if it is not the best by using approximation algorithms.
And the last part is to get the best solution by using a solver (here it is CPlex).

The instance of the problem can be found in data/A and the best solution for each one in data/P.
