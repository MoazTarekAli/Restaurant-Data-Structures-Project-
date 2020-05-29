#include <cstdlib>
#include <time.h>
#include <iostream>

using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"

Restaurant::Restaurant() 
{
	currentTimeSteps = 1;
	totalMoney = 0;
	injuredCount = 0;
	pGUI = NULL;
	
	totalCookCount = 0;
	normalCookCount = 0;
	veganCookCount = 0;
	vipCookCount = 0;
	injuredCount = 0;

	totalOrdersCount = 0;
	normalOrdersCount = 0;
	veganOrdersCount = 0;
	vipOrdersCount = 0;
	urgentOrdersCount = 0;

	autoPromotedCount = 0;


	EventsQueue = new Queue<Event*>;

	normalOrderQueue = new Queue<Order*>;
	veganOrderQueue = new Queue<Order*>;
	vipOrderQueue = new PriorityQueue<Order*>;
	urgentOrderQueue = new PriorityQueue<Order*>;
	finishedQueue = new Queue<Order*>;
	InServiceQueue = new PriorityQueue<Order*>;

	availableCooks = new LinkedList<Cook*>;
	normalCooks = new Queue<Cook*>;
	veganCooks = new Queue<Cook*>;
	vipCooks = new Queue<Cook*>;
	assignedCooks = new PriorityQueue<Cook*>;
	restingCooks = new PriorityQueue<Cook*>;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();
	
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		//	SimpleSimulator();
		if (InteractiveMode()) return;
		break;
	case MODE_STEP:
		if (StepByStepMode()) return;
		break;
	case MODE_SLNT:
		if (SilentMode()) return;
		break;
	//case MODE_DEMO:
		//Just_A_Demo();

	};
	SaveRestaurant();
}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents()
{
	Event *pE;
	while( EventsQueue->peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > currentTimeSteps)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue->dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}

void Restaurant::AddToQueue(Order* pOrd,const int prio)
{
	switch (pOrd->GetType())
	{
	case 0:
		normalOrderQueue->enqueue(pOrd);
		break;
	case 1:
		veganOrderQueue->enqueue(pOrd);
		break;
	case 2:
		vipOrderQueue->enqueue(pOrd, prio);
		break;
	default:
		break;
	}
}

Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;

	delete EventsQueue;

	delete normalOrderQueue;
	delete veganOrderQueue;
	delete vipOrderQueue;
	delete urgentOrderQueue;
	delete finishedQueue;
	delete InServiceQueue;

	delete availableCooks;
	delete normalCooks;
	delete veganCooks;
	delete vipCooks;
	delete assignedCooks;
	delete restingCooks;
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

	//adding cooks to gui

	
	for (int i = 0; i < veganCookCount + vipCookCount + normalCookCount; ++i)
	{
		Cook* pcook;
		if (i < vipCookCount)
		{
			vipCooks->dequeue(pcook);
			pGUI->AddToDrawingList(pcook);
			vipCooks->enqueue(pcook);
		}
		else if (i < vipCookCount + normalCookCount)
		{
			normalCooks->dequeue(pcook);
			pGUI->AddToDrawingList(pcook);
			normalCooks->enqueue(pcook);
		}
		else
		{
			veganCooks->dequeue(pcook);
			pGUI->AddToDrawingList(pcook);
			veganCooks->enqueue(pcook);
		}
	}

	//adding waiting orders
	int normal_count = 0, vegan_count = 0, vip_count = 0, urgent_count = 0;
	Order** Normal = normalOrderQueue->toArray(normal_count);
	Order** vegan = veganOrderQueue->toArray(vegan_count);
	Order** vip = vipOrderQueue->toArray(vip_count);
	Order** urgent = urgentOrderQueue->toArray(urgent_count);
	int sum = normal_count + vegan_count + vip_count + urgent_count;
	Order** all_orders = new Order * [sum];
	for (int i = 0; i < sum; i++)
	{
		if (i < normal_count)
			all_orders[i] = Normal[i];
		else if (i < normal_count + vegan_count)
			all_orders[i] = vegan[i - normal_count];
		else if (i < normal_count + vegan_count + vip_count)
			all_orders[i] = vip[i - normal_count - vegan_count];
		else if (i < normal_count + vegan_count + vip_count + urgent_count)
			all_orders[i] = urgent[i - normal_count - vegan_count - vip_count];
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
	Order** served = InServiceQueue->toArray(served_count);
	for (int i = 0; i < served_count; i++)
	{
		pGUI->AddToDrawingList(served[i]);
	}

	//adding last assigned orders
	int cook_count = 0;
	Cook**last_cooks = assignedCooks->toArray(cook_count);
	string assigned_orders = "";
	for (int i = 0; i < cook_count; i++)
	{
		if ((last_cooks[i]->GetOrder())->GetServTime() == currentTimeSteps - 1)
		{
			switch (last_cooks[i]->GetType())
			{
			case TYPE_NRM:
				assigned_orders += "N";
				break;
			case TYPE_VGAN:
				assigned_orders += "VG";
				break;
			case TYPE_VIP:
				assigned_orders += "V";
				break;
			}

			assigned_orders += (to_string(last_cooks[i]->GetID()) + "(");

			switch ((last_cooks[i]->GetOrder())->GetType())
			{
			case TYPE_NRM:
				assigned_orders += "N";
				break;
			case TYPE_VGAN:
				assigned_orders += "VG";
				break;
			case TYPE_VIP:
				assigned_orders += "V";
				break;
			case TYPE_URGNT:
				assigned_orders += "V";
				break;
			}

			assigned_orders += to_string((last_cooks[i]->GetOrder())->GetID());
			assigned_orders += ") ";
		}
	}

	//adding finished orders
	int finished_count = 0;
	Order** finished = finishedQueue->toArray(finished_count);
	for (int i = 0; i < finished_count; i++)
	{
		pGUI->AddToDrawingList(finished[i]);
	}
	//adding timesteps , number of available cooks , number of waiting orders
	pGUI->PrintMessage(
		"Current time step : " + to_string(currentTimeSteps) + '\n' +
		"Number of available normal cooks : " + to_string(normalCookCount) + '\n' +
		"Number of available vegan cooks : " + to_string(veganCookCount) + '\n' +
		"Number of available vip cooks : " + to_string(vipCookCount) + '\n' +
		"Number of injured cooks :" + to_string(injuredCount) + '\n' +
		"Number of waiting normal orders : " + to_string(normal_count) + '\n' +
		"Number of waiting vegan orders : " + to_string(vegan_count) + '\n' +
		"Number of waiting vip orders : " + to_string(vip_count) + '\n' +
		"Number of waiting urgent orders :" + to_string(urgent_count) + '\n' +
		"Number of orders in service : " + to_string(served_count) + '\n' +
		"Orders served in the last timestep : " + assigned_orders + '\n' +
		"Number of finished orders : " + to_string(finished_count) + '\n');
	pGUI->UpdateInterface();
	pGUI->ResetDrawingList();
}

