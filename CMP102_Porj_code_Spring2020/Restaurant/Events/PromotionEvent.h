#pragma once
#include"Event.h"
class PromotionEvent :public Event
{
	double PromotionMoney;
public:
	PromotionEvent(int eTime, int ordID, double pMoney);
	virtual void Execute(Restaurant*pRest);
};