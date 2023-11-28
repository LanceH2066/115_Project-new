#pragma once
#include <iostream>
using namespace std;

struct QNode
{
	int data;
	QNode* next;

	QNode(int d)
	{
		data = d;
		next = nullptr;
	}
};

class Queue
{
private:
	QNode* front, * rear;
public:

	Queue();

	void Enqueue(int x);
	void Dequeue();
};

