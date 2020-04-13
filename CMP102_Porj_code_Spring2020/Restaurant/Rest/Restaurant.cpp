#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"


Restaurant::Restaurant() 
{
	numberOfCooks = 0;
	totalTimeSteps = 1;
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		SimpleSimulator();
		break;
	case MODE_STEP:
		break;
	case MODE_SLNT:
		break;
	//case MODE_DEMO:
		//Just_A_Demo();

	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}

//void Restaurant::addtoNormalQueue(Order* pOrd)
//{
//	normalOrderQueue.enqueue(pOrd);
//}
//
//void Restaurant::addtoVeganQueue(Order* pOrd)
//{
//	veganOrderQueue.enqueue(pOrd);
//}
//
//void Restaurant::addtoVIPQueue(Order* pOrd,int prio)
//{
//	vipOrderQueue.enqueue(pOrd,prio);
//}

void Restaurant::AddToQueue(Order* pOrd,const int prio)
{
	switch (pOrd->GetType())
	{
	case 0:
		normalOrderQueue.enqueue(pOrd);
		break;
	case 1:
		veganOrderQueue.enqueue(pOrd);
		break;
	case 2:
		vipOrderQueue.enqueue(pOrd, prio);
		break;
	default:
		break;
	}
}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);
	
	//adding cooks to gui
	for (int i = 0; i < numberOfCooks; i++)
	{

		pGUI->AddToDrawingList(cookList.getEntry(i));

	}
	
	//adding unfinished orders
	int normal_count = 0, vegan_count = 0, vip_count = 0;
	Order** Normal = normalOrderQueue.toArray(normal_count);
	Order** vegan = veganOrderQueue.toArray(vegan_count);
	Order** vip = vipOrderQueue.toArray(vip_count);
	int sum = normal_count + vegan_count + vip_count;
	Order** all_orders =new Order* [sum];
	for (int i = 0; i < sum; i++)
	{
		if (i < normal_count)
			all_orders[i] = Normal[i];
		else if (i < normal_count + vegan_count)
			all_orders[i] = vegan[i - normal_count];
		else if (i < normal_count + vegan_count + vip_count)
			all_orders[i] = vip[i - normal_count - vegan_count];
	}

	for (int i = 1; i < sum; i++)
	{
		Order* key = all_orders[i];
		int j = i - 1;
		while (j >= 0 && (all_orders[j]->GetArrTime() > key->GetArrTime()))
		{
			all_orders[j + 1] = all_orders[j];
			j = j - 1;
		}
		all_orders[j + 1] = key;
	}
	for (int i = 0; i < sum; i++)
	{
		pGUI->AddToDrawingList(all_orders[i]);
	}
	delete[] all_orders;
	//adding served orders
	int served_count = 0;
	Order** served = servedQueue.toArray(served_count);
	for (int i = 0; i < served_count; i++)
	{
		pGUI->AddToDrawingList(served[i]);
	}
	//adding finished orders
	int finished_count=0;
	Order** finished = finishedQueue.toArray(finished_count);
	for (int i = 0; i < finished_count; i++)
	{
		pGUI->AddToDrawingList(finished[i]);
	}
	//adding timesteps , number of available cooks , number of waiting orders
	pGUI->PrintMessage(
		"Current time step : " + to_string(totalTimeSteps) + '\n' +
		"Number of available normal cooks : " + to_string(numberOfNormalCooks) + '\n' +
		"Number of available vegan cooks : " + to_string(numberOfVeganCooks) + '\n' +
		"Number of available vip cooks : " + to_string(numberOfVipCooks) + '\n' +
		"Number of waiting normal orders : " + to_string(normal_count) + '\n' +
		"Number of waiting vegan orders : " + to_string(vegan_count) + '\n'+
		"Number of waiting vip orders : " + to_string(vip_count) + '\n' +
		"Number of orders in service : " + to_string(served_count) + '\n' +
		"Number of finished orders : " + to_string(finished_count) +'\n');

}

void Restaurant::CancelOrder(int ID)
{
	int count;
	int flag=-1;
	Order** NORMAL = normalOrderQueue.toArray(count);
	for (int i = 0; i < count; i++)
	{
		if (NORMAL[i]->GetID() == ID)
			flag = i;
	}
	for (int i = 0; i < count; i++)
	{
		Order* x;
		normalOrderQueue.dequeue(x);
	}
	for (int i = 0; i < count; i++)
	{
		if (i != flag)
			normalOrderQueue.enqueue(NORMAL[i]);
	}
}


void Restaurant::InteractiveMode()
{
	Event* pE;
	
}

