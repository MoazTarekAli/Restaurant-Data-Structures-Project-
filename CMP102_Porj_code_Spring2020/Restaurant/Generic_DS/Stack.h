#pragma once
#include <iostream>
#include "Node.h"

using namespace std;

template < typename T>
class Stack
{
	Node<T>* top;

public:

	Stack()
	{
		// Default stack constructor to make sure top pointer is NULL in empty stack
		
		top = nullptr;
	}
	
	~Stack()	
	{
		// Stack destructor that pops all items of stack to delete them

		while (top)
			pop();
	}

	void push(T entry)
	{
		// Create a new node for the entry

		Node<T>* pusher = new Node<T>(entry);

		if (IsEmpty())	// Function returns true for an empty stack
		{
			// validation for the first item in the stack

			top = pusher;

		}
		else {

			// Store the current top value as the next pointer of the node

			pusher->setNext(top);

			// Changes the current top to the last entered item as stack is LIFO

			top = pusher;
		}
	}
	
	// Checks if the Stack is Empty 

	bool IsEmpty()
	{

		if (top)			// Checks that the top doesn't point to a null pointer
			return false;

		return true;		// returns true when the top pointer is null
	}

	// "Pops" the stack returning the item on the "top" of the stack which is the last entered item 

	T pop()
	{
		if (IsEmpty())	// Validation incase the stack is empty 
			return -1;	// return -1 in case of empty

		T item = top->getItem();	// stores the item in the top of the stack
		Node<T>* tempDel = top;		// Temporary pointer to delete the top
		top = top->getNext();		// moves the top of the stack
		delete tempDel;				// Delete action
		return item;
	}

	T peek()
	{
		if (top)			// Checks if the stack is empty to avoid returning a NULL ptr
			return top;		// Returns the top item in the stack
		
		// If the stack is empty it should return -1

		return -1;
	}
};

