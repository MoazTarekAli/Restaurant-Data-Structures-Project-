#ifndef __CANCELLATION_EVENT_H
#define __CANCELLATION_EVENT_H

#include "Event.h"

class CancellationEvent : public Event
{
	public:
		CancellationEvent(int eTime, int ordID);
		virtual void Execute(Restaurant *pRest);
};

#endif