void Restaurant::LoadRestaurant()
{
	pGUI->PrintMessage((string)"[The input file must be placed in the Input_Files folder]\n"+
		"[test0.txt, test1.txt, test3.txt, test4.txt files are already provided in the folder]\n"+
		"Enter the input file name:\n");
	string fileName = pGUI->GetString();
	fileName = "Input_Files\\" + fileName;
	LoadRestaurant(fileName);
}

void Restaurant::LoadRestaurant(string fileName)
{
	ifstream inFile(fileName, ios::in);
	if (!inFile.is_open())
	{
		pGUI->PrintMessage("Error! Can't open the file! The file must be placed inside the Input_Files folder.");
	}
	else
	{
		LoadRestaurant(inFile);
	}
}

void Restaurant::LoadRestaurant(ifstream& inFile)
{
	// loading values

	// initializing the variables to be used to store the data from the input file
	int normalCookCount, veganCookCount, vipCookCount;
	int normalCookSpeed, veganCookSpeed, vipCookSpeed;
	int ordersBeforeBreak, normalBreakDuration, veganBreakDuration, vipBreakDuration;
	int stepsBeforeAutoPromotion, numberOfEvents;
	// creating an array containing pointers to these variables in order to easily loop over them
	// to store the data from the input file
	int* inputValues[] =
	{
		&normalCookCount, &veganCookCount, &vipCookCount,
		&normalCookSpeed, &veganCookSpeed, &vipCookSpeed,
		&ordersBeforeBreak, &normalBreakDuration, &veganBreakDuration, &vipBreakDuration,
		&stepsBeforeAutoPromotion, &numberOfEvents
	};
	// looping over the variables and storing the data from the input file in them
	for (int i = 0; i < 12; ++i)
	{
		if (inFile.eof())
		{
			pGUI->PrintMessage("Error! Not enough values in the file!");
			pGUI->waitForClick();
			return;
		}
		inFile >> *inputValues[i];
	}
	
	// creating cooks

	// updating the number of cooks data member
	numberOfNormalCooks = normalCookCount;
	numberOfVeganCooks = veganCookCount;
	numberOfVipCooks = vipCookCount;
	numberOfCooks = normalCookCount + veganCookCount + vipCookCount;
	// creating arrays of different cook values in order to easily loop over them
	// to creat the cooks
	int cookCounts[] = { normalCookCount, veganCookCount, vipCookCount };
	int cookSpeeds[] = { normalCookSpeed, veganCookSpeed, vipCookSpeed };
	int breakDurations[] = { normalBreakDuration, veganBreakDuration, vipBreakDuration };
	ORD_TYPE ordTypes[] = { TYPE_NRM, TYPE_VGAN, TYPE_VIP };
	// initializing the cook ID to 0
	int currentID = 0;
	// creating the cooks
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < cookCounts[i]; ++j)
		{
			Cook* pCook = new Cook(++currentID, ordTypes[i], cookSpeeds[i], breakDurations[i], ordersBeforeBreak);
			cookList.Append(pCook);
		}
	}
	
	// creating events

	// repeats the steps for the given number of events
	for (int i = 0; i < numberOfEvents; ++i)
	{
		if (inFile.eof())
		{
			pGUI->PrintMessage("Error! Not enough values in the file!");
			pGUI->waitForClick();
			return;
		}
		// gets the event type for the current event
		char eventType;
		inFile >> eventType;
		// initializing the variables to be used and creating an areas of pointers to them
		int eventTimeStep, orderID, orderSize;
		int* eventInputValues[] = { &eventTimeStep, &orderID, &orderSize };
		Event* pEvent;
		// handling the different types of events
		switch (eventType)
		{
		// arrival event
		case 'R':
			if (inFile.eof())
			{
				pGUI->waitForClick();
				pGUI->PrintMessage("Error! Not enough values in the file!");
				return;
			}
			// getting the order type
			char orderTypeInput;
			inFile >> orderTypeInput;
			ORD_TYPE orderType;
			switch (orderTypeInput)
			{
			case 'N':
				orderType = TYPE_NRM;
				break;
			case 'G':
				orderType = TYPE_VGAN;
				break;
			case 'V':
				orderType = TYPE_VIP;
				break;
			default:
				pGUI->PrintMessage("Error! Unknown event type!");
				pGUI->waitForClick();
				return;
			}
			// getting the order inputs
			for (int j = 0; j < 3; ++j)
			{
				if (inFile.eof())
				{
					pGUI->PrintMessage("Error! Not enough values in the file!");
					pGUI->waitForClick();
					return;
				}
				inFile >> *eventInputValues[j];
			}
			if (inFile.eof())
			{
				pGUI->PrintMessage("Error! Not enough values in the file!");
				pGUI->waitForClick();
				return;
			}
			double orderMoney;
			inFile >> orderMoney;
			// creating the event
			pEvent = new ArrivalEvent(eventTimeStep, orderID, orderType, orderMoney, orderSize);
			EventsQueue.enqueue(pEvent);
			break;
		// cancellation event
		case 'X':
			for (int j = 0; j < 2; ++j)
			{
				if (inFile.eof())
				{
					pGUI->PrintMessage("Error! Not enough values in the file!");
					pGUI->waitForClick();
					return;
				}
				inFile >> *eventInputValues[j];
			}
			pEvent = new CancellationEvent(eventTimeStep, orderID);
			EventsQueue.enqueue(pEvent);
			break;
		// promotion event
		case 'P':
			pGUI->PrintMessage("Promotion events not implemented yet, please try again in phase 2!");
			break;
		default:
			pGUI->waitForClick();
			pGUI->PrintMessage("Error! Unknown event type!");
			return;
		}
	}
}