void Restaurant::CancelOrder(int ID)
{
	int count;
	int flag=-1;
	Order** NORMAL = normalOrderQueue->toArray(count);
	for (int i = 0; i < count; i++)
	{
		if (NORMAL[i]->GetID() == ID)
			flag = i;
	}
	for (int i = 0; i < count; i++)
	{
		Order* x;
		normalOrderQueue->dequeue(x);
	}
	for (int i = 0; i < count; i++)
	{
		if (i != flag)
			normalOrderQueue->enqueue(NORMAL[i]);
	}
}


bool Restaurant::InteractiveMode()
{
	if (LoadRestaurant()) return true;
	currentTimeSteps = 0;

	while (!(EventsQueue->isEmpty() && normalOrderQueue->isEmpty()
		&& veganOrderQueue->isEmpty() && vipOrderQueue->isEmpty()
		&& InServiceQueue->isEmpty()&&restingCooks->isEmpty()))
	{
		currentTimeSteps++;
		UpdateWaitingTime();
		ExecuteEvents();
		Injury();
		AutoPromote();
		UpdateUrgentOrders();
		CheckFinishedOrders();
		CheckCooksBreaks();
		AssignToCook2();
		FillDrawingList();
		pGUI->waitForClick();
	}
	return false;
}

bool Restaurant::StepByStepMode()
{
	if (LoadRestaurant()) return true;
	currentTimeSteps = 0;

	while (!(EventsQueue->isEmpty() && normalOrderQueue->isEmpty()
		&& veganOrderQueue->isEmpty() && vipOrderQueue->isEmpty()
		&& InServiceQueue->isEmpty() && restingCooks->isEmpty()))
	{
		currentTimeSteps++;
		UpdateWaitingTime();
		ExecuteEvents();
		Injury();
		AutoPromote();
		UpdateUrgentOrders();
		CheckFinishedOrders();
		CheckCooksBreaks();
		AssignToCook();
		FillDrawingList();
		Sleep(1000);
	}
	return false;
}

bool Restaurant::SilentMode()
{
	if (LoadRestaurant()) return true;
	currentTimeSteps = 0;

	while (!(EventsQueue->isEmpty() && normalOrderQueue->isEmpty()
		&& veganOrderQueue->isEmpty() && vipOrderQueue->isEmpty()
		&& InServiceQueue->isEmpty() && restingCooks->isEmpty()))
	{
		currentTimeSteps++;
		UpdateWaitingTime();
		ExecuteEvents();
		Injury();
		AutoPromote();
		UpdateUrgentOrders();
		CheckFinishedOrders();
		CheckCooksBreaks();
		AssignToCook();
	}
	return false;
}

