#pragma once
#include "Queue.h"

struct QueueNode
{
	int vertex;
	double weight;
};

class PriorityQueue
{
private:
	int capacity;
	int nelements;
	QueueNode* queue;
	void minHeapify(int i, int size);
public:
	PriorityQueue(int c);
	~PriorityQueue();
	void buildMinHeap();
	void push(int index, int weight);
	QueueNode pop();
	void DecreaseKey(int index, double value);
	void display();

	int getParent(int child);
	int getLeftChild(int parent);
	int getRightChild(int parent);

	bool isEmpty() { return(nelements == 0); }
	bool isFull() { return(nelements == capacity); }
};

