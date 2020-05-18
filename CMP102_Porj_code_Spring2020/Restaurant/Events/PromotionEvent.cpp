#include"PromotionEvent.h"
#include"..\Rest\Restaurant.h"

PromotionEvent::PromotionEvent(int eTime, int ordID, double pMoney) : Event(eTime, ordID), PromotionMoney(pMoney) {}

void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->PromoteOrder(OrderID, PromotionMoney);
}