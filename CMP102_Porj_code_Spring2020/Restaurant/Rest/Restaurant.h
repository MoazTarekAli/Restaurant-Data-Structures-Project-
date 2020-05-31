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

#include <fstream>
#include <string>

using namespace std;

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*>* EventsQueue;	//Queue of all events that will be loaded from file

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	//Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	//
	// TODO: Add More Data Members As Needed
	//

	Queue<Order*>* normalOrderQueue;
	Queue<Order*>* veganOrderQueue;
	PriorityQueue<Order*>* vipOrderQueue;
	PriorityQueue<Order*>* urgentOrderQueue;
	PriorityQueue<Order*>* InServiceQueue;
	Queue<Order*>* finishedQueue;
	
	Queue<Cook*>* normalCooks;
	Queue<Cook*>* veganCooks;
	Queue<Cook*>* vipCooks;
	PriorityQueue<Cook*>* assignedCooks;
	PriorityQueue<Cook*>* restingCooks;
	
	int totalCookCount;
	int normalCookCount;
	int	veganCookCount;
	int vipCookCount;
	int injuredCount;

	int totalOrdersCount;
	int normalOrdersCount;
	int veganOrdersCount;
	int vipOrdersCount;
	int urgentOrdersCount;
	
	int totalMoney;
	int currentTimeSteps;
	
	int autoPromotedCount;
	int autoPromotionSteps;
	double injuryProbability;
	int urgentSteps;
	int restSteps;

	// private loading functions

	bool CheckEOF(ifstream& inFile);
	template <typename T>
	bool LoadValues(ifstream& inFile, int itemCount, T* items[]);
	void LoadCooks(int ordersBeforeBreak, int* cookCounts, int cookSpeeds[3][2], int CookBreaks[3][2]);
	bool LoadEvents(ifstream& inFile);
	bool LoadArrivalEvent(ifstream& inFile);
	bool LoadCancellationEvent(ifstream& inFile);
	bool LoadPromotionEvent(ifstream& inFile);

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents();	// Executes all events at current timestep
	void RunSimulation();	// Promopts the user to pick one of the available three modes
	bool InteractiveMode();	// First available mode, moves timesteps by mouseclicks
	bool StepByStepMode();	// Second available mode, moves a single timestep every second
	bool SilentMode();		// Third available mode, finishes the runtime on it's own

	void FillDrawingList();

	bool AssignOrder(int currentTimeStep, Order* order, Queue<Cook*>* cookList);
	void AssignToCook();

	void AssignToCook2();
	void AssignUrgent();
	void AssignVIP();
	void AssignNormal();
	void AssignVegan();

	//
	// TODO: Add More Member Functions As Needed
	//

	void AddToQueue(Order* pO, const int prio=0);
	void CancelOrder(int ID);
	void PromoteOrder(int ID, double promotionMoney);

	//void AssignToCook();

	void CheckFinishedOrders();
	void CheckCooksBreaks();
	void Injury();
	void UpdateUrgentOrders();
	void AutoPromote();
	void UpdateWaitingTime();
	int CalcPriority(Order* O);
	

	// public load functions

	bool LoadRestaurant(); // to load using input from user
	bool LoadRestaurant(ifstream& inFile); // to load using a file directly
	
	void SaveRestaurant(); // to save the restaurants data



};

#endif