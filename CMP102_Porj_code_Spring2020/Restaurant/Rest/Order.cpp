#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}

void Order::SetType(ORD_TYPE type_)
{
	type = type_;
}

ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}

int Order::GetDistance() const
{
	return Distance;
}


void Order::SetStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::GetStatus() const
{
	return status;
}

void Order::SetSize(int size)
{
	Size = size;
}

int Order::GetSize()
{
	return Size;
}

void Order::SetMoney(double money)
{
	totalMoney = money;
}

double Order::GetMoney()
{
	return totalMoney;
}

void Order::SetArrTime(int time)
{
	ArrTime = time;
}

int Order::GetArrTime()
{
	return ArrTime;
}

void Order::SetServTime(int time)
{
	ServTime = time;
}

int Order::GetServTime()
{
	return ServTime;
}

void Order::SetFinishTime(int time)
{
	FinishTime = time;
}

int Order::GetFinishTime()
{
	return FinishTime;
}

Order::Order()
{

}

Order::Order(int id, ORD_TYPE r_Type, int s, double money, int aritime)
{
	ID = id;
	type = r_Type;
	Size = s;
	status = WAIT;
	totalMoney = money;
	ArrTime = aritime;
	ServTime = 0;
	FinishTime = 0;
}


