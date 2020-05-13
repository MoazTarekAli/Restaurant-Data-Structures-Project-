#pragma once
#include"Event.h"
class PromotionEvent :public Event
{

public:

	PromotionEvent(int eTime,int ordID);
	virtual void Execute(Restaurant*pRest);
};