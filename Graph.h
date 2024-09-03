#include <vector>
#include <string>

#ifndef GRAPH_H_
#define GRAPH_H_


class Node {
private:
	int number;
public:
	std::vector<Node> neighbours;
	Node(int number);
	int getNumber();
	std::vector<Node> getNeighbours();
	void setNeighbours(std::vector<Node> neighbours);

};

class Graph {
private:
	std::vector<std::vector<int>> adjacencyMatrix;
public:
	std::vector<Node> nodes;
	Graph(std::vector<std::vector<int>> adjacencyMatrix);
	Graph(std::vector<Node> nodes);
	std::vector<Node> getNodes();
	void setNodesRefresh(std::vector<Node> nodes);
	std::vector<Node> getMaxEdgeNodes();
	std::vector<std::vector<int>> getAdjacencyMatrix();
	void deleteNode(Node node);

	void print();
	void printToFile(std::string name);
	void printMatrix();
};




#endif /* GRAPH_H_ */
