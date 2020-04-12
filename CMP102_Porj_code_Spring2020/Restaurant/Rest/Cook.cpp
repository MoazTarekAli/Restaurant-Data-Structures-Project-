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


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}



//new functions:-

Cook::Cook(int ID_, ORD_TYPE type_, int speed_, int breakTime_, int maxOrders_) :
	ID(ID_), type(type_), speed(speed_), breakTime(breakTime_), maxOrders(maxOrders_),
	numberOfServedOrders(0), orderBeingServed(nullptr), available(true) {}

int Cook::timeToFinishOrder()
{
	if (available)
		return 0;
	double Time = static_cast<double> (orderBeingServed->getSize()) / speed;
	return ceil(Time);
}

void Cook::setOrder(Order o)
{
	if (!available)
		return;
	orderBeingServed = &o;
	available = false;
	o.setStatus(SRV);
	numberOfServedOrders++;
}

Order* Cook::getOrder() const
{
	return orderBeingServed;
}

void Cook::setAvailable(bool a)
{
	available = a;
}

bool Cook::getAvailable() const
{
	return available;
}

bool Cook::needBreak()
{
	if (numberOfServedOrders == 0) return false;
	if (numberOfServedOrders % maxOrders==0)
	{
		available = false;
		return true;
	}
	return false;
}

