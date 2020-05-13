#pragma once
#include"Event.h"
class PromotionEvent :public Event
{
	int autopromotiontime; //get it from loaded file
	int extra_money_for_promotion;
	int autopromotedorders;
	int currentimestep;
public:
	PromotionEvent(int apt, int emp,int apo,int cts,int etime,int ordID);
	virtual void Execute(Restaurant*pRest);
};