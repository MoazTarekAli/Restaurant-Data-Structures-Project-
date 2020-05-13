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
	ID(ID_), type(type_), speed(speed_), BreakDuration(breakDuration_), OrdersBeforeBreak(ordersBeforeBreak_),
	ServedOrdersCount(0), OrderBeingServed(nullptr), IsCooking(false), IsResting(false), BreakTimeEnd(0)
	, IsInjured(false) {}

int Cook::TimeToFinishOrder()
{
	if (!IsCooking)
		return 0;
	double Time = static_cast<double> (OrderBeingServed->GetSize()) / speed;
	return ceil(Time);
}


void Cook::SetOrder(Order* o)
{
	OrderBeingServed = o;
	IsCooking = true;
	o->SetStatus(SRV);
	ServedOrdersCount++;
}

Order* Cook::GetOrder() const
{
	return OrderBeingServed;
}

void Cook::SetCookingSpeed(int spd)
{
	speed = spd;
}

int Cook::GetCookingSpeed() const
{
	return speed;
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
	if (ServedOrdersCount == 0) return false;
	if (ServedOrdersCount % OrdersBeforeBreak==0)
	{
		IsCooking = false;
		IsResting = true;
		return true;
	}
	return false;
}

bool Cook::GetIsResting()	const
{
	return IsResting;
}

void Cook::SetIsResting(bool is)
{
	IsResting = is;
}

void Cook::SetIsInjured(bool injury)
{
	IsInjured = injury;
}

bool Cook::GetIsInjured()	const
{
	return IsInjured;
}

int Cook::GetBreakTimeEnd()	const
{
	return BreakTimeEnd;
}

void Cook::SetBreakTimeEnd(int time)
{
	BreakTimeEnd = time;
}