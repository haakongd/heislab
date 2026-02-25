#include "bestillinger.h"
#include "elevio.h"

int orders [N_FLOORS][N_BUTTONS];

void orderIn(void){
    for (int floor = 0; floor < N_FLOORS; floor++){
        for (int button = 0; button < N_BUTTONS; button++){
            if (elevio_callButton(floor, button)){
                orders[floor][button] = 1;
            }
        }
    }
}


int orderExist(void){
    for (int floor = 0; floor < N_FLOORS; floor++){
        for (int button = 0; button < N_BUTTONS; button++){
            if(orders[floor][button]){
                return 1;
            }
        }

    }
    return 0;
}


int orderAtFloor(int floor){
    for (int button = 0; button < N_BUTTONS; button++){
        if(orders[floor][button]){
            return 1;
         }
    }
    return 0;
}

int orderAbove(int floor){
    for(int f = floor + 1; f < N_FLOORS; f ++){
            for (int button = 0; button < N_BUTTONS; button++){
                if(orders[f][button]){
                    return 1;
                }
            }
    }
    return 0;
}

int orderBelow(int floor){
    for (int f = 0; f < floor; f ++){
        for(int button = 0; button < N_BUTTONS; button ++){
            if(orders[f][button]){
                return 1;
            }
        }
    }
     return 0;
}


int shouldStop(int floor, Direction dir){

    if (orders[floor][BUTTON_CAB]){
        return 1;
    }

    if (dir == DIR_UP){
        if (orders[floor][BUTTON_HALL_UP]){
            return 1;

        }
        if (!orderAbove(floor) && orders[floor][BUTTON_HALL_DOWN]){
        return 1;
        } 
    }
    
    if (dir == DIR_DOWN){
        if (orders[floor][BUTTON_HALL_DOWN]){
            return 1;
        if (!orderBelow(floor) && orders[floor][BUTTON_HALL_UP]){
            return 1;
        }
        }
    }
    if (dir == DIR_STOP){
        if (orderExist()){
            return 1;

        }
    }
    
    return 0;
}

void clearOrders(int floor){
    for (int button = 0; button < N_BUTTONS; button ++){
        orders[floor][button] = 0;
    }
    
}