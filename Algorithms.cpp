#include "Graph.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

// HELPER METHODS:

std::vector<Node> intersect(std::vector<Node> n1, std::vector<Node> n2) {
	std::vector<Node> common;
	for(Node n : n1) {
		for(Node m : n2) {
			if(n.getNumber() == m.getNumber()) {
				common.push_back(n);
				break;
			}
		}
	}

	return common;
}

// filters out the same multiple 3-Cliques
std::vector<Graph> filter(std::vector<Graph> collection) {
	std::vector<Node> commonNodes;
	for(unsigned int g = 0; g < collection.size(); g++) {
		for(unsigned int i = g + 1; i < collection.size(); i++) {
			commonNodes = intersect(collection.at(g).getNodes(), collection.at(i).getNodes());
			if(commonNodes.size() == collection.at(i).getNodes().size() && commonNodes.size() == collection.at(g).getNodes().size()) {
				collection.erase(collection.begin() + i);
				i--;
			}
		}
	}

	return collection;
}

std::vector<Graph> grow(Graph graph, Graph clique) {

	std::vector<Graph> collection;
	std::vector<Node> commonNodes = graph.getNodes();

	// find node connected to every other node
	for(Node c : clique.getNodes()) {
		commonNodes = intersect(graph.getNodes().at(c.getNumber()).getNeighbours(), commonNodes);
	}

	if(commonNodes.size() == 0) {collection.push_back(clique); return collection;}

	else {
		for(unsigned int i = 0; i < commonNodes.size(); i++) {
			// add new clique node to neighbours
			Graph bigger = clique;
			Node n(commonNodes.at(i).getNumber()); // = commonNodes.at(i) replacement
			n.setNeighbours(bigger.getNodes());
			for(unsigned int s = 0; s < bigger.getNodes().size(); s++) {
				bigger.nodes.at(s).neighbours.push_back(n);
			}
			// add new clique node itself
			std::vector<Node> nodes = bigger.getNodes();
			nodes.push_back(n);
			bigger.setNodesRefresh(nodes);

			// save bigger cliques
			collection.push_back(bigger);
		}

		return collection;
	}


}

std::vector<Graph> growCliques(Graph graph, std::vector<Graph> smallerCliques) {
	
	std::vector<Graph> collection;

	for(Graph clique : smallerCliques) {
		std::vector<Graph> temp = grow(graph, clique);
		collection.insert(collection.end(), temp.begin(), temp.end());
	}

	return filter(collection);
}

bool isZeroRow(std::vector<std::vector<int>> adj, int row) {

	for(int j = 0; j < adj.at(row).size(); j++) {
		if(adj.at(row).at(j) == 1) {
			return false;
		}
	}

	return true;
}


bool contains(std::vector<Graph> collection, Graph graph) {

	for(Graph g : collection) {
		if(g.getNodes().size() == graph.getNodes().size()) {
			int i = 0;
			while(i < g.getNodes().size() && g.getNodes().at(i).getNumber() == graph.getNodes().at(i).getNumber()) {
				if(i == g.getNodes().size() - 1) {
					return true;
				}
				i++;
			}
		}
	}

	return false;
}

std::vector<std::vector<int>> multiply(std::vector<std::vector<int>> A, std::vector<std::vector<int>> B) {

	std::vector<std::vector<int>> C;
	for(unsigned int i = 0; i < A.size(); i++) {
		std::vector<int> newLine;
		for(unsigned int k = 0; k < A.size(); k++) {
			int sum = 0;
			for(unsigned int j = 0; j < A.size(); j++) {
				sum += A.at(i).at(j) * B.at(j).at(k);
			}
			newLine.push_back(sum);
		}
		C.push_back(newLine);
	}
	return C;
}

Graph getMaximalGraph(std::vector<Graph> graphs) {

	int max = 0;
	int index = 0;


	for(int i = 0; i < graphs.size(); i++) {
		// graphs.at(i).print();
		if(graphs.at(i).getNodes().size() > max) {
			max = graphs.at(i).getNodes().size();
			index = i;
		}
	}

	return graphs.at(index);
}

