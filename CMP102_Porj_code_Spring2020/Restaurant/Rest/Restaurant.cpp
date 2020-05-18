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
	totalCookCount = 0;
	totalTimeSteps = 1;
	totalMoney = 0;
	injuredCount = 0;
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
	for (int i = 0; i < totalCookCount; ++i)
	{
		pGUI->AddToDrawingList(availableCooks.getEntry(i));
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
	Order** served = InServiceQueue_test.toArray(served_count);
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
		"Number of available normal cooks : " + to_string(normalCookCount) + '\n' +
		"Number of available vegan cooks : " + to_string(veganCookCount) + '\n' +
		"Number of available vip cooks : " + to_string(vipCookCount) + '\n' +
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

inline bool Restaurant::CheckEOF(ifstream& inFile)
{
	if (inFile.eof())
	{
		return true;
	}
	return false;
}

template <typename T>
bool Restaurant::LoadValues(ifstream& inFile, int itemCount, T** items)
{
	for (int i; i < itemCount; ++i)
	{
		if (CheckEOF(inFile)) return true;
		cin >> *items[i];
	}
	return false;
}

void Restaurant::LoadRestaurant()
{
	pGUI->PrintMessage((string)"[The input file must be placed in the Input_Files folder]\n" +
		"[test0.txt, test1.txt, test3.txt, test4.txt files are already provided in the folder]\n" +
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

	// declaring the variables to be used to store the data from the input file
	int normalCookCountInput, veganCookCountInput, vipCookCountInput;
	int minNormalSpeed, minVeganSpeed, minVipSpeed, maxNormalSpeed, maxVeganSpeed, maxVipSpeed;
	int ordersBeforeBreak, minNormalBreak, minVeganBreak, minVipBreak, maxNormalBreak, maxVeganBreak, maxVipBreak;
	int injuryProbability, restStepsInput;
	int autoPromotionStepsInput, urgentStepsInput;
	int numberOfEvents;

	// creating an array containing pointers to these variables in order to easily loop over them
	// to store the data from the input file
	int* inputValues[] =
	{
		&normalCookCountInput, &veganCookCountInput, &vipCookCountInput,
		&minNormalSpeed, &maxNormalSpeed, &minVeganSpeed, &maxVeganSpeed, &minVipSpeed, &maxVipSpeed,
		&ordersBeforeBreak, &minNormalBreak, &maxNormalBreak, &minVeganBreak, &maxVeganBreak, &minVipBreak, &maxVipBreak,
		&injuryProbability, &restStepsInput,
		&autoPromotionStepsInput, &urgentStepsInput,
	};

	// getting the input values
	if (LoadValues<int>(inFile, 20, inputValues)) return;

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
	if (LoadEvents(inFile)) return;

	// closing the file
	inFile.close();
}

void Restaurant::LoadCooks(int ordersBeforeBreak, int* cookCounts, int cookSpeeds[3][2], int cookBreaks[3][2])
{
	// creating the cooks

	Stack<Cook*> cookStacks[] = { normalCooks, veganCooks, vipCooks };
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
			availableCooks.Append(pCook);
			cookStacks[i].push(pCook);
		}
	}
}

bool Restaurant::LoadEvents(ifstream& inFile)
{
	// creating events

	// getting the number of events
	if (CheckEOF(inFile)) return true;
	int numberOfEvents;
	cin >> numberOfEvents;

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
			pGUI->waitForClick();
			pGUI->PrintMessage("Error! Unknown event type!");
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
	cin >> orderTypeInput;
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
	double eventTimeStep, orderID, orderSize, orderMoney;
	double* eventInputValues[] = { &eventTimeStep, &orderID, &orderSize, &orderMoney };
	if (LoadValues<double>(inFile, 4, eventInputValues)) return true;

	// creating the event and adding it to the events queue
	Event* pEvent = new ArrivalEvent(eventTimeStep, orderID, orderType, orderMoney, orderSize);
	EventsQueue.enqueue(pEvent);
	
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
	EventsQueue.enqueue(pEvent);

	return false;
}

inline bool Restaurant::LoadPromotionEvent(ifstream& inFile)
{
	// getting the order inputs
	double eventTimeStep, orderID, promotionMoney;
	double* eventInputValues[] = { &eventTimeStep, &orderID, &promotionMoney };
	if (LoadValues<double>(inFile, 3, eventInputValues)) return true;

	// adding the promotion money to the total money
	totalMoney += promotionMoney;

	// creating the event and adding it to the events queue
	Event* pEvent = new PromotionEvent(eventTimeStep, orderID, promotionMoney);
	EventsQueue.enqueue(pEvent);

	return false;
}

