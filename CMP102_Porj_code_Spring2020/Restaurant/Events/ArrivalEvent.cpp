#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int ordID, ORD_TYPE ordType, double ordMoney, int s) :
	Event(eTime, ordID), OrdType(ordType), Size(s), OrdMoney(ordMoney), OrdDistance(0) {}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	
	Order* pOrd = new Order(OrderID, OrdType, Size, OrdMoney, EventTime);

	if (OrdType != TYPE_VIP)
	{
		pRest->AddToQueue(pOrd);
	}
	else if (OrdType == TYPE_VIP)
	{
		pRest->AddToQueue(pOrd, pRest->CalcPriority(pOrd));
	}
}