Graph getAntiGraph(Graph graph) {

	std::vector<std::vector<int>> adj = graph.getAdjacencyMatrix();
	std::vector<std::vector<int>> new_adj = adj;

	for(int i = 0; i < adj.size(); i++) {

		if(isZeroRow(adj, i)) {
			continue;
		}

		for(int j = 0; j < adj[i].size(); j++) {

			if(i == j) {
				new_adj[i][j] = 0;
			}

		    else if(adj[i][j] == 1) {
				new_adj[i][j] = 0;
			}
			else {
				new_adj[i][j] = 1;
			}
		}
	}

	Graph antigraph(new_adj);

	return antigraph;
}

// construct clique matrix from graph 3-Cliques
std::vector<std::vector<int>> getCliqueMatrix(std::vector<Graph> triangles, Graph graph) {

	std::vector<std::vector<int>> C = graph.getAdjacencyMatrix();

	// C := -C
	for(unsigned int i = 0; i < C.size(); i++) {
		for(unsigned int j = 0; j < C.at(i).size(); j++) {
			int temp = C.at(i).at(j);
			C.at(i).at(j) = - temp;
		}
	}

	// c_ij = 1 if (i,j) triangle edge else - 1 or 0
	for(Graph cl : triangles) {
		C.at(cl.getNodes().at(0).getNumber()).at(cl.getNodes().at(1).getNumber()) = 1;
		C.at(cl.getNodes().at(1).getNumber()).at(cl.getNodes().at(0).getNumber()) = 1;

		C.at(cl.getNodes().at(0).getNumber()).at(cl.getNodes().at(2).getNumber()) = 1;
		C.at(cl.getNodes().at(2).getNumber()).at(cl.getNodes().at(0).getNumber()) = 1;

		C.at(cl.getNodes().at(1).getNumber()).at(cl.getNodes().at(2).getNumber()) = 1;
		C.at(cl.getNodes().at(2).getNumber()).at(cl.getNodes().at(1).getNumber()) = 1;
	}

	return C;
}

