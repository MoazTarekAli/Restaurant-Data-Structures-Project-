#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double oMoney, int s) :Event(eTime, oID)
{
	OrdType = oType;
	size = s;
	OrdMoney = oMoney;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	
	Order* pOrd = new Order(OrderID, OrdType, size, OrdMoney, EventTime);

	if (OrdType < 2)
	{
		pRest->addtoQueue(pOrd);
	}
	else if (OrdType == 2)
	{
		int prio = 5;

		// place holder equation for priority until calculated (Read document explaination in next 3 lines)
		// prio = pOrd->getMoney() * pOrd->getArrTime() / pOrd->getSize();  SHOULD BE CHANGED !!!!!!!!!!!
		// You should develop a reasonable weighted priority equation depending on at least the
		// following factors : Order Arrival Time, Order Money, and Order Size.

		

		pRest->addtoQueue(pOrd, prio);
	}
	
	
	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2
	//Order* pOrd = new Order(OrderID,OrdType);
	//	pRest->AddtoDemoQueue(pOrd);
}
