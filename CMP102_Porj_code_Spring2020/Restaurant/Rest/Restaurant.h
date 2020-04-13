#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\Stack.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Generic_DS\LinkedList.h"
#include "..\Events\Event.h"


#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	//Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	//
	// TODO: Add More Data Members As Needed
	//

	Queue<Order*> NORMAL_Queue;
	Queue<Order*> VEGAN_Queue;
	PriorityQueue<Order*> VIP_Queue;
	LinkedList<Cook*> COOK_LIST;
	Queue<Order*> finished_Queue;
	Queue<Order*> served_Queue;
	int ncooks;
	int TimeSteps;
public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void InteractiveMode();
	


	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//

	//void addtoNormalQueue(Order* pO);
	//void addtoVeganQueue(Order* pO);
	//void addtoVIPQueue(Order* pO, int prio);

	void addtoQueue(Order* pO, const int prio=0);
	void cancel(int ID);
	void EventPerformer(int);
	void simpleSimulator();
	

/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	//void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	//void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

/// ================================================================================================== 



};

#endif