void printMatrix(std::vector<std::vector<int>> matrix) {
	for(int i = 0; i < matrix.size(); i++) {
		for(int j = 0; j < matrix.at(i).size(); j++) {
			std::cout << matrix.at(i).at(j) << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	return;
}

// random adjacency matrix of size n where node neighbours <= rowcount
std::vector<std::vector<int>> getRandomMatrix(int size, int rowcount) {

	std::vector<int> line(size, 0);
	std::vector<std::vector<int>> adj(size, line);
	int randomInt;
	std::vector<int> rowsum(size, 0);
	std::vector<int> colsum(size, 0);
	srand(time(0));

	for(int i = 0; i < size - 1; ++i) {
		while(rowsum.at(i) <= rowcount)  {
			randomInt = rand() % (size - i - 1) + i + 1;
			rowsum.at(i)++;
			rowsum.at(randomInt)++;
			colsum.at(randomInt)++;
			colsum.at(i)++;
			adj.at(i).at(randomInt) = 1;
			adj.at(randomInt).at(i) = 1;
		}
	}

	return adj;
}

Graph del_unique(Graph graph) {

	Graph antigraph = getAntiGraph(graph);
	std::vector<Node> maxNodes = antigraph.getMaxEdgeNodes();

	// while max edge node is unique --> deletion clique-safe!
	while(maxNodes.size() == 1) {
		antigraph.print();
		antigraph.deleteNode(maxNodes.at(0));
		graph.deleteNode(maxNodes.at(0));
		maxNodes = antigraph.getMaxEdgeNodes();

	}

	return graph;
}


Graph del(Graph graph) {

	Graph antigraph = getAntiGraph(graph);
	std::vector<Node> maxNodes = antigraph.getMaxEdgeNodes();

	// while max edge node exists --> deletion clique-unsafe but interesting!
	while(maxNodes.size() > 0) {
		antigraph.deleteNode(maxNodes.at(0));
		graph.deleteNode(maxNodes.at(0));
		maxNodes = antigraph.getMaxEdgeNodes();
	}

	return graph;
}

std::vector<Graph> del_recursive(Graph graph, Graph antigraph) {

	std::vector<Graph> collection;
	std::vector<Node> maxNodes;

	maxNodes = antigraph.getMaxEdgeNodes();

	// if no edges then return
	if(maxNodes.size() == 0) {
		graph.print();
		collection.push_back(graph);
		return collection;
	}

	// delete every maxNode successively and collect returns
	for(int i = 0; i < maxNodes.size(); i++) {
		// std::cout << "LOOP " << i << "\n";
		Graph copy = graph;
		Graph anticopy = antigraph;

		copy.deleteNode(maxNodes.at(i));
		anticopy.deleteNode(maxNodes.at(i));

		std::vector<Graph> temp = del_recursive(copy, anticopy);

		collection.insert(collection.end(), temp.begin(), temp.end());
	}

	return collection;

}

std::vector<Graph> CL3(Graph graph) {

	std::vector<Graph> collection;
	std::vector<int> candidates;
	std::vector<std::vector<int>> adj = graph.getAdjacencyMatrix();
	std::vector<std::vector<int>> square = multiply(adj, adj);

	// get two candidates each
	for(int i = 0; i < adj.size(); i++) {
		for(int j = i + 1; j < adj.at(i).size(); j++) {
			if(adj.at(i).at(j) > 0 && square.at(i).at(j) > 0) {
				candidates.push_back(i);
				candidates.push_back(j);
			}
		}
	}

	if(candidates.size() == 0) {return collection;}

	// get third node
	for(int c = 0; c < candidates.size() - 1; c += 2) {
		int i = candidates.at(c);
		int j = candidates.at(c + 1);
		std::vector<Node> commonNeighbours = intersect(graph.getNodes().at(i).getNeighbours(), graph.getNodes().at(j).getNeighbours());
		for(Node n : commonNeighbours) {
			// add 3-Clique
			Node ni(graph.getNodes().at(i).getNumber());
			Node nj(graph.getNodes().at(j).getNumber());
			Node nk(n.getNumber());
			ni.setNeighbours({nj, nk});
			nj.setNeighbours({ni, nk});
			nk.setNeighbours({ni, nj});
			Graph cl({ni, nj, nk});
			collection.push_back(cl);
		}
	}

	// filter out repeated cliques
	return filter(collection);
}


std::vector<Graph> CLIQUE(Graph graph, int k) {

	int prev_size = 2;
	int size = 3;
	std::vector<Graph> temp = CL3(graph);
	std::vector<Graph> collection = temp;

	for(int i = 4; i <= k; i++) {
		temp = growCliques(graph, temp);
		collection.insert(collection.end(), temp.begin(), temp.end());
	}

	return filter(collection);
}



int main() {

	std::vector<std::vector<int>> adj = getRandomMatrix(10, 6);

	Graph g(adj);
	g.printToFile("my_graph");

	std::vector<Graph> cliques = CLIQUE(g, 6);

	for(Graph cl : cliques) {
		cl.print();
	}


//	std::vector<std::vector<int>> C = getCliqueMatrix(collection, g);
//	printMatrix(C);
//	std::vector<std::vector<int>> CC = multiply(C, C);
//	printMatrix(CC);
//	std::vector<std::vector<int>> CCC = multiply(C, CC);
//	printMatrix(CCC);
//
//	for(Graph c : collection) {
//		c.print();
//	}





/*
	std::cout << "Creating Graph \n";
	Graph graph(adj);
	std::cout << "Get Anti Graph \n";
	Graph antigraph = getAntiGraph(graph);

	std::cout << "Writing to File \n";

	antigraph.printToFile("my_antigraph");

	std::cout << "\n Algorithm: \n";

	Graph cl = del(graph);
	std::cout << "CLIQUE: \n";
	cl.print(); */

   /*   std::vector<Graph> graphs = del_recursive(graph, antigraph);

	std::cout << "RESULTS: \n";

	Graph cl = getMaximalGraph(graphs);

	std::cout << "MAXIMAL: \n";
	cl.print();
	cl.printToFile("my_result"); */



	return 0;
}
