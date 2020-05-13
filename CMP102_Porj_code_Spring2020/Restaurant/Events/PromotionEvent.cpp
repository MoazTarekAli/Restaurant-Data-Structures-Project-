#include"PromotionEvent.h"
#include"..\Rest\Restaurant.h"
PromotionEvent::PromotionEvent(int apt, int emp,int apo,int cts,int etime,int ordID):Event(etime,ordID)
{
	autopromotiontime = apt;
	extra_money_for_promotion = emp;
	autopromotedorders = apo;
	currentimestep = cts;
}
void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->Promote(currentimestep,extra_money_for_promotion);
}