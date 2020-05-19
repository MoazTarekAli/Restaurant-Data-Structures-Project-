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
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	//Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	//
	// TODO: Add More Data Members As Needed
	//

	Queue<Order*> normalOrderQueue;
	Queue<Order*> veganOrderQueue;
	PriorityQueue<Order*> vipOrderQueue;
	PriorityQueue<Order*> urgentOrderQueue;
	Queue<Order*> finishedQueue;
	Queue<Order*> InServiceQueue;
	PriorityQueue<Order*> InServiceQueue_test;
	
	LinkedList<Cook*> availableCooks;
	Stack<Cook*> normalCooks;
	Stack<Cook*> veganCooks;
	Stack<Cook*> vipCooks;
	
	PriorityQueue<Cook*> assignedCooks;
	PriorityQueue<Cook*> restingCooks;
	
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
	int injuryProbability;
	int urgentSteps;
	int restSteps;

	bool AssignOrder(int currentTimeStep, Order* order, Stack<Cook*>& cookList);

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
	
	void ExecuteEvents();	//executes all events at current timestep
	void RunSimulation();
	void InteractiveMode();
	void StepByStepMode();
	void SilentMode();
	void SimpleSimulator();

	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//

	//void addtoNormalQueue(Order* pO);
	//void addtoVeganQueue(Order* pO);
	//void addtoVIPQueue(Order* pO, int prio);

	void AddToQueue(Order* pO, const int prio=0);
	void CancelOrder(int ID);
	void PromoteOrder(int ID, double promotionMoney);
	void AssignToCook();
	void check_finished_orders();
	void check_cooks_breaks();
	void Injury();
	void UpdateUrgentOrders();
	void AutoPromote();
	int CalcPriority(Order* O);

	
	// public load functions
	void LoadRestaurant(); // to load using input from user
	void LoadRestaurant(string fileName); // to load using a file name
	void LoadRestaurant(ifstream& inFile); // to load using a file directly
	
	void SaveRestaurant(); // to save the restaurants data

/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	//void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	//void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

/// ================================================================================================== 



};

#endif