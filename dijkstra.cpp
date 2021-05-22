/*
Commented definitons of each function exists on top of them, below main() function
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

//Nodes in the graph, or vertices, children means connections from it, they are not necessarily its "children"
struct Node {
	std::string name;
	std::vector<Node*> children;	//for every children, a weight exists
	std::vector<unsigned> weights;		//for every children, a weight exists
	unsigned distance = 10000;	//initial value of each node's distance is infinite/very large
	bool done = false;
};

//Stores information of an edge in the graph
struct Edge {
	std::string first;
	std::string second;
	unsigned weight = 10000;	//will be erronous to use this initial value
};

class Graph {
public:
	Graph();	//not needed
	~Graph();	//clears member vectors
	void create(const std::vector<std::string>, std::vector<Edge>);
	void FindShortest(std::string, std::string);
	bool CloseToEnemy(unsigned);
	void clear();
private:
	std::vector<Node*> places;	//this can be used in place of head, as graphs usually don't have a head as we know it.
};

unsigned find_index(std::vector<Node*>, std::string);

int main()
{
	std::string fname;
	std::cin >> fname;
	std::ifstream path_info(fname);

	if (!path_info) {
		std::cerr << "File cannot be opened!";
		exit(1);
	}

	Graph g;	//Attention: Because this code was given in the homework description, I used this. But instead of this, I could discard the Graph::create function and create the graph with a constructor.
	std::string startVertex = "Ma";
	std::string endVertex = "Mo";

	std::vector<std::string> unique;	//All unique places will be stored in this

	std::vector<Edge> path_info_vec;	//All valid edges for the tunnel will be read from this later on

	std::string place[2];
	std::string weight;

	//txt file is read and information is extracted
	do {

		getline(path_info, place[0], ',');
		getline(path_info, place[1], ',');
		getline(path_info, weight);

		Edge temp_edge;
		temp_edge.first = place[0];
		temp_edge.second = place[1];
		temp_edge.weight = stoi(weight);

		path_info_vec.push_back(temp_edge);

		unsigned i = 0;
		bool flag[2] = { true, true };
		while ((i < unique.size()) && (flag[0] || flag[1])) {
			if (unique[i] == place[0]) {
				flag[0] = false;
			}
			if (unique[i] == place[1]) {
				flag[1] = false;
			}
			i++;
		}
		if (flag[0]) {
			unique.push_back(place[0]);
		}
		if (flag[1]) {
			unique.push_back(place[1]);
		}

	} 	while (!path_info.eof());

	g.create(unique, path_info_vec);	//creates the graph

	g.FindShortest(startVertex, endVertex);	//finds the shortest path between startVertex and endVertex, with respect to given restrictions

	//memory clean-up
	path_info_vec.clear();
	unique.clear();
}

//no need
Graph::Graph()
{
}

//clears member vectors to prevent memory leak
Graph::~Graph()
{
	this->clear();

	places.clear();
}

//Creates the graph, and also creates a list of nodes so that they can be reached with indexing (this is useful when creating the graph)
void Graph::create(const std::vector<std::string> unique, std::vector<Edge> path_info_vec)
{
	//A node is created for each unique place
	for (unsigned i = 0; i < unique.size(); i++) {
		Node* new_node = new Node();
		new_node->name = unique[i];
		places.push_back(new_node);
	}

	//Connections between vertices are created
	for (unsigned i = 0; i < path_info_vec.size(); i++) {
		unsigned index_1 = find_index(places, path_info_vec[i].first);
		unsigned index_2 = find_index(places, path_info_vec[i].second);

		//both places in the text line get each other as children
		places[index_1]->children.push_back(places[index_2]);
		places[index_1]->weights.push_back(path_info_vec[i].weight);
		places[index_2]->children.push_back(places[index_1]);
		places[index_2]->weights.push_back(path_info_vec[i].weight);
	}
}

//Finds the shortest path between the nodes named start_vertex and end_vertex, considering the given restrictions
void Graph::FindShortest(std::string start_vertex, std::string end_vertex)
{	
	std::vector<Node*> out_nodes;	//will hold the necessary information to print output
	std::vector<Node*> temp_nodes;	//holds a solution, not necessarily the best one

	Node* temp;

	temp = places[find_index(places, start_vertex)];	//starts creating the MST from here		/		this variable is used so that find_index() isn't called twice as much
	temp->distance = 0;
	temp_nodes.push_back(temp);

	//if start_vertex doesn't exist in the given file, terminate
	if (!temp) {
		exit(1);
	}

	//Traverses the graph according to Djikstra's algorithm
	unsigned local_distance = 9999;	//9999 basically means infinite, holds the length of the shortest-yet solution
	bool not_finished = true;	//not_finished becomes false when all vertices in the graph are "Node::done == true"
	while (not_finished) {
		not_finished = false;

		//updates distance of every "not-done" neighbour of the current vertex.
		for (unsigned i = 0; i < temp->children.size(); i++) {
			if ((temp->distance + temp->weights[i]) < temp->children[i]->distance) {
				temp->children[i]->distance = temp->distance + temp->weights[i];
			}

			//if destination is found	/	this means a solution is found but not necessarily the best one
			if (temp->children[i]->name == end_vertex) {
				if (temp->children[i]->distance < local_distance) {
					local_distance = temp->children[i]->distance;

					//discards the recent solution and stores the new shorter solution
					out_nodes.clear();
					for (unsigned j = 0; j < temp_nodes.size(); j++) {
						out_nodes.push_back(temp_nodes[j]);
					}
					out_nodes.push_back(temp->children[i]);
				}
			}
		}

		temp->done = true;

		//finding the the next vertex to iterate over, basically the shortest not-done neighbour of the current vertex
		unsigned local_closest = 9999;
		Node* temp_temp = NULL;
		for (unsigned i = 0; i < temp->children.size(); i++) {
			
			//if i'th child is not done and has lower distance
			if (!CloseToEnemy(find_index(places, temp->children[i]->name)) && !(temp->children[i]->done) && (temp->weights[i] < local_closest)) {
				local_closest = temp->weights[i];
				temp_temp = temp->children[i];
				not_finished = true;
			}
		}
		
		//if not finished, push the new  vertex to temp_nodes
		if (not_finished) {
			temp = temp_temp;
			temp_nodes.push_back(temp);
		}
	}

	//Prints the solution with the wanted format
	std::cout << out_nodes[0]->name;
	for (unsigned i = 1; i < out_nodes.size(); i++) {
		std::cout << " " << out_nodes[i]->name;
	}
	std::cout << " " << local_distance << "\n";
	
	//memory clean-up
	out_nodes.clear();
	temp_nodes.clear();
}

//Checks if this->places[index] is enemy or close to enemy
bool Graph::CloseToEnemy(unsigned index)
{
	//checks if node is enemy
	if (this->places[index]->name.substr(0, 1) == "E") {
		return true;
	}
	
	//checks if node is close to enemy
	for (unsigned i = 0; i < this->places[index]->children.size(); i++) {
		if (this->places[index]->children[i]->name.substr(0, 1) == "E") {
			if (this->places[index]->weights[i] < 5) {
				return true;
			}
		}
	}
	
	return false;
}

//because we are holding all nodes in a vector, clearing the graph is very easy		/	used inside the destructor
void Graph::clear()
{
	for (unsigned i = 0; i < places.size(); i++) {
		delete places[i];
	}
}

//this function makes it easy to effectively use indexes on std::vector<Node*>'s	/	finds the index of the Node whose name variable is the given string
unsigned find_index(std::vector<Node*> all_places, std::string place)
{
	for (unsigned i = 0; i < all_places.size(); i++) {
		if (all_places[i]->name == place) {
			return i;
		}
	}

	exit(1);	//erronous outcome
}
