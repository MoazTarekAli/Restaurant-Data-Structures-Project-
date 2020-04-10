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

Cook::Cook(int i, ORD_TYPE t, int s , int b)
{
	ID = i;
	type = t;
	speed = s;
	Break_time = b;
	Number_of_served_orders = 0;
	Order_being_served = nullptr;
	Available = true;
}

int Cook::Time_taken_to_finish_current_order()
{
	if (Available)
		return 0;
	double Time = static_cast<double> (Order_being_served->GetSize()) / speed;
	return ceil(Time);
}

void Cook::SetOrder(Order o)
{
	if (!Available)
		return;
	Order_being_served = &o;
	Available = false;
	Number_of_served_orders++;
}

Order* Cook::GetOrder()
{
	return Order_being_served;
}

void Cook::SetAvailable(bool a)
{
	Available = a;
}

bool Cook::GetAvailable()
{
	return Available;
}

