#pragma once
#include <fstream>
#include <string>
#include "Priority_Queue.h"

class nodeAL 
{
public:

	int vertex;					//vertex number
	double weight;				//movement penalty for the final proj
	char landType;				//for final proj
	nodeAL* next;				//linked list of connections

	nodeAL();					//constructor
	nodeAL(int v, double w);	//constructor with vertex and weight parameters
	nodeAL(int v, double w, char type);		//constructor with vertex, weight, and landtype

	
};

class GraphAL
{
private:

	int n;					// # of Vertices
	int* degree;			// Degree of each vertex

	nodeAL** a;				// Array Of NodeAL
	nodeAL** current;		// Iterators

public:

	GraphAL();
	GraphAL(int n1);
	~GraphAL();

	//Functions-------------------

	bool existEdge(int v1, int v2);						//check if there is an edge between two vertices
	double getEdgeWeight(int v1, int v2);				//return edge weight
	nodeAL* retrieveEdge(int v1, int v2);
	void setDirectedEdge(int v1, int v2, double w);		//set edge
	void setDirectedEdge(int v1, int v2, double w, char type);
	void setDirectedEdge(int v1, int v2);
	void setDirectedEdge(int v1, int v2, char type);
	void removeDirectedEdge(int v1, int v2);			//remove edge
	void Display();										//print lists

	//TESTING
	void setUndirectedEdge(int v1, int v2, double weight, char type);

	void setDirectedEdgeTest(int source, int destination, double weight, char type);

	void addNodeToAdjList(int source, int destination, double weight, char type);


	void setNodeWeight(int node, double w);
	double getNodeWeight(int node);
	char getNodeLandtype(int node);
	void setLandType(int node, char c);

	// Iterator Functions
	bool ExistAdjacent(int u);
	void SetCurrentVertex(int u);
	int GetNextAdjacent(int u);

	// Dijkstra Shortest Path
	void shortestPath(int s, double*& d, int*& pi);
	void DisplayShortestPath(double* d, int* pi, int n);
};