inline bool Restaurant::CheckEOF(ifstream& inFile)
{
	if (inFile.eof())
	{
		pGUI->PrintMessage("Error! Not enough values in the file!");
		pGUI->waitForClick();
		return true;
	}
	return false;
}

template <typename T>
bool Restaurant::LoadValues(ifstream& inFile, int itemCount, T* items[])
{
	for (int i = 0; i < itemCount; ++i)
	{
		if (CheckEOF(inFile)) return true;
		inFile >> *items[i];
	}
	return false;
}

bool Restaurant::LoadRestaurant()
{
	pGUI->PrintMessage((string)"[The input file must be placed in the Input_Files folder]\n" +
		"[test0.txt, test1.txt, test3.txt, test4.txt files are already provided in the folder]\n" +
		"Enter the input file name:\n");
	string fileName = pGUI->GetString();
	fileName = "Input_Files\\" + fileName;
	return (LoadRestaurant(fileName));
}

bool Restaurant::LoadRestaurant(string fileName)
{
	ifstream inFile(fileName+".txt", ios::in);
	if (!inFile.is_open())
	{
		pGUI->PrintMessage("Error! Can't open the file! The file must be placed inside the Input_Files folder.");
		pGUI->waitForClick();
		return true;
	}
	return (LoadRestaurant(inFile));
}

bool Restaurant::LoadRestaurant(ifstream& inFile)
{
	// loading values

	// declaring the variables to be used to store the data from the input file
	int normalCookCountInput, veganCookCountInput, vipCookCountInput;
	int minNormalSpeed, maxNormalSpeed, minVeganSpeed, maxVeganSpeed, minVipSpeed, maxVipSpeed;
	int ordersBeforeBreak, minNormalBreak, maxNormalBreak, minVeganBreak, maxVeganBreak, minVipBreak, maxVipBreak;
	double injuryProbability;
	int restStepsInput, autoPromotionStepsInput, urgentStepsInput;

	// creating an array containing pointers to these variables in order to easily loop over them
	// to store the data from the input file
	int* inputValuesA[] =
	{
		&normalCookCountInput, &veganCookCountInput, &vipCookCountInput,
		&minNormalSpeed, &maxNormalSpeed, &minVeganSpeed, &maxVeganSpeed, &minVipSpeed, &maxVipSpeed,
		&ordersBeforeBreak, &minNormalBreak, &maxNormalBreak, &minVeganBreak, &maxVeganBreak, &minVipBreak, &maxVipBreak
	};
	double* inputValuesB[] = { &injuryProbability };
	int* inputValuesC[] = { &restStepsInput, &autoPromotionStepsInput, &urgentStepsInput };

	// getting the input values
	if (LoadValues<int>(inFile, 16, inputValuesA)) return true;
	if (LoadValues<double>(inFile, 1, inputValuesB)) return true;
	if (LoadValues<int>(inFile, 3, inputValuesC)) return true;

	// initializing data members
	normalCookCount = normalCookCountInput;
	veganCookCount = veganCookCountInput;
	vipCookCount = vipCookCountInput;
	totalCookCount = normalCookCountInput + veganCookCountInput + vipCookCountInput;
	autoPromotionSteps = autoPromotionStepsInput;
	urgentSteps = urgentStepsInput;
	restSteps = restStepsInput;

	// creating cooks
	int cookCounts[] = { normalCookCount, veganCookCount, vipCookCount };
	int cookSpeeds[3][2] = { {minNormalSpeed, maxNormalSpeed}, {minVeganSpeed, maxVeganSpeed}, {minVipSpeed, maxVipSpeed} };
	int cookBreaks[3][2] = { {minNormalBreak, maxNormalBreak}, {minVeganBreak, maxVeganBreak}, {minVipBreak, maxVipBreak} };
	LoadCooks(ordersBeforeBreak, cookCounts, cookSpeeds, cookBreaks);

	// creating events
	if (LoadEvents(inFile)) return true;

	// closing the file
	inFile.close();

	return false;
}

void Restaurant::LoadCooks(int ordersBeforeBreak, int* cookCounts, int cookSpeeds[3][2], int cookBreaks[3][2])
{
	// creating the cooks

	Queue<Cook*>** cookStacks[] = { &normalCooks, &veganCooks, &vipCooks };
	ORD_TYPE ordTypes[] = { TYPE_NRM, TYPE_VGAN, TYPE_VIP };

	// initializing the cook ID to 0
	int currentID = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < cookCounts[i]; ++j)
		{
			// srand to generate a random seed
			srand(time(NULL));
			// generating the random cook speed and break time
			int cookSpeed = rand() % (cookSpeeds[i][1] - cookSpeeds[i][0] + 1) + cookSpeeds[i][0];
			int cookBreak = rand() % (cookBreaks[i][1] - cookBreaks[i][0] + 1) + cookBreaks[i][0];
			// creating the cook
			Cook* pCook = new Cook(++currentID, ordTypes[i], cookSpeed, cookBreak, ordersBeforeBreak);
			// adding the cook to the appropriate lists
			availableCooks->Append(pCook);
			(*cookStacks[i])->enqueue(pCook);
		}
	}
}

