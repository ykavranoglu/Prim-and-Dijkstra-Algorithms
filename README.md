# Prim and Dijkstra Algorithms
Two C++ programs, one finds the minimum spanning tree of a graph using Prim's Algorithm, and the other finds the shortest path on a graph using Dijkstra's Algorithm.

These two programs are inside the same repository because they share some codes and classes.

## Prim's Algorithm:

"In computer science, Prim's (also known as Jarník's) algorithm is a greedy algorithm that finds a minimum spanning tree for a weighted undirected graph. This means it finds a subset of the edges that forms a tree that includes every vertex, where the total weight of all the edges in the tree is minimized. The algorithm operates by building this tree one vertex at a time, from an arbitrary starting vertex, at each step adding the cheapest possible connection from the tree to another vertex."
Wikipedia contributors. (2021, April 27). Prim's algorithm. In Wikipedia, The Free Encyclopedia. Retrieved 17:02, May 22, 2021, from https://en.wikipedia.org/wiki/Prim%27s_algorithm

Here, given example creates a graph of a city. Nodes of this graph are the special buildings that exists in that city.

The program takes a txt file as input. This input file contains the information of all the buildings, which building is connected to which, and the weights of those connections. Once this graph is created, Prim's Algorithm implementation (Graph::FindPrimMST) finds the minimum spanning tree of this graph. Starting vertex is always "GP" meaning Great Palace. All the vertices that exists in thee minimum spanning tree are printed. Also the cost of the entire minimum spanning tree is printed at the end.

Input txt uses these abbreviations:
GP - Great Palace
Hipp - Hippodrome
Bas1 - Basilica 1
Ch1 - Church 1
Hp1 - Home of important person 1

These conditions are satisfied:
1- The construction cost must be minimal
2- The Great Palace must be directly connected to at least one church
3- The first construction must be between a church and the Great Palace
4- The Hippodrome must have a direct connection to the Great Palace
5- The Hippodrome should not be directly connected to any basilicas
6- There shouldn't be any direct connections between the houses of two important people

The program takes the file name of the input txt after the execution with std::cin function.


## Dijkstra's Algorithm:

Dijkstra's algorithm is an algorithm for finding the shortest paths between nodes in a graph, which may represent, for example, road networks. It was conceived by computer scientist Edsger W. Dijkstra in 1956 and published three years later.
Wikipedia contributors. (2021, May 6). Dijkstra's algorithm. In Wikipedia, The Free Encyclopedia. Retrieved 17:21, May 22, 2021, from https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

Here, given example creates a graph, which has one starting position (a city) and one destination (also a city). And there are also safe spots, enemy spots and resting places.

Like the Prim's Algorithm programi the input file contains the information of all the places and their weighted connections. Once the graph is created, the shortest path which satisfies some conditions is found using the Dijkstra's Algorithm. All the places that exists in the shortest path are printed along with the total weight of the shortest path.

Input txt uses these abbreviations:
S1 - Safe Spot 1
E1 - Enemy Spot 1
R1 - Resting Place 1
Ma - Mankara
Mo - Monstantinople

These conditions are satisfied:
1- The path must be as short as possible
2- The path must contain safe spots and/or resting spots
3- The path must not cross a spot which is occupied by enemies.
4- The path must not cross a spot which is directly connected to an enemy occupied place with a distance smaller than 5 units of distance.

The program takes the file name of the input txt after the execution with std::cin function.


Examples are taken from ITU BLG336E Course, Spring 2021 Homeworks
