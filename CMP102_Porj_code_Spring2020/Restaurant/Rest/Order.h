#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant 
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times
	
	
	//
	// TODO: Add More Data Members As Needed
	int Size;
	//

public:
	Order(int ID, ORD_TYPE r_Type);
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	void SetDistance(int d);
	int GetDistance() const;

	void SetStatus(ORD_STATUS s);
	ORD_STATUS GetStatus() const;
	
	//
	// TODO: Add More Member Functions As Needed

	Order();
	Order(int id, ORD_TYPE r_Type,int size, double money, int aritime);
	void SetSize(int size);
	int GetSize();

	void SetMoney(double money);
	double GetMoney();

	void SetArrTime(int time);
	int GetArrTime();

	void SetServTime(int time);
	int GetServTime();

	void SetFinishTime(int time);
	int GetFinishTime();

	//

};

#endif