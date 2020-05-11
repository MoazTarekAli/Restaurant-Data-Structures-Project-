#include "Cook.h"
#include <math.h> 

Cook::Cook()
{
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}


void Cook::SetID(int id)
{
	ID = id;
}

void Cook::SetType(ORD_TYPE t)
{
	type = t;
}



//new functions:-

Cook::Cook(int ID_, ORD_TYPE type_, int speed_, int breakDuration_, int ordersBeforeBreak_) :
	ID(ID_), type(type_), speed(speed_), breakDuration(breakDuration_), ordersBeforeBreak(ordersBeforeBreak_),
	servedOrdersCount(0), orderBeingServed(nullptr), IsCooking(true), isResting(false), breakTimeEnd(0) {}

int Cook::TimeToFinishOrder()
{
	if (IsCooking)
		return 0;
	double Time = static_cast<double> (orderBeingServed->GetSize()) / speed;
	return ceil(Time);
}

void Cook::SetOrder(Order* o)
{
	if (!IsCooking)
		return;
	orderBeingServed = o;
	IsCooking = false;
	o->SetStatus(SRV);
	servedOrdersCount++;
}

Order* Cook::GetOrder() const
{
	return orderBeingServed;
}

void Cook::SetIsCooking(bool a)
{
	IsCooking = a;
}

bool Cook::GetIsCooking() const
{
	return IsCooking;
}

bool Cook::NeedBreak()
{
	if (servedOrdersCount == 0) return false;
	if (servedOrdersCount % ordersBeforeBreak==0)
	{
		IsCooking = false;
		return true;
	}
	return false;
}

bool Cook::GetIsResting()
{
	return isResting;
}

void Cook::SetIsResting(bool is)
{
	isResting = is;
}

int Cook::GetBreakTimeEnd()
{
	return breakTimeEnd;
}

void Cook::setBreakTimeEnd(int time)
{
	breakTimeEnd = time;
}