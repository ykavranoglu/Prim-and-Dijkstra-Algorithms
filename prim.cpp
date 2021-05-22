/*
Commented definitons of each function exists on top of them, below main() function
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>	// std::sort

//Nodes in the graph, or vertices, children means connections from it, they are not necessarily its "children"
struct Node {
	std::string name;
	std::vector<Node*> children;	//for every children, a weight exists
	std::vector<unsigned> weights;		//for every children, a weight exists
	bool in_mst = false;
};

//Stores information of an edge in the graph
struct Edge {
	std::string first;
	std::string second;
	unsigned weight = 10000;	//will be erronous to use this initial value, won't be used normally
};

class Graph {
public:
	Graph();	//not needed
	~Graph();	//clears member vectors
	void create(const std::vector<std::string>, std::vector<Edge>);	//creates the graph
	void FindPrimMST(std::string);
	void clear();
private:
	std::vector<Node*> places;	//this can be used in place of head, as graphs usually don't have a head as we know it.
	std::vector<Edge> city_plan;	//will be used by FindPrimMST() to check the format of the output
};

unsigned find_index(std::vector<Node*>, std::string);
bool SortingFunc(Edge, Edge);

int main()
{
    std::string fname;
    std::cin >> fname;
    std::ifstream city_plan(fname);

	if (!city_plan) {
		std::cerr << "File cannot be opened!";
		exit(1);
	}

	Graph g;	//Attention: Because this code was given in the homework description, I used this. But instead of this, I could discard the Graph::create function and create the graph with a constructor.
	std::string startVertex = "GP";

	std::vector<std::string> unique;	//All unique places will be stored in this

	std::vector<Edge> city_plan_vec;	//All valid edges for the tunnel will be read from this later on

	std::string place[2];
	std::string weight;

	//txt file is read and information is extracted
	do {
		
		getline(city_plan, place[0], ',');
		getline(city_plan, place[1], ',');
		getline(city_plan, weight);

		bool valid_edge = true;
		if (((place[0].substr(0, 4) == "Hipp") && (place[1].substr(0, 3) == "Bas")) || ((place[1].substr(0, 4) == "Hipp") && (place[0].substr(0, 3) == "Bas"))) {	//Hipp can't connect to Bas
			valid_edge = false;
		}
		if ((place[0].substr(0, 2) == "Hp") && (place[1].substr(0, 2) == "Hp")) {	//Hp can't connect to Hp
			valid_edge = false;
		}
		if (valid_edge) {
			Edge temp_edge;
			temp_edge.first = place[0];
			temp_edge.second = place[1];
			temp_edge.weight = stoi(weight);

			city_plan_vec.push_back(temp_edge);
		}

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

	}
	while (!city_plan.eof());

	g.create(unique, city_plan_vec); //creates the graph
	
	g.FindPrimMST(startVertex);	//finds the MST inside the graph

	//memory clean-up
	city_plan_vec.clear();
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
	city_plan.clear();
}

//Creates the graph, and also creates a list of nodes so that they can be reached with indexing (this is useful when creating the graph)
void Graph::create(const std::vector<std::string> unique, std::vector<Edge> city_plan_vec)
{
	city_plan = city_plan_vec;	//will be used later on, this vector is saved to graph

	//A node is created for each unique place
	for (unsigned i = 0; i < unique.size(); i++) {
		Node* new_node = new Node();
		new_node->name = unique[i];
		places.push_back(new_node);
	}

	//Connections between vertices are created
	for (unsigned i = 0; i < city_plan_vec.size(); i++) {
		unsigned index_1 = find_index(places, city_plan_vec[i].first);
		unsigned index_2 = find_index(places, city_plan_vec[i].second);

		//both places in the text line get each other as children
		places[index_1]->children.push_back(places[index_2]);
		places[index_1]->weights.push_back(city_plan_vec[i].weight);
		places[index_2]->children.push_back(places[index_1]);
		places[index_2]->weights.push_back(city_plan_vec[i].weight);
	}


}

//Finds the MST with Prim algorithm
void Graph::FindPrimMST(std::string start_vertex)
{
	unsigned total_cost = 0;
	std::vector<Edge> output;	//will hold the necessary information to print output
	std::vector<Node*> mst_nodes;	//nodes inside the MST

	Node* temp;

	temp = places[find_index(places, start_vertex)];	//starts creating the MST from here		/		this variable is used so that find_index() isn't called twice as much
	temp->in_mst = true;	//this means temp is now inside the mst
	mst_nodes.push_back(temp);


	//this block checks if direct connections exist from GP to Hippodrom or any Church, connects them initally to GP. Church with the minimum cost is chosen.
	{//just limits the scope
		unsigned church_distance = 10000;
		Edge temp_edge;
		Edge hippo_edge;
		bool flag1 = false;
		bool flag2 = false;
		for (unsigned i = 0; i < temp->children.size(); i++) {
			if (temp->children[i]->name.substr(0, 2) == "Ch") {	//if that child is church
				if (temp->weights[i] < church_distance) {
					church_distance = temp->weights[i];
					
					temp_edge.first = temp->name;
					temp_edge.second = temp->children[i]->name;
					temp_edge.weight = church_distance;

					flag1 = true;
				}
			}
			else if (temp->children[i]->name.substr(0, 4) == "Hipp") {
				hippo_edge.first = temp->name;
				hippo_edge.second = temp->children[i]->name;
				hippo_edge.weight = temp->weights[i];

				flag2 = true;
			}
		}
		if (flag1) {
			temp = places[find_index(places, temp_edge.second)];
			
			mst_nodes.push_back(temp);
			temp->in_mst = true;
			
			output.push_back(temp_edge);

			total_cost += temp_edge.weight;
		}
		if (flag2) {
			temp = places[find_index(places, hippo_edge.second)];

			mst_nodes.push_back(temp);
			temp->in_mst = true;

			output.push_back(hippo_edge);

			total_cost += hippo_edge.weight;
		}
	}

	//adds nodes to MST until no other can be added, if all the nodes are in the MSTi no other node can be added and "bool node_added" becomes false
	bool node_added = true;
	while (node_added) {
		node_added = false;
		unsigned local_min = 9999;
		Edge temp_edge;
		for (unsigned i = 0; i < mst_nodes.size(); i++) {
			for (unsigned j = 0; j < mst_nodes[i]->children.size(); j++) {
				if (!(mst_nodes[i]->children[j]->in_mst)) {
					if (mst_nodes[i]->weights[j] < local_min) {
						local_min = mst_nodes[i]->weights[j];
						
						temp_edge.first = mst_nodes[i]->name;
						temp_edge.second = mst_nodes[i]->children[j]->name;
						temp_edge.weight = local_min;

						node_added = true;
					}
				}
			}
		}
		//if a node is added, MST grows in size
		if (node_added) {
			temp = places[find_index(places, temp_edge.second)];
			
			mst_nodes.push_back(temp);
			temp->in_mst = true;
			
			output.push_back(temp_edge);

			total_cost += temp_edge.weight;
		}

	}

	//reorganizes edge printing order, as in the txt file
	for (unsigned i = 0; i < output.size(); i++) {
		for (unsigned j = 0; j < city_plan.size(); j++) {
			if ((output[i].first == city_plan[j].second) && (output[i].second == city_plan[j].first)) {
				std::string temp_name = output[i].first;
				output[i].first = output[i].second;
				output[i].second = temp_name;
			}
		}
	}

	//sorts the output in ascending order of weights
	std::sort(output.begin(), output.end(), SortingFunc);

	//Prints the MST with the wanted format
	for (unsigned i = 0; i < output.size(); i++) {
		std::cout << output[i].first << " " << output[i].second << " " << output[i].weight << "\n";
	}
	std::cout << total_cost << "\n";

	//memory clean-up
	output.clear();
	mst_nodes.clear();	//of course the nodes it points to are not deleted. Since they exist in the graph itself, they will be cleared when the clear() function of the graph is called.
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

//To use within std::sort	/	Used when sorting a vector of a built in type, with respect to a specific variable inside its elements.
bool SortingFunc(Edge edge1, Edge edge2) {
	return (edge1.weight < edge2.weight);
}
