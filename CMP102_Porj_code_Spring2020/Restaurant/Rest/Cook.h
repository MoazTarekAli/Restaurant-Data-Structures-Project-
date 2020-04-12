#pragma once

#include "..\Defs.h"
#include "..\Rest\Order.h"


class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)

	//new data members:-
	int numberOfServedOrders;
	int maxOrders;
	int breakTime;
	Order* orderBeingServed;
	bool available; 

public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void setID(int);
	void setType(ORD_TYPE) ;
	
	//new function:-

	Cook(int _ID, ORD_TYPE _type, int _speed, int _breakTime, int _maxOrders);
	int timeToFinishOrder();
	void setOrder(Order o);
	Order* getOrder() const;
	void setAvailable(bool a);
	bool getAvailable() const;
	bool needBreak();
};
