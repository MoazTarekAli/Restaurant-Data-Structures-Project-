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

	void RemoveEvensAfterOdds()
	{
		Node<T>* pointer = head;
		int OddIndex = -1;
		for (int i = 0; i < count; i++)
		{
			if (pointer->getItem() % 2 == 0)
			{
				pointer = pointer->getNext();
			}
			else 
			{
				OddIndex = i;
				break;
			}
		}

		if (OddIndex != -1) 
		{
			pointer = pointer->getNext();
			
			for (int i = OddIndex+1; i < count; i++)
			{
				
				if ((pointer->getItem()) % 2 == 0)
				{
					pointer = pointer->getNext();
					this->Delete(i--);
					count--;
					
				}
				else
				{
					pointer = pointer->getNext();
				}
				if (!pointer)
				{
					break;
				}
			}
		}
	}

	void CompletesTen()
	{
		Node<T>* pointer = head;

		for (int i = 0; i < count; i++)
		{
			if (pointer->getItem() < 10)
			{
				this->Insert(10 - pointer->getItem(), i + 1);
				count++;
				pointer = pointer->getNext();
				i++;				
			}
			if (!pointer)
				break;
			pointer = pointer->getNext();
			if (!pointer)
				break;
		}
	}

	void RemovesEquals()
	{
		int target = head->getItem();
		Node<T>* current = head->getNext();
		int targetindex = 0;
		bool wasconcurrent = false;
		for (int i = 1; i < count; ++i)
		{
			if (target == current->getItem())
			{
				current = current->getNext();
				this->Delete(i);
				count--;
				i--;
				wasconcurrent = true;
			}
			else
			{
				if (wasconcurrent)
				{
					wasconcurrent = false;
					this->Delete(targetindex);
					count--;
					i--;		
				}
				targetindex = i;
				target = current->getItem();
				current = current->getNext();
			}
			if (!current)
				break;			
		}
		if (wasconcurrent)
		{
			wasconcurrent = false;
			this->Delete(targetindex);
			count--;
		}
	}

	void CompressEquals()
	{
		int target = head->getItem();
		Node<T>* current = head->getNext();
		int targetindex = 0;
		bool wasconcurrent = false;
		T number = target;
		int NumberOfRemoved = 0;
		int IndexOfNumberORemoved = -1;
		for (int i = 1; i < count; ++i)
		{
			if (target == current->getItem())
			{
				current = current->getNext();
				this->Delete(i);
				count--;
				i--;
				NumberOfRemoved++;
				wasconcurrent = true;
			}
			else
			{
				if (wasconcurrent)
				{
					wasconcurrent = false;
					this->Delete(targetindex);
					NumberOfRemoved++;
					number = target;
					this->Insert(number * NumberOfRemoved, targetindex);
				}
				targetindex = i;
				target = current->getItem();
				current = current->getNext();
				NumberOfRemoved = 0;
				IndexOfNumberORemoved = targetindex;
			}
			if (!current)
				break;
		}
		if (wasconcurrent)
		{
			wasconcurrent = false;
			this->Delete(targetindex);
			NumberOfRemoved++;
			this->Insert(number * NumberOfRemoved, targetindex);
		}
	}



};

