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
	servedOrdersCount(0), orderBeingServed(nullptr), available(true) {}

int Cook::TimeToFinishOrder()
{
	if (available)
		return 0;
	double Time = static_cast<double> (orderBeingServed->GetSize()) / speed;
	return ceil(Time);
}

void Cook::SetOrder(Order o)
{
	if (!available)
		return;
	orderBeingServed = &o;
	available = false;
	o.SetStatus(SRV);
	servedOrdersCount++;
}

Order* Cook::GetOrder() const
{
	return orderBeingServed;
}

void Cook::SetAvailable(bool a)
{
	available = a;
}

bool Cook::GetAvailable() const
{
	return available;
}

bool Cook::NeedBreak()
{
	if (servedOrdersCount == 0) return false;
	if (servedOrdersCount % ordersBeforeBreak==0)
	{
		available = false;
		return true;
	}
	return false;
}

