#pragma once

#include "..\Defs.h"
#include "..\Rest\Order.h"


class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)

	//new data members:-

	int ServedOrdersCount;
	int OrdersBeforeBreak;
	int BreakDuration;
	Order* OrderBeingServed;
	bool IsCooking; 
	bool IsResting;
	bool IsInjured;
	int BreakTimeEnd;

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
	void SetCookingSpeed(int spd);
	int	GetCookingSpeed() const;
	void SetIsCooking(bool a);
	bool GetIsCooking() const;
	void SetIsResting(bool ir);
	bool GetIsResting()	const;
	void SetIsInjured(bool inj);
	bool GetIsInjured()	const;
	bool NeedBreak();
	void SetBreakTimeEnd(int time);
	int GetBreakTimeEnd() const;
	void SetBreakDuration(int bd);
	int GetBreakDuration();
};
