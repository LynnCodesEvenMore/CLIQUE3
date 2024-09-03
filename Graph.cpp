#include "Graph.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

int getNodeIndex(std::vector<Node> nodes, int number) {
	for(int i = 0; i < nodes.size(); i++) {
		if(nodes.at(i).getNumber() == number) {
			return i;
		}
	}

	return -1;
}

Node::Node(int number) {
	this->number = number;
}

int Node::getNumber() {
	return number;
}

std::vector<Node> Node::getNeighbours() {
	return neighbours;
}

void Node::setNeighbours(std::vector<Node> neighbours) {
	this->neighbours = neighbours;
	return;
}

Graph::Graph(std::vector<std::vector<int>> adjacencyMatrix) {
	// constructor with matrix

	this->adjacencyMatrix = adjacencyMatrix;

	std::cout << "Creating Nodes \n";

	for(int i=0; i < adjacencyMatrix.size(); i++) {
		Node n(i);
	    nodes.push_back(n);
	}

	std::cout << "Adding Neighbours \n";
	for(int i=0; i < adjacencyMatrix.size(); i++) {
		std::vector<Node> myNeighbours;
		for(int j=0; j < adjacencyMatrix.at(i).size(); j++) {
			if(adjacencyMatrix.at(i).at(j) > 0) {
				myNeighbours.push_back(nodes.at(j));
			}
		}
		nodes.at(i).setNeighbours(myNeighbours);
	}
}

Graph::Graph(std::vector<Node> nodes) {
	this->nodes = nodes;
	// initialize adjacency matrix
	std::vector<int> row(nodes.size(), 0);
	std::vector<std::vector<int>> adj(nodes.size(), row);

	for(int i = 0; i < nodes.size(); i++) {
		for(int n = 0; n < nodes.at(i).getNeighbours().size(); n++) {
			int j = getNodeIndex(nodes, nodes.at(i).getNeighbours().at(n).getNumber());
			if(j > -1) {
			adj.at(i).at(j) = 1;
			}
		}
	}

	this->adjacencyMatrix = adj;
}

std::vector<Node> Graph::getNodes() {
	return nodes;
}

void Graph::setNodesRefresh(std::vector<Node> nodes) {
	this->nodes = nodes;
	// refresh adjacency matrix
	Graph g(nodes);
	this->adjacencyMatrix = g.getAdjacencyMatrix();
	return;
}

std::vector<std::vector<int>> Graph::getAdjacencyMatrix() {
	return adjacencyMatrix;
}

std::vector<Node> Graph::getMaxEdgeNodes() {
	// returns empty vector if graph has no edges

	int max = 0;
	std::vector<Node> maxNodes;

	for(Node node : nodes) {
		if(node.getNeighbours().size() > max) {
			max = node.getNeighbours().size();
		}
	}

	if(max > 0) {
		for(Node node : nodes) {
			if(node.getNeighbours().size() == max) {
				maxNodes.push_back(node);
			}
		}
	}

	return maxNodes;
}

void Graph::deleteNode(Node node) {

	for(int i = 0; i < nodes.size(); i++) {
		// delete node itself
		if(nodes.at(i).getNumber() == node.getNumber()) {
			// deleting node in nodes
			nodes.erase(nodes.begin() + i);
			i--; // index one too much after deletion
		}

		else {
			// delete node as neighbour node
			for(int n = 0; n < nodes.at(i).getNeighbours().size(); n++) {
				if(nodes.at(i).getNeighbours().at(n).getNumber() == node.getNumber()) {
					// deleting node in neighbours
					std::vector<Node> myNeighbours = nodes.at(i).getNeighbours();
					myNeighbours.erase(myNeighbours.begin() + n);
					nodes.at(i).setNeighbours(myNeighbours);
					break;
				}
			}
		}
	}

// NODES IN ADJACENCY MATRIX CANNOT BE DELETED FULLY!!!
	// delete node row in adjacency matrix with 0s
	for(int j = 0; j < adjacencyMatrix.at(node.getNumber()).size(); j++) {
		adjacencyMatrix.at(node.getNumber()).at(j) = 0;
	}
	// delete node column in adjacency matrix with 0s
	for(int row = 0; row < adjacencyMatrix.size(); row++) {
		adjacencyMatrix.at(row).at(node.getNumber()) = 0;
	}


	return;
}

void Graph::print() {

	for(Node node : nodes) {

		std::cout << node.getNumber() << ": ";
		std::vector<Node> neighbours = node.getNeighbours();

		for(Node neighbour : neighbours) {
			std::cout << neighbour.getNumber() << " | ";
		}

		std::cout << "\n";
	}

	std::cout << "\n";

	return;
}

void Graph::printMatrix() {
	for(int i = 0; i < adjacencyMatrix.size(); i++) {
		for(int j = 0; j < adjacencyMatrix.at(i).size(); j++) {
			std::cout << adjacencyMatrix.at(i).at(j) << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	return;
}

void Graph::printToFile(std::string name) {

	std::string filename = name + ".gv";
	const char* path = filename.c_str();

	  std::ofstream myfile (path);
	  if (myfile.is_open())
	  {
		  myfile << "graph { \n";
		for(int i = 0; i < adjacencyMatrix.size(); ++i) {
			bool hasEdges = false;
			for(int j = i; j < adjacencyMatrix.at(i).size(); ++j) {
				if(adjacencyMatrix.at(i).at(j) > 0) {
					myfile << i;
					myfile << " -- ";
					myfile << j;
					myfile << ";\n";
					hasEdges = true;
				}
				else if(j == adjacencyMatrix.size()-1 && !hasEdges) {
					myfile << i;
					myfile << ";\n";
				}
			}
		}
		myfile << "}\n";
	    myfile.close();
	  }
	  else {std::printf("Unable to open file\n");}

	return;
}
