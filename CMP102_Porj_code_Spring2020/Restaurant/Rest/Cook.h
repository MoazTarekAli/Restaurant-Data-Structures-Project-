#pragma once

#include "..\Defs.h"
#include "..\Rest\Order.h"


class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)

	//new data members:-
	int servedOrdersCount;
	int ordersBeforeBreak;
	int breakDuration;
	Order* orderBeingServed;
	bool IsCooking; 
	bool isResting;
	int breakTimeEnd;
public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void SetID(int);
	void SetType(ORD_TYPE) ;
	
	//new function:-

	Cook(int ID_, ORD_TYPE type_, int speed_, int breakDuration_, int ordersBeforeBreak_);
	int TimeToFinishOrder();
	void SetOrder(Order* o);
	Order* GetOrder() const;
	void SetIsCooking(bool a);
	bool GetIsCooking() const;
	bool NeedBreak();
	void SetIsResting(bool ir);
	bool GetIsResting();
	void setBreakTimeEnd(int time);
	int GetBreakTimeEnd();
};
