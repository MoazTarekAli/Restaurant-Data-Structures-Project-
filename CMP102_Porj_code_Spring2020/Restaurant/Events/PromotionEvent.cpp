#include"PromotionEvent.h"
#include"..\Rest\Restaurant.h"

PromotionEvent::PromotionEvent(int eTime,int ordID):Event(eTime,ordID)
{
}
void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->PromoteOrder(OrderID);
}