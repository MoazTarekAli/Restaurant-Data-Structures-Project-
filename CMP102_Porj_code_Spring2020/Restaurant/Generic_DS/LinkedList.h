#pragma once
#include <iostream>
#include "Node.h"

using namespace std;

template <typename T>
class LinkedList
{
private:

	Node<T>* head;
	Node<T>* tail;
	int count;

public:

	LinkedList()
	{
		head = nullptr;
		tail = nullptr;
		count = 0;
	}
	
	~LinkedList()
	{
		DeleteAll();
	}

	bool IsEmpty()
	{
		if (head)
			return true;
		else
			return false;
	}

	int GetCount()
	{
		return count;
	}

	void PrintList()	const
	{
		Node<T>* p = head;

		while (p)
		{
			cout << p->getItem() << " ";
			p = p->getNext();
		}
	}



	void Append(T entry)
	{

		Node<T>* pointer = new Node<T>(entry);
		
		if (!count)
		{
			head = tail = pointer;
			count++;
			return;
		}

		tail->setNext(pointer);
		tail = pointer;
		count++;
		return;
	}

	void Insert(const T item, int index)
	{
		if (!(index >= 0 && index < count))
			return;

		Node<T>* pointer = head;
		Node<T>* newitem = new Node<T>(item);

		if (index == 0)
		{
			newitem->setNext(head);
			head = newitem;
			count++;
			return;
		}

		if (index == count)
		{
			tail->setNext(newitem);
			tail = newitem;
			count++;
			return;
		}

		for (int i = 0; i < index - 1; i++)
		{
			pointer = pointer->getNext();
		}

		newitem->setNext(pointer->getNext());
		pointer->setNext(newitem);
		count++;

	}

	void DeleteAll()
	{
		Node<T>* P = head;
		while (head)
		{
			P = head->getNext();
			delete head;
			head = P;
		}
		count = 0;
		tail = head;
	}

	void Delete(int index)
	{
		if (!(index >= 0 && index < count))
			return;

		Node<T>* pointer = head;
		
		if (index == 0)
		{
			head = head->getNext();
			delete pointer;
			return;
		}
		
		Node<T>* pointer2 = pointer;

		for (int i = 0; i < index; i++)
		{
			pointer2 = pointer;
			pointer = pointer->getNext();
		}

		pointer2->setNext(pointer->getNext());
		delete pointer;
		return;
	}


};

