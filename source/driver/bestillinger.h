#pragma once


void orderIn(void);
int orderExist(void);
int orderAtFloor(int floor);
int orderAbove(int floor);
int orderBelow(int floor);


typedef enum {
    DIR_DOWN = -1,
    DIR_STOP = 0,
    DIR_UP   = 1
} Direction;

int shouldStop(int floor, Direction dir);

void clearOrders(void);