void Restaurant::SimpleSimulator()
{

	//place of loading calling
	LoadRestaurant();
	while (!(EventsQueue.isEmpty() && normalOrderQueue.isEmpty() && veganOrderQueue.isEmpty() && vipOrderQueue.isEmpty() && servedQueue.isEmpty()))
	{
		ExecuteEvents(totalTimeSteps);
		pGUI->PrintMessage(to_string(totalTimeSteps));
		Order* normal,* vegan,* vip;
		if (normalOrderQueue.peekFront(normal) && numberOfNormalCooks)
		{
			normalOrderQueue.dequeue(normal);
			normal->SetStatus(SRV);
			normal->SetServTime(totalTimeSteps);
			servedQueue.enqueue(normal);
			numberOfNormalCooks--;
		}
		if (veganOrderQueue.peekFront(vegan) && numberOfVeganCooks)
		{
			veganOrderQueue.dequeue(vegan);
			vegan->SetStatus(SRV);
			vegan->SetServTime(totalTimeSteps);
			servedQueue.enqueue(vegan);
			numberOfVeganCooks--;
		}
		if (vipOrderQueue.peekFront(vip) && numberOfVipCooks)
		{
			vipOrderQueue.dequeue(vip);
			vip->SetStatus(SRV);
			vip->SetServTime(totalTimeSteps);
			servedQueue.enqueue(vip);
			numberOfVipCooks--;
		}

		if (totalTimeSteps % 5 == 0)
		{
			Order* finished;
			for (int i = 0; i < 3; i++)
			{
				if (servedQueue.peekFront(finished))
				{
					servedQueue.dequeue(finished);
					finished->SetStatus(DONE);
					finished->SetFinishTime(totalTimeSteps);
					finishedQueue.enqueue(finished);
					switch (finished->GetType())
					{
					case(TYPE_NRM):
						numberOfNormalCooks++;
						break;
					case(TYPE_VGAN):
						numberOfVeganCooks++;
						break;
					case(TYPE_VIP):
						numberOfVipCooks++;
						break;
					}
				}
			}
		}
		FillDrawingList();
		pGUI->UpdateInterface();

		pGUI->waitForClick();
		pGUI->PrintMessage("");
		pGUI->ResetDrawingList();

		totalTimeSteps++;
	}
	pGUI->waitForClick();
}
/*
//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
void Restaurant::Just_A_Demo()
{
	
	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	
	int EventCnt;	
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();
		
	//Just for sake of demo, generate some cooks and add them to the drawing list
	//In next phases, Cooks info should be loaded from input file
	int C_count = 12;	
	Cook *pC = new Cook[C_count];
	int cID = 1;

	for(int i=0; i<C_count; i++)
	{
		cID+= (rand()%15+1);	
		pC[i].SetID(cID);
		pC[i].SetType((ORD_TYPE)(rand()%TYPE_CNT));
	}	

		
	int EvTime = 0;

	int O_id = 1;
	
	//Create Random events and fill them into EventsQueue
	//All generated event will be "ArrivalEvents" for the demo
	for(int i=0; i<EventCnt; i++)
	{
		O_id += (rand()%4+1);		
		int OType = rand()%TYPE_CNT;	//Randomize order type		
		EvTime += (rand()%5+1);			//Randomize event time
		pEv = new ArrivalEvent(EvTime,O_id,(ORD_TYPE)OType);
		EventsQueue.enqueue(pEv);

	}	

	// --->   In next phases, no random generation is done
	// --->       EventsQueue should be filled from actual events loaded from input file

	
	
	
	
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep,timestep,10);	
		pGUI->PrintMessage(timestep);


		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		

/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks
		
		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		for(int i=0; i<C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);
		
		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);
		
		for(int i=0; i<size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete []pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

	
}
////////////////

void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////

*/
