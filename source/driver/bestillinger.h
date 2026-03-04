#pragma once
#include "elevio.h"


void orderIn(void);
int orderExist(void);
int orderAtFloor(int floor);
int orderAbove(int floor);
int orderBelow(int floor);

int shouldStop(int floor, MotorDirection dir);

void clearOrders(int floor);
