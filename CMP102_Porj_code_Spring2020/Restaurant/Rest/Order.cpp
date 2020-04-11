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


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::setSize(int size)
{
	Size = size;
}

int Order::getSize()
{
	return Size;
}

void Order::setMoney(double money)
{
	totalMoney = money;
}

double Order::getMoney()
{
	return totalMoney;
}

void Order::setArrTime(int time)
{
	ArrTime = time;
}

int Order::getArrTime()
{
	return ArrTime;
}

void Order::setServTime(int time)
{
	ServTime = time;
}

int Order::getServTime()
{
	return ServTime;
}

void Order::setFinishTime(int time)
{
	FinishTime = time;
}

int Order::getFinishTime()
{
	return FinishTime;
}

Order::Order()
{

}