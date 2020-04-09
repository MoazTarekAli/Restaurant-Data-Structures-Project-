#pragma once
#include <iostream>
#include "Node.h"

#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

using namespace std;

template <typename T>
class LinkedList
{
private:

	Node<T>* head;		// Pointer to the first item in the linkedlist
	Node<T>* tail;		// Pointer to the last item in the linkedlist
	int count;			// Counts the number of nodes in the linkedlist

public:

	// Linkedlist constructor that initializes data members to avoid garbage output

	LinkedList()
	{
		head = nullptr;
		tail = nullptr;
		count = 0;
	}
	
	// Linkedlist destructor deletes all nodes in the list

	~LinkedList()
	{
		DeleteAll();
	}

	// IsEmpty function returns true for an empty list

	bool IsEmpty()
	{
		if (head)
			return false;		
		return true;
	}

	// GetCount function returns number of nodes in the list

	int GetCount()
	{
		return count;
	}

	// Print function to print linkedlist

	void PrintList()	const
	{
		// Pointer to traverse the list for printing 

		Node<T>* printTraverse = head;

		while (printer) // exits the loop at the last item in the linkedlist 
		{
			cout << printTraverse->getItem() << " ";
			printTraverse = printTraverse->getNext();
		}
	}


	// Adds new item to the linkedlist assigning it to the tail of the list

	void Append(T entry)
	{
		// Create new node to insert in the list

		Node<T>* appender = new Node<T>(entry); 
		
		if (!count)		// Check if the list is empty
		{
			// For the first item in the list the head and tail will point to it

			head = tail = appender; 
			count++;
			return;
		}

		tail->setNext(appender);
		tail = appender;
		count++;
		return;
	}

	// Insert function to add an item to the linked list in given position

	void Insert(const T item, int index)
	{
		// Step 1: Check if the index inserted is out of bounds to terminate if true

		if (!(index >= 0 && index < count))
			return;

		Node<T>* traverser = head;				// Node pointer to traverse our list
		Node<T>* newitem = new Node<T>(item);	// Create a new node to insert into the list



		//	Step 2: Handling special cases
		//	Special case handling inserting in the 0 index which replaces the head

		if (index == 0)
		{
			newitem->setNext(head);
			head = newitem;
			count++;
			return;
		}

		//	Special case handling inserting in the last index replacing the head

		if (index == count)
		{
			tail->setNext(newitem);
			tail = newitem;
			count++;
			return;
		}

		//	Step 3: loop to traverse to the given index

		for (int i = 0; i < index - 1; i++)
		{
			traverser = traverser->getNext();
		}

		//	Step 4: Adding the input item in the input index 

		newitem->setNext(traverser->getNext());
		traverser->setNext(newitem);
		
		//	Step 5: increasing count

		count++; 

	}

	// DeleteAll function to remove all nodes in the linkedlist

	void DeleteAll()
	{
		Node<T>* Deleter = head;
		while (head)
		{
			Deleter = head->getNext();
			delete head;
			head = Deleter;
		}
		count = 0;
		tail = head;
	}

	void Delete(int index)
	{
		// Check if the index inserted is out of bounds to terminate if true

		if (!(index >= 0 && index < count))
			return;

		// Temporary pointer to delete and free the allocated memory

		Node<T>* tempDel = head;
		
		if (index == 0)
		{
			head = head->getNext();
			delete tempDel;
			return;
		}
		
		// Traversing pointer to go to given index to delete

		Node<T>* traverseToDel = tempDel;

		for (int i = 0; i < index; i++)
		{
			traverseToDel = tempDel;
			tempDel = tempDel->getNext();
		}

		// Deleting action

		traverseToDel->setNext(tempDel->getNext());
		delete tempDel;

	}


};
#endif
