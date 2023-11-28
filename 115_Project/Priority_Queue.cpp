#include "Priority_Queue.h"

PriorityQueue::PriorityQueue(int c)
{
	capacity = c;
	nelements = 0;

	queue = new QueueNode[c];
}

PriorityQueue::~PriorityQueue()
{
	delete[] queue;
}

int PriorityQueue::getParent(int child)
{
	if (child % 2 == 0)
		return(child / 2) - 1;
	else
		return child / 2;
}

int PriorityQueue::getLeftChild(int parent)
{
	return(2 * parent + 1);
}

int PriorityQueue::getRightChild(int parent)
{
	return(2 * parent + 2);
}

void PriorityQueue::minHeapify(int i, int size)
{
	int left, right, smallest;

	left = 2 * i + 1;
	right = 2 * i + 2;
	smallest = i;

	if ((left < size) && queue[left].weight < queue[i].weight)
	{
		smallest = left;
	}
	if ((right < size) && queue[right].weight < queue[smallest].weight)
	{
		smallest = right;
	}
	if (smallest != i)
	{
		swap(queue[i], queue[smallest]);
		minHeapify(smallest, size);
	}
}

void PriorityQueue::buildMinHeap()
{
	for (int i = nelements / 2 - 1; i >= 0; i--)
	{
		minHeapify(i, nelements);
	}
}

void PriorityQueue::push(int index, int weight)
{
	if (nelements < capacity)
	{
		QueueNode x;
		x.vertex = index;
		x.weight = weight;

		int i = nelements;
		while ((i != 0) && (x.weight < queue[i / 2].weight))
		{
			queue[i] = queue[i / 2];
			i /= 2;
		}
		queue[i] = x;
		nelements++;
		//cout << "Added (" << index << "," << weight << ") size =" << nelements << endl;
	}
	else
	{
		cout << "Queue Full!" << endl;
	}
}

QueueNode PriorityQueue::pop()
{
	if (nelements > 0)
	{
		QueueNode tmp = queue[0];
		queue[0] = queue[nelements - 1];
		minHeapify(0, nelements);
		nelements--;
		return tmp;
	}
	else
	{
		QueueNode tmp;
		tmp.weight = 0;
		tmp.vertex = -1;
		return tmp;
	}
}

void PriorityQueue::display()
{
	cout << "Priority Queue" << endl;
	cout << "Capacity: " << capacity << endl;
	cout << "nelements: " << nelements << endl;
	for (int i = 0; i < nelements; i++)
	{
		cout << i << ": (" << queue[i].vertex << "," << queue[i].weight << ")" << endl;
	}
	cout << endl;
}

void PriorityQueue::DecreaseKey(int index, double value)
{
	int i = 0;
	bool found = false;
	while ((!found) && (i < nelements))
	{
		if (queue[i].vertex == index)
		{
			queue[i].weight = value;
			found = true;
		}
		i++;
	}
	if (found)
	{
		int child = i - 1;
		int parent = getParent(child);
		while ((queue[child].weight < queue[parent].weight) && (child >= 0 && parent >= 0))
		{
			swap(queue[child], queue[parent]);
			child = parent;
			parent = getParent(child);
		}
	}
}