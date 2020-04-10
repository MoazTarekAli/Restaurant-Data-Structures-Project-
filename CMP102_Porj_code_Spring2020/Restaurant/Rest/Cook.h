#pragma once

#include "..\Defs.h"
#include "..\Rest\Order.h"

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)

	//new data members:-
	int Number_of_served_orders;
	int Break_time;
	Order* Order_being_served;
	bool Available; 

public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void setID(int);
	void setType(ORD_TYPE) ;
	
	//new function:-

	Cook(int,ORD_TYPE,int,int);
	int Time_taken_to_finish_current_order();
	void SetOrder(Order);
	Order* GetOrder();
	void SetAvailable(bool);
	bool GetAvailable();

};
