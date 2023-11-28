#include "GraphAL.h"


nodeAL::nodeAL() //constructor
{
	vertex = 0;
	weight = 0;
	landType = ' ';
	next = nullptr;
}

nodeAL::nodeAL(int v, double w)
{
	vertex = v;
	weight = w;
	landType = ' ';
	next = nullptr;
}

nodeAL::nodeAL(int v, double w, char type) {
	vertex = v;
	weight = w;
	landType = type;
	next = nullptr;
}



GraphAL::GraphAL()
{
	n = 0;
	a = nullptr;
	current = nullptr;
	degree = nullptr;
}

GraphAL::GraphAL(int n1)
{
	n = n1;
	a = new nodeAL * [n];
	degree = new int[n];
	current = new nodeAL * [n];

	for (int i = 0; i < n; i++)
	{
		a[i] = new nodeAL(i, 0);
		current[i] = nullptr;
		degree[i] = 0;
	}
}

GraphAL::~GraphAL()
{
	for (int i = 0; i < n; i++)
	{
		nodeAL* current = a[i];
		while (current)
		{
			nodeAL* next = current->next;
			delete current;
			current = next;
		}
	}
	delete[] a;
	delete[] current;
	delete[] degree;
}


//Functions-------------------

bool GraphAL::existEdge(int v1, int v2) //check if there is an edge between two vertices
{
	if (v1 < n)
	{
		nodeAL* cursor = a[v1];

		while (cursor)
		{
			if (cursor->vertex == v2)
			{
				return true;
			}
			else
			{
				cursor = cursor->next;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

double GraphAL::getEdgeWeight(int v1, int v2)
{
	if (v1 < n)
	{
		nodeAL* cursor = a[v1];
		while (cursor != nullptr)
		{
			if (cursor->vertex == v2)
			{
				return cursor->weight;
			}
			else
			{
				cursor = cursor->next;
			}
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

void GraphAL::setDirectedEdge(int v1, int v2, double w)
{
	if (!existEdge(v1, v2))
	{
		nodeAL* newNode = new nodeAL(v2, w);
		//if list is empty add the new node
		if (a[v1] == nullptr) {
			a[v1] = newNode;
		}
		else {
			//traverse to the end
			nodeAL* current = a[v1];
			while (current->next != nullptr) {
				current = current->next;
			}

			current->next = newNode;
		}
		degree[v1]++;
	}
}

void GraphAL::setDirectedEdge(int v1, int v2, double w, char type) {
	if (!existEdge(v1, v2))
	{
		nodeAL* newNode = new nodeAL(v2, w, type);
		//if list is empty add the new node
		if (a[v1] == nullptr) {
			a[v1] = newNode;
		}
		else {
			//traverse to the end
			nodeAL* current = a[v1];
			while (current->next != nullptr) {
				current = current->next;
			}

			current->next = newNode;
		}
		degree[v1]++;
	}
}


void GraphAL::removeDirectedEdge(int v1, int v2)
{
	nodeAL* cursor = a[v1];
	nodeAL* previous = nullptr;

	while (cursor != nullptr)
	{
		if (cursor->vertex == v2)
		{
			if (previous == nullptr)
			{
				// This is the first element of the list
				a[v1] = cursor->next;
			}
			else
			{
				// This is not the first element of the list
				previous->next = cursor->next;
			}
			delete cursor;
			degree[v1]--;
			return; // Found and removed the edge, no need to continue.
		}
		previous = cursor;
		cursor = cursor->next;
	}
}

void GraphAL::Display()
{
	for (int i = 0; i < n; i++)
	{
		cout << i << ":";
		nodeAL* cursor = a[i];
		while (cursor)
		{
			cout << cursor->vertex << "(" << cursor->weight << ") " << "Landtype:";
			cout << cursor->landType << " ";
			cursor = cursor->next;
		}
		cout << endl;
	}
}


void GraphAL::setUndirectedEdge(int v1, int v2, double weight, char type) {
	setDirectedEdge(v1, v2, weight, type);
	setDirectedEdge(v2, v1, weight, type);
}


void GraphAL::setNodeWeight(int node, double w) {
	if (node < n) {
		a[node]->weight = w;
	}
}

nodeAL* GraphAL::retrieveEdge(int v1, int v2) //retrieves edge(node v2 in list of v1)
{
	if (v1 < n)
	{
		nodeAL* cursor = a[v1];
		while (cursor != nullptr)
		{
			if (cursor->vertex == v2)
			{
				return cursor;
			}
			cursor = cursor->next;
		}
	}
	else
	{
		return nullptr;
	}
}

// ITERATOR FUNCTIONS
bool GraphAL::ExistAdjacent(int u)
{
	return current[u] != nullptr;
}
void GraphAL::SetCurrentVertex(int u)
{
	current[u] = a[u];
}
int GraphAL::GetNextAdjacent(int u)
{
	int v = current[u]->vertex;
	current[u] = current[u]->next;
	return v;
}

// Dijkstra Shortest Path
void GraphAL::shortestPath(int s, double*& d, int*& pi)
{
	d = new double[n];	//Distance
	pi = new int[n];	//Predecessor

	PriorityQueue* V = new PriorityQueue(n);

	Queue* S = new Queue;

	for (int u = 0; u < n; u++) //Initialize
	{
		d[u] = INT_MAX;
		pi[u] = -1;
	}

	d[s] = 0;	//Source Distance = 0

	for (int u = 0; u < n; u++) // Push (Vertex,Distance) into Priority Queue
	{
		V->push(u, d[u]);
	}

	while (!V->isEmpty())
	{
		// Pop Vertex From Priority Queue and Iterate
		QueueNode out = V->pop();
		S->Enqueue(out.vertex);
		int u = out.vertex;
		SetCurrentVertex(u);

		while (ExistAdjacent(u)) // Relax
		{
			int v = GetNextAdjacent(u);
			double w = getEdgeWeight(u, v);
			if (d[v] > d[u] + w)
			{
				d[v] = d[u] + w;
				pi[v] = u;
				V->DecreaseKey(v, d[v]);
			}
		}
	}

	delete V;
	delete S;
}

void GraphAL::DisplayShortestPath(double* d, int* pi, int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << "Vertex: " << i
			<< " : d=" << d[i]
			<< " : pi=" << pi[i]
			<< endl;
	}
	cout << endl;
}

double GraphAL::getNodeWeight(int node) {
	if (node < n) {
		return a[node]->weight;
	}
	return 0.0;
}

void GraphAL::setLandType(int node, char c) {
	if (node < n) {
		a[node]->landType = c;
		//cout << "a[" << node << "]->landType: " << a[node]->landType << endl;
	}
	return;
}

char GraphAL::getNodeLandtype(int node) {
	if (node < n) {
		
		return a[node]->landType;
	}
	return 'u';
}