void Restaurant::SaveRestaurant()
{
	// Prompt the user to enter the name of the savedfile
	pGUI->PrintMessage((string)"Enter the output file name:\n");

	// Get the string entered by the user and set as the file name
	ofstream outFile(pGUI->GetString());

	// Writing the header of the table
	outFile << "FT\tID\tAT\tWT\tST" << endl;

	// Variables for the total waited time and total serving time to calculate the averages
	int totalWaitingTime = 0, totalServingTime = 0;
	
	// Loop over the total orders finished in the restaurant to print their respective Finish times
	// , IDs, etc
	for (int i = 0; i < totalOrdersCount; i++)
	{
		Order* pOrder;
		finishedQueue.dequeue(pOrder);

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

void Restaurant::Assign_to_cook(int current_time_step)
{
	bool flag = true;
	Cook* vip_cook, * vegan_cook, * normal_cook, * resting_cook;
	Order* inorder;

	while (urgentOrderQueue.peekFront(inorder) && flag)
	{
		if (vipCooks.peek(vip_cook))	// Check if a VIP Cook is available
		{
			// remove the order from the urgent queue to inservice queue
			urgentOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -vip_cook->TimeToFinishOrder() - current_time_step);

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks.pop(vip_cook);
			vip_cook->SetOrder(inorder);
			assignedCooks.enqueue(vip_cook, -vip_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vip_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vip_cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;

		}
		else if (normalCooks.peek(normal_cook))	// If VIP cook isn't available check for Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -normal_cook->TimeToFinishOrder() - current_time_step);

			// Pop the Normal cook, assign him an order and add him to the unavailable cooks queue
			normalCooks.pop(normal_cook);
			normal_cook->SetOrder(inorder);
			assignedCooks.enqueue(normal_cook, -normal_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(normal_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			normal_cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (veganCooks.peek(vegan_cook))	// If Normal and VIP cooks aren't available Check for Vegan
		{
			// remove the order from the waiting queue to inservice queue
			urgentOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -vegan_cook->TimeToFinishOrder() - current_time_step);

			// Pop the Vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks.pop(vegan_cook);
			vegan_cook->SetOrder(inorder);
			assignedCooks.enqueue(vegan_cook, -vegan_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vegan_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vegan_cook->SetIsCooking(true);

			// Decrement Available Vegan Cooks count
			veganCookCount--;
		}
		else if (restingCooks.peekFront(resting_cook))
		{
			// remove the order from the waiting queue to inservice queue
		    urgentOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -resting_cook->TimeToFinishOrder() - current_time_step);

			// dequeue the resting cook, assign him an order and add him to the unavailable cooks queue
			restingCooks.dequeue(resting_cook);
			resting_cook->SetOrder(inorder);
			assignedCooks.enqueue(resting_cook, -resting_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(resting_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

			// Change Order Status to In Service and set Cook status to Cooking and not resting
			inorder->SetStatus(SRV);
			resting_cook->SetIsResting(false);
			resting_cook->SetIsCooking(true);
		}
		else
			flag = false;
	}

	flag = true;

	while (vipOrderQueue.peekFront(inorder) && flag)
	{
		if (vipCooks.peek(vip_cook))	// Check if a VIP Cook is available
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -vip_cook->TimeToFinishOrder() - current_time_step);

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks.pop(vip_cook);
			vip_cook->SetOrder(inorder);
			assignedCooks.enqueue(vip_cook, -vip_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vip_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			vip_cook->SetIsCooking(true);

			// Decrement Available VIP Cooks count
			vipCookCount--;

		}
		else if (normalCooks.peek(normal_cook))	// If VIP cook isn't available check for Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -normal_cook->TimeToFinishOrder() - current_time_step);

			// Pop the Normal cook, assign him an order and add him to the unavailable cooks queue
			normalCooks.pop(normal_cook);
			normal_cook->SetOrder(inorder);
			assignedCooks.enqueue(normal_cook, -normal_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(normal_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			normal_cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (veganCooks.peek(vegan_cook))	// If Normal and VIP cooks aren't available Check for Vegan
		{
			// remove the order from the waiting queue to inservice queue
			vipOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -vegan_cook->TimeToFinishOrder() - current_time_step);

			// Pop the Vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks.pop(vegan_cook);
			vegan_cook->SetOrder(inorder);
			assignedCooks.enqueue(vegan_cook, -vegan_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vegan_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

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

	while (veganOrderQueue.peekFront(inorder) && flag)
	{
		if (veganCooks.peek(vegan_cook))	// Check for an available Vegan Cook
		{
			// remove the order from the waiting queue to inservice queue
			veganOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -vegan_cook->TimeToFinishOrder() - current_time_step);

			// Pop the vegan cook, assign him an order and add him to the unavailable cooks queue
			veganCooks.pop(vegan_cook);
			vegan_cook->SetOrder(inorder);
			assignedCooks.enqueue(vegan_cook, -vegan_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vegan_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

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

	while (normalOrderQueue.peekFront(inorder) && flag)
	{
		if (normalCooks.peek(normal_cook))	// Check for an Available Normal Cook
		{
			// remove the order from the waiting queue to inservice queue
			normalOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -normal_cook->TimeToFinishOrder() - current_time_step);

			// Pop the vegan cook, assign him an order and add him to the unavailable cooks queue
			normalCooks.pop(normal_cook);
			normal_cook->SetOrder(inorder);
			assignedCooks.enqueue(normal_cook, -normal_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(normal_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

			// Change Order Status to In Service and set Cook status to Cooking
			inorder->SetStatus(SRV);
			normal_cook->SetIsCooking(true);

			// Decrement Available Normal Cooks count
			normalCookCount--;
		}
		else if (vipCooks.peek(vip_cook))	// If Normal Cooks aren't available check for a VIP cook
		{
			// remove the order from the waiting queue to inservice queue
			normalOrderQueue.dequeue(inorder);
			InServiceQueue_test.enqueue(inorder, -vip_cook->TimeToFinishOrder() - current_time_step);

			// Pop the VIP cook, assign him an order and add him to the unavailable cooks queue
			vipCooks.pop(vip_cook);
			vip_cook->SetOrder(inorder);
			assignedCooks.enqueue(vip_cook, -vip_cook->TimeToFinishOrder() - current_time_step);

			// Add price of order to total restaurant money
			totalMoney += inorder->GetMoney();

			// Set Finish Time for the order
			inorder->SetFinishTime(vip_cook->TimeToFinishOrder() + current_time_step);

			// Set Service Start Time for the order
			inorder->SetServTime(current_time_step);

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

void Restaurant::check_finished_orders(int current_time_step)
{
	bool flag = true;
	Cook* busy_cook;
	while (assignedCooks.peekFront(busy_cook) && flag)
	{
		if (busy_cook->GetIsCooking() && (busy_cook->GetOrder())->GetFinishTime() == current_time_step)
		{
			Order* finished_order = busy_cook->GetOrder();
			InServiceQueue_test.dequeue(finished_order);
			finishedQueue.enqueue(finished_order);
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
			case(TYPE_URGNT):
				vipCookCount++;
				urgentOrdersCount++;
				break;
			}

			assignedCooks.dequeue(busy_cook);
			if (busy_cook->GetIsInjured())
			{
				busy_cook->SetIsInjured(false);
				busy_cook->SetIsResting(true);
				busy_cook->SetBreakTimeEnd(restSteps + current_time_step);
				restingCooks.enqueue(busy_cook, -restSteps - current_time_step);
			}
			else if (busy_cook->NeedBreak())
			{
				busy_cook->SetIsResting(true);
				busy_cook->SetBreakTimeEnd(busy_cook->GetBreakDuration() + current_time_step);
				restingCooks.enqueue(busy_cook, -busy_cook->GetBreakDuration() - current_time_step);
			}
			else
			{
				switch (busy_cook->GetType())
				{
				case TYPE_NRM:
					normalCooks.push(busy_cook);
					normalCookCount++;
					break;
				case TYPE_VIP:
					vipCooks.push(busy_cook);
					vipCookCount++;
					break;
				case TYPE_VGAN:
					veganCooks.push(busy_cook);
					veganCookCount++;
					break;
				}
			}
		}
		else
			flag = false;
	}
}

void Restaurant::check_cooks_breaks(int current_time_step)
{
	Cook* busy_cook;
	bool flag = true;
	while (restingCooks.peekFront(busy_cook) && flag)
	{
		if (busy_cook->GetBreakTimeEnd()==current_time_step)
		{
			restingCooks.dequeue(busy_cook);

			switch (busy_cook->GetType())
			{
			case TYPE_NRM:
				normalCooks.push(busy_cook);
				normalCookCount++;
				break;
			case TYPE_VIP:
				vipCooks.push(busy_cook);
				vipCookCount++;
				break;
			case TYPE_VGAN:
				veganCooks.push(busy_cook);
				veganCookCount++;
				break;
			}
		}
		else
			flag = false;
	}
}

void Restaurant::SimpleSimulator()
{

	// place of loading calling
	LoadRestaurant();
	while (!(EventsQueue.isEmpty() && normalOrderQueue.isEmpty() && veganOrderQueue.isEmpty() && vipOrderQueue.isEmpty() && InServiceQueue.isEmpty()))
	{
		ExecuteEvents(totalTimeSteps);
		Order* normal,* vegan,* vip;
		if (normalOrderQueue.peekFront(normal) && normalCookCount)
		{
			normalOrderQueue.dequeue(normal);
			normal->SetStatus(SRV);
			normal->SetServTime(totalTimeSteps);
			InServiceQueue.enqueue(normal);
			normalCookCount--;
		}
		if (veganOrderQueue.peekFront(vegan) && veganCookCount)
		{
			veganOrderQueue.dequeue(vegan);
			vegan->SetStatus(SRV);
			vegan->SetServTime(totalTimeSteps);
			InServiceQueue.enqueue(vegan);
			veganCookCount--;
		}
		if (vipOrderQueue.peekFront(vip) && vipCookCount)
		{
			vipOrderQueue.dequeue(vip);
			vip->SetStatus(SRV);
			vip->SetServTime(totalTimeSteps);
			InServiceQueue.enqueue(vip);
			vipCookCount--;
		}

		if (totalTimeSteps % 5 == 0)
		{
			Order* finished;
			for (int i = 0; i < 3; i++)
			{
				if (InServiceQueue.peekFront(finished))
				{
					InServiceQueue.dequeue(finished);
					finished->SetStatus(DONE);
					finished->SetFinishTime(totalTimeSteps);
					finishedQueue.enqueue(finished);
					switch (finished->GetType())
					{
					case(TYPE_NRM):
						normalCookCount++;
						break;
					case(TYPE_VGAN):
						veganCookCount++;
						break;
					case(TYPE_VIP):
						vipCookCount++;
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

void Restaurant::Injury(int current_time_step)
{
	Order* pOrder;
	Cook *pCook;

	if (((rand() % 100) / 100) <= injuryProbability * 100)
	{
		// peek check if there are cooks that are unavailable
		if (assignedCooks.peekFront(pCook) && !pCook->GetIsInjured())
		{
			// check if the cook is cooking an order
			if (pCook->GetIsCooking())
			{
				// Dequeue the cook from the unavailable priority queue, as his time in the unavailable
				// queue just increased
				assignedCooks.dequeue(pCook);
				pCook->SetIsInjured(true);

				// Increment total number of injuries in the restaurant
				injuredCount++;


				pOrder = pCook->GetOrder();
				pOrder->SetFinishTime(pOrder->GetFinishTime() * 2 - current_time_step);

				assignedCooks.enqueue(pCook, -(pOrder->GetFinishTime() * 2 - current_time_step));
			}
		}
	}
}

void Restaurant::PromoteOrder(int ID, double promotionMoney)
{
	// AUTO-PROMOTION IS STILL NOT IMPLEMENTED WAITING FOR INTERACTIVE MODE

	// Leave these here for now, we might need em later
	//int wait_time;
	//Order* pOrder;


	int count;
	int flag = -1;
	Order** NORMAL = normalOrderQueue.toArray(count);
	Order* x;
	
	for (int i = 0; i < count; i++)
	{
		if (NORMAL[i]->GetID() == ID)
			flag = i;
	}

	for (int i = 0; i < count; i++)
	{
		normalOrderQueue.dequeue(x);
	}

	for (int i = 0; i < count; i++)
	{
		if (i != flag)
			normalOrderQueue.enqueue(NORMAL[i]);
		else
		{
			autoPromotedCount++;
			x->SetType(TYPE_VIP);
			x->SetMoney(x->GetMoney() + promotionMoney);
			int priority = calcPriority(x);
			vipOrderQueue.enqueue(x,priority);
		}
	}

	/*if (normalOrderQueue.peekFront(pOrder))
	{
		normalOrderQueue.dequeue(pOrder);
		pOrder->SetType(TYPE_VIP);
		vipOrderQueue.enqueue(pOrder, priority);

		if (currentimestep - pOrder->GetArrTime() >= autoPromotionSteps)
		{
			autoPromotedCount++;
		}
	}*/
}

void Restaurant::UpdateUrgentOrders(int currentTimeStep)
{
	PriorityQueue<Order*> newVipOrderQueue;
	Order* currentOrder;
	while (vipOrderQueue.dequeue(currentOrder))
	{
		int currentOrderPriority = calcPriority(currentOrder);
		if ((currentTimeStep - currentOrder->GetArrTime()) > urgentSteps)
		{
			currentOrder->SetType(TYPE_URGNT);
			urgentOrderQueue.enqueue(currentOrder, currentOrderPriority);
		}
		else
		{
			newVipOrderQueue.enqueue(currentOrder, currentOrderPriority);
		}
	}
	vipOrderQueue = newVipOrderQueue;
}

void Restaurant::AutoPromote(int currentTimeStep)
{
	Order* currentOrder;
	while (normalOrderQueue.peekFront(currentOrder))
	{
		if (currentTimeStep - currentOrder->GetArrTime() > autoPromotionSteps)
		{
			normalOrderQueue.dequeue(currentOrder);
			currentOrder->SetType(TYPE_VIP);
			vipOrderQueue.enqueue(currentOrder, calcPriority(currentOrder));
		}
		else break;
	}
}

int Restaurant::calcPriority(Order* O)
{
	return (10000 * O->GetMoney() / ((double)(O->GetSize()) * O->GetArrTime()));
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
