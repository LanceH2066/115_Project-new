#include "Queue.h"

Queue::Queue()
{
	front = rear = nullptr;
}

void Queue::Enqueue(int x)
{
	QNode* temp = new QNode(x);

	if (!rear)
	{
		front = rear = temp;
	}
	else
	{
		rear->next = temp;
		rear = temp;
	}
}

void Queue::Dequeue()
{
	if (front)
	{
		// Store previous front and
		// move front one node ahead
		QNode* temp = front;
		front = front->next;

		// If front becomes NULL, then
		// change rear also as NULL
		if (!front)
			rear = nullptr;

		delete (temp);
	}
}