bool Restaurant::LoadEvents(ifstream& inFile)
{
	// creating events

	// getting the number of events
	if (CheckEOF(inFile)) return true;
	int numberOfEvents;
	inFile >> numberOfEvents;

	// calling the appropriate functions based on the event type
	for (int i = 0; i < numberOfEvents; ++i)
	{
		if (CheckEOF(inFile)) return true;
		char eventType;
		inFile >> eventType;
		switch (eventType)
		{
		case 'R':
			if (LoadArrivalEvent(inFile)) return true;
			break;
		case 'X':
			if (LoadCancellationEvent(inFile)) return true;
			break;
		case 'P':
			if (LoadPromotionEvent(inFile)) return true;
			break;
		default:
			pGUI->PrintMessage("Error! Unknown event type!");
			pGUI->waitForClick();
			return true;
		}
	}
	return false;
}

inline bool Restaurant::LoadArrivalEvent(ifstream& inFile)
{
	// loading arrival events

	// getting the order type
	if (CheckEOF(inFile)) return true;
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
		return true;
	}

	// getting the order inputs
	int eventTimeStep, orderID, orderSize;
	double orderMoney;
	int* eventInputValuesA[] = { &eventTimeStep, &orderID, &orderSize };
	double* eventInputValuesB[] = { &orderMoney };
	if (LoadValues<int>(inFile, 3, eventInputValuesA)) return true;
	if (LoadValues<double>(inFile, 1, eventInputValuesB)) return true;

	// creating the event and adding it to the events queue
	Event* pEvent = new ArrivalEvent(eventTimeStep, orderID, orderType, orderMoney, orderSize);
	EventsQueue->enqueue(pEvent);
	
	return false;
}

inline bool Restaurant::LoadCancellationEvent(ifstream& inFile)
{
	// getting the order inputs
	int eventTimeStep, orderID;
	int* eventInputValues[] = { &eventTimeStep, &orderID };
	if (LoadValues<int>(inFile, 2, eventInputValues)) return true;

	// creating the event and adding it to the events queue
	Event* pEvent = new CancellationEvent(eventTimeStep, orderID);
	EventsQueue->enqueue(pEvent);

	return false;
}

inline bool Restaurant::LoadPromotionEvent(ifstream& inFile)
{
	// getting the order inputs
	int eventTimeStep, orderID;
	double promotionMoney;
	int* eventInputValuesA[] = { &eventTimeStep, &orderID };
	double* eventInputValuesB[] = { &promotionMoney };
	if (LoadValues<int>(inFile, 2, eventInputValuesA)) return true;
	if (LoadValues<double>(inFile, 1, eventInputValuesB)) return true;

	// creating the event and adding it to the events queue
	Event* pEvent = new PromotionEvent(eventTimeStep, orderID, promotionMoney);
	EventsQueue->enqueue(pEvent);

	return false;
}

void Restaurant::SaveRestaurant()
{
	// Prompt the user to enter the name of the savedfile
	pGUI->PrintMessage((string)"Enter the output file name:\n");

	// Get the string entered by the user and set as the file name
	ofstream outFile("Output_Files\\" + pGUI->GetString());

	// Writing the header of the table
	outFile << "FT\tID\tAT\tWT\tST" << endl;

	// Variables for the total waited time and total serving time to calculate the averages
	int totalWaitingTime = 0, totalServingTime = 0;
	
	// Loop over the total orders finished in the restaurant to print their respective Finish times
	// , IDs, etc
	for (int i = 0; i < totalOrdersCount; i++)
	{
		Order* pOrder;
		finishedQueue->dequeue(pOrder);

		// Print out all the order info in the output file

		outFile << pOrder->GetFinishTime() << '\t'
				<< pOrder->GetID() << '\t'
				<< pOrder->GetArrTime() << '\t'
				<< pOrder->GetServTime() - pOrder->GetArrTime() << '\t'
				<< pOrder->GetServTime() << endl;

		// Increment the total waited time and serving time to calculate the averages
		totalWaitingTime += (pOrder->GetServTime() - pOrder->GetArrTime());
		totalServingTime += pOrder->GetServTime();
	}

	// Calculating average waiting time and serving time
	double averageWaitingTime = (double)totalWaitingTime / totalOrdersCount;
	double averageServingTime = (double)totalServingTime / totalOrdersCount;


	// Printing the total number of orders and number of orders of each type
	outFile << "Orders: " << totalOrdersCount << '\t' << "[Norm:" << normalOrdersCount
			<< ", Veg:" << veganOrdersCount << ", VIP:" << vipOrdersCount << ']' << endl;

	// Printing total number of cooks and number of cooks of each type
	outFile << "cooks: " << totalCookCount << '\t' << "[Norm:" << normalCookCount 
			<< ", Veg:" << veganCookCount << ", VIP:" << vipCookCount << ']' << endl;

	// Printing average waiting time and average serving time
	outFile << "Avg Wait = " << averageWaitingTime << ",\t" << "Avg Serv = " << averageServingTime << endl;

	// Printing urgent orders count and percentage of auto promoted orders relative to total orders
	outFile << "Urgent Orders: " << urgentOrdersCount << ", \t" << "Auto Promoted: " 
			<< autoPromotedCount / totalOrdersCount << '%' << endl;

	// Closing file
	outFile.close();
}

//bool Restaurant::AssignOrder(int currentTimeStep, Order* pOrder, Queue<Cook*>* cookList)
//{
//	// checks the list is empty
//	if (cookList->isEmpty()) return false;
//
//	// assigning the order to the cook
//	Cook* pCook;
//	cookList->enqueue(pCook);
//	pCook->SetOrder(pOrder);
//
//	// adding the order to the in service queue and the cook to the assinged cooks
//	InServiceQueue->enqueue(pOrder, -1 * (pCook->TimeToFinishOrder()) - currentTimeStep);
//	assignedCooks->enqueue(pCook, -1 * (pCook->TimeToFinishOrder()) - currentTimeStep);
//	
//	// updating the total money
//	totalMoney += pOrder->GetMoney();
//	
//	// updating the order and cook data members
//	pOrder->SetFinishTime(pCook->TimeToFinishOrder() + currentTimeStep);
//	pOrder->SetServTime(currentTimeStep);
//	pOrder->SetStatus(SRV);
//	pCook->SetIsCooking(true);
//
//	return true;
//}

//void Restaurant::AssignToCook()
//{
//	Order* pOrder;
//	
//	// assigning the urgent orders
//	while (urgentOrderQueue->dequeue(pOrder) && (!availableCooks->IsEmpty() || !restingCooks->isEmpty()))
//	{
//		if (AssignOrder(currentTimeSteps, pOrder, vipCooks)) vipCookCount--;
//		else if (AssignOrder(currentTimeSteps, pOrder, normalCooks)) normalCookCount--;
//		else if (AssignOrder(currentTimeSteps, pOrder, veganCooks)) veganCookCount--;
//		else if (!restingCooks->isEmpty())
//		{
//			Cook* pCook;
//			restingCooks->dequeue(pCook);
//			pCook->SetOrder(pOrder);
//			InServiceQueue->enqueue(pOrder, -1 * (pCook->TimeToFinishOrder()) - currentTimeSteps);
//			assignedCooks->enqueue(pCook, -1 * (pCook->TimeToFinishOrder()) - currentTimeSteps);
//			totalMoney += pOrder->GetMoney();
//			pOrder->SetFinishTime(pCook->TimeToFinishOrder() + currentTimeSteps);
//			pOrder->SetServTime(currentTimeSteps);
//			pOrder->SetStatus(SRV);
//			pCook->SetIsResting(false);
//			pCook->SetIsCooking(true);
//		}
//		else break;
//	}
//
//	// assigning the vip orders
//	while (vipOrderQueue->dequeue(pOrder) && !availableCooks->IsEmpty())
//	{
//		if (AssignOrder(currentTimeSteps, pOrder, vipCooks)) vipCookCount--;
//		else if (AssignOrder(currentTimeSteps, pOrder, normalCooks)) normalCookCount--;
//		else if (AssignOrder(currentTimeSteps, pOrder, veganCooks)) veganCookCount--;
//		else break;
//	}
//
//	// assigning the vegan orders
//	while (veganOrderQueue->dequeue(pOrder) && !availableCooks->IsEmpty())
//	{
//		if (AssignOrder(currentTimeSteps, pOrder, veganCooks)) veganCookCount--;
//		else break;
//	}
//
//	// assigning the normal orders
//	while (normalOrderQueue->dequeue(pOrder) && !availableCooks->IsEmpty())
//	{
//		if (AssignOrder(currentTimeSteps, pOrder, normalCooks)) normalCookCount--;
//		else if (AssignOrder(currentTimeSteps, pOrder, vipCooks)) vipCookCount--;
//		else break;
//	}
//}

void Restaurant::CheckFinishedOrders()
{
	bool flag = true;
	Cook* busy_cook;
	while (assignedCooks->peekFront(busy_cook) && flag)
	{
		if (busy_cook->GetIsCooking() && (busy_cook->GetOrder())->GetFinishTime() == currentTimeSteps)
		{
			Order* finished_order = busy_cook->GetOrder();
			InServiceQueue->dequeue(finished_order);
			finishedQueue->enqueue(finished_order);
			finished_order->SetStatus(DONE);

			totalOrdersCount++;
			switch (finished_order->GetType())
			{
			case TYPE_NRM:
				normalOrdersCount++;
				break;
			case TYPE_VIP:
				vipOrdersCount++;
				break;
			case TYPE_VGAN:
				veganOrdersCount++;
				break;
			case TYPE_URGNT:
				vipOrdersCount++;
				urgentOrdersCount++;
				break;
			}

			assignedCooks->dequeue(busy_cook);
			if (busy_cook->GetIsInjured())
			{
				busy_cook->SetIsInjured(false);
				busy_cook->SetIsResting(true);
				busy_cook->SetBreakTimeEnd(restSteps + currentTimeSteps);
				restingCooks->enqueue(busy_cook, -restSteps - currentTimeSteps);
			}
			else if (busy_cook->NeedBreak())
			{
				busy_cook->SetIsResting(true);
				busy_cook->SetBreakTimeEnd(busy_cook->GetBreakDuration() + currentTimeSteps);
				restingCooks->enqueue(busy_cook, -busy_cook->GetBreakDuration() - currentTimeSteps);
			}
			else
			{
				switch (busy_cook->GetType())
				{
				case TYPE_NRM:
					normalCooks->enqueue(busy_cook);
					normalCookCount++;
					break;
				case TYPE_VIP:
					vipCooks->enqueue(busy_cook);
					vipCookCount++;
					break;
				case TYPE_VGAN:
					veganCooks->enqueue(busy_cook);
					veganCookCount++;
					break;
				}
			}
		}
		else
			flag = false;
	}
}

void Restaurant::CheckCooksBreaks()
{
	Cook* busy_cook;
	bool flag = true;
	while (restingCooks->peekFront(busy_cook) && flag)
	{
		if (busy_cook->GetBreakTimeEnd()==currentTimeSteps)
		{
			restingCooks->dequeue(busy_cook);

			switch (busy_cook->GetType())
			{
			case TYPE_NRM:
				normalCooks->enqueue(busy_cook);
				normalCookCount++;
				break;
			case TYPE_VIP:
				vipCooks->enqueue(busy_cook);
				vipCookCount++;
				break;
			case TYPE_VGAN:
				veganCooks->enqueue(busy_cook);
				veganCookCount++;
				break;
			}
		}
		else
			flag = false;
	}
}


void Restaurant::Injury()
{
	Order* pOrder;
	Cook *pCook;

	if (((rand() % 100)) <= injuryProbability * 100)
	{
		// peek check if there are cooks that are unavailable
		if (assignedCooks->peekFront(pCook) && !pCook->GetIsInjured())
		{
			// check if the cook is cooking an order
			if (pCook->GetIsCooking())
			{
				// Dequeue the cook from the unavailable priority queue, as his time in the unavailable
				// queue just increased
				assignedCooks->dequeue(pCook);
				pCook->SetIsInjured(true);

				// Increment total number of injuries in the restaurant
				injuredCount++;


				pOrder = pCook->GetOrder();
				InServiceQueue->enqueue(pOrder, -(pOrder->GetFinishTime() * 2 - currentTimeSteps));
				assignedCooks->enqueue(pCook, -(pOrder->GetFinishTime() * 2 - currentTimeSteps));
				pOrder->SetFinishTime(pOrder->GetFinishTime() * 2 - currentTimeSteps);
				InServiceQueue->dequeue(pOrder);
			}
		}
	}
}

void Restaurant::PromoteOrder(int ID, double promotionMoney)
{
	int count;
	int flag = -1;
	Order** normal = normalOrderQueue->toArray(count);
	Order* traverser;
	Order* toBePromoted;

	for (int i = 0; i < count; i++)
	{
		if (normal[i]->GetID() == ID)
		{
			toBePromoted = normal[i];
			flag = i;
		}
	}

	for (int i = 0; i < count; i++)
	{
		normalOrderQueue->dequeue(traverser);
	}

	for (int i = 0; i < count; i++)
	{
		if (i != flag)
			normalOrderQueue->enqueue(normal[i]);
		else
		{
			autoPromotedCount++;
			toBePromoted->SetType(TYPE_VIP);
			toBePromoted->SetWaitTime(0);
			toBePromoted->SetMoney(toBePromoted->GetMoney() + promotionMoney);
			int priority = CalcPriority(toBePromoted);
			vipOrderQueue->enqueue(toBePromoted,priority);
		}
	}
}

void Restaurant::UpdateUrgentOrders()
{
	PriorityQueue<Order*>* newVipOrderQueue = new PriorityQueue<Order*>;
	Order* currentOrder;
	while (vipOrderQueue->dequeue(currentOrder))
	{
		int currentOrderPriority = CalcPriority(currentOrder);
		if ((currentOrder->GetWaitTime()) >= urgentSteps)
		{
			currentOrder->SetType(TYPE_URGNT);
			urgentOrderQueue->enqueue(currentOrder, currentOrderPriority);
		}
		else
		{
			newVipOrderQueue->enqueue(currentOrder, currentOrderPriority);
		}
	}
	delete vipOrderQueue;
	vipOrderQueue = newVipOrderQueue;
}

void Restaurant::AutoPromote()
{
	Order* currentOrder;
	while (normalOrderQueue->peekFront(currentOrder))
	{
		if (currentTimeSteps - currentOrder->GetArrTime() >= autoPromotionSteps)
		{
			normalOrderQueue->dequeue(currentOrder);
			currentOrder->SetType(TYPE_VIP);
			currentOrder->SetWaitTime(0);
			vipOrderQueue->enqueue(currentOrder, CalcPriority(currentOrder));
		}
		else break;
	}
}

int Restaurant::CalcPriority(Order* O)
{
	return (10000 * O->GetMoney() / ((double)(O->GetSize()) * O->GetArrTime()));
}

void Restaurant::UpdateWaitingTime()
{
	int normal_count = normalOrderQueue->getCounter();
	int vegan_count = veganOrderQueue->getCounter();
	int vip_count = vipOrderQueue->getCount();
	int urgent_count = urgentOrderQueue->getCount();
	Order* porder;
	for (int i = 0; i < normal_count; i++)
	{
		normalOrderQueue->dequeue(porder);
		porder->SetWaitTime(porder->GetWaitTime()+1);
		normalOrderQueue->enqueue(porder);
	}
	for (int i = 0; i < vegan_count; i++)
	{
		veganOrderQueue->dequeue(porder);
		porder->SetWaitTime(porder->GetWaitTime() + 1);
		veganOrderQueue->enqueue(porder);
	}
	Queue<Order*>* temp = new Queue<Order*>;
	for (int i = 0; i < vip_count; i++)
	{
		vipOrderQueue->dequeue(porder);
		porder->SetWaitTime(porder->GetWaitTime() + 1);
		temp->enqueue(porder);
	}
	for (int i = 0; i < vip_count; i++)
	{
		temp->dequeue(porder);
		vipOrderQueue->enqueue(porder, CalcPriority(porder));
	}
	delete temp;
}

void Restaurant::AssignToCook2()
{
	AssignUrgent();
	AssignVIP();
	AssignVegan();
	AssignNormal();
}

void Restaurant::AssignUrgent()
{
	Cook* cook;
	Order* inorder;
	
	while (urgentOrderQueue->peekFront(inorder))
	{
		if (vipCooks->peekFront(cook))	// Check if a VIP Cook is available
		{
			// remove the order from the urgent queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;

		}
		else if (normalCooks->peekFront(cook))	// If VIP cook isn't available check for Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Normal cook, assign him an order and add him to the unavailable cooks queue
			normalCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (veganCooks->peekFront(cook))	// If Normal and VIP cooks aren't available Check for Vegan
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available Vegan Cooks count
			veganCookCount--;
		}
		else if (restingCooks->peekFront(cook))
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// dequeue the resting cook, assign him an order and add him to the unavailable cooks queue
			restingCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking and not resting
			inorder->SetStatus(SRV);
			cook->SetIsResting(false);
			cook->SetIsCooking(true);
		}
		else
			break;
	}
}

void Restaurant::AssignVIP()
{
	Cook* cook;
	Order* inorder;
	while (vipOrderQueue->peekFront(inorder))
	{
		if (vipCooks->peekFront(cook))	// Check if a VIP Cook is available
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;

		}
		else if (normalCooks->peekFront(cook))	// If VIP cook isn't available check for Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Normal cook, assign him an order and add him to the unavailable cooks queue
			normalCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (veganCooks->peekFront(cook))	// If Normal and VIP cooks aren't available Check for Vegan
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available Vegan Cooks count
			veganCookCount--;
		}
		else
			break;
	}
}

void Restaurant::AssignVegan()
{
	Cook* cook;
	Order* inorder;

	while (veganOrderQueue->peekFront(inorder))
	{
		if (veganCooks->peekFront(cook))	// Check for an available Vegan Cook
		{
			// remove the order from the waiting queue to inservice queue
			veganOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement count of available vegan cooks
			veganCookCount--;
		}
		else
			break;
	}
}

void Restaurant::AssignNormal() 
{
	Cook* cook;
	Order* inorder;

	while (normalOrderQueue->peekFront(inorder))
	{
		if (normalCooks->peekFront(cook))	// Check for an Available Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			normalOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the vegan cook, assign him an order and add him to the unavailable cooks queue
			normalCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (vipCooks->peekFront(cook))	// If Normal Cooks aren't available check for a VIP cook
		{
			// remove the order from the waiting queue to inservice queue
			normalOrderQueue->dequeue(inorder);
			cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks->dequeue(cook);
			assignedCooks->enqueue(cook, -cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;
		}
		else
			break;
	}
}

void Restaurant::AssignToCook()
{
	bool flag = true;
	Cook* vip_cook, * vegan_cook, * normal_cook, * resting_cook;
	Order* inorder;

	while (urgentOrderQueue->peekFront(inorder) && flag)
	{
		if (vipCooks->peekFront(vip_cook))	// Check if a VIP Cook is available
		{
			// remove the order from the urgent queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			vip_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -vip_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps-inorder->GetArrTime());

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks->dequeue(vip_cook);
			assignedCooks->enqueue(vip_cook, -vip_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vip_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vip_cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;

		}
		else if (normalCooks->peekFront(normal_cook))	// If VIP cook isn't available check for Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			normal_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -normal_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Normal cook, assign him an order and add him to the unavailable cooks queue
			normalCooks->dequeue(normal_cook);
			assignedCooks->enqueue(normal_cook, -normal_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(normal_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			normal_cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (veganCooks->peekFront(vegan_cook))	// If Normal and VIP cooks aren't available Check for Vegan
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			vegan_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -vegan_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks->dequeue(vegan_cook);
			assignedCooks->enqueue(vegan_cook, -vegan_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vegan_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vegan_cook->SetIsCooking(true);

			// Decrement Available Vegan Cooks count
			veganCookCount--;
		}
		else if (restingCooks->peekFront(resting_cook))
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue->dequeue(inorder);
			resting_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -resting_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// dequeue the resting cook, assign him an order and add him to the unavailable cooks queue
			restingCooks->dequeue(resting_cook);
			assignedCooks->enqueue(resting_cook, -resting_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(resting_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking and not resting
			inorder->SetStatus(SRV);
			resting_cook->SetIsResting(false);
			resting_cook->SetIsCooking(true);
		}
		else
			flag = false;
	}

	flag = true;

	while (vipOrderQueue->peekFront(inorder) && flag)
	{
		if (vipCooks->peekFront(vip_cook))	// Check if a VIP Cook is available
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue->dequeue(inorder);
			vip_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -vip_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks->dequeue(vip_cook);
			assignedCooks->enqueue(vip_cook, -vip_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vip_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vip_cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;

		}
		else if (normalCooks->peekFront(normal_cook))	// If VIP cook isn't available check for Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue->dequeue(inorder);
			normal_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -normal_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Normal cook, assign him an order and add him to the unavailable cooks queue
			normalCooks->dequeue(normal_cook);
			assignedCooks->enqueue(normal_cook, -normal_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(normal_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			normal_cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (veganCooks->peekFront(vegan_cook))	// If Normal and VIP cooks aren't available Check for Vegan
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue->dequeue(inorder);
			vegan_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -vegan_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the Vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks->dequeue(vegan_cook);
			assignedCooks->enqueue(vegan_cook, -vegan_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vegan_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vegan_cook->SetIsCooking(true);

			// Decrement Available Vegan Cooks count
			veganCookCount--;
		}
		else
			flag = false;
	}

	flag = true;

	while (veganOrderQueue->peekFront(inorder) && flag)
	{
		if (veganCooks->peekFront(vegan_cook))	// Check for an available Vegan Cook
		{
			// remove the order from the waiting queue to inservice queue
			veganOrderQueue->dequeue(inorder);
			vegan_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -vegan_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks->dequeue(vegan_cook);
			assignedCooks->enqueue(vegan_cook, -vegan_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vegan_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vegan_cook->SetIsCooking(true);

			// Decrement count of available vegan cooks
			veganCookCount--;
		}
		else
			flag = false;
	}

	flag = true;

	while (normalOrderQueue->peekFront(inorder) && flag)
	{
		if (normalCooks->peekFront(normal_cook))	// Check for an Available Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			normalOrderQueue->dequeue(inorder);
			normal_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -normal_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the vegan cook, assign him an order and add him to the unavailable cooks queue
			normalCooks->dequeue(normal_cook);
			assignedCooks->enqueue(normal_cook, -normal_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(normal_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			normal_cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (vipCooks->peekFront(vip_cook))	// If Normal Cooks aren't available check for a VIP cook
		{
			// remove the order from the waiting queue to inservice queue
			normalOrderQueue->dequeue(inorder);
			vip_cook->SetOrder(inorder);
			InServiceQueue->enqueue(inorder, -vip_cook->TimeToFinishOrder() - currentTimeSteps);
			inorder->SetWaitTime(currentTimeSteps - inorder->GetArrTime());

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks->dequeue(vip_cook);
			assignedCooks->enqueue(vip_cook, -vip_cook->TimeToFinishOrder() - currentTimeSteps);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vip_cook->TimeToFinishOrder() + currentTimeSteps);

			// Set Service Start Time for the order
			inorder->SetServTime(currentTimeSteps);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vip_cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;
		}
		else
			flag = false;
	}
}