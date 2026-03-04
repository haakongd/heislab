#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "driver/elevio.h"
#include "driver/start_up.h"
#include "driver/bestillinger.h"

int main(){
    elevio_init(); 
    start(); // Init heis

    printf("=== Example Program ===\n");

    MotorDirection dir = DIRN_STOP;
    int lastFloor = elevio_floorSensor();
    if(lastFloor == -1) {
        lastFloor = 0;
    }

    int doorOpen = 0;
    time_t doorCloseTime = 0;
    const int DOOR_OPEN_SECONDS = 3;

    elevio_motorDirection(DIRN_STOP);

     while(1){ 
        int floor = elevio_floorSensor();
        if(floor != -1) lastFloor = floor;
        elevio_floorIndicator(lastFloor);

        orderIn(); // H1

        elevio_stopLamp(elevio_stopButton());
        
       if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);

            int floor = elevio_floorSensor();

            if(floor != -1){
                elevio_doorOpenLamp(1);
                doorOpen = 1;
            }

            while(elevio_stopButton()){
                usleep(20 * 1000);
            }

            doorCloseTime = time(NULL) + DOOR_OPEN_SECONDS;
        }else{
            elevio_stopLamp(0);
        }
    
        if (doorOpen) {
            elevio_motorDirection(DIRN_STOP);

            if (elevio_obstruction()) {
        
            } else {
        
            if (doorCloseTime == 0) {
                doorCloseTime = time(NULL) + DOOR_OPEN_SECONDS;
            }

        
            if (time(NULL) >= doorCloseTime) {
                doorOpen = 0;
                doorCloseTime = 0;
                elevio_doorOpenLamp(0);
            }
        }

        usleep(20*1000);
        continue;
    }

        int refFloor = (floor == -1) ? lastFloor : floor;


    
        if(orderExist()){

            if(dir ==  DIRN_STOP) {
                if(orderAbove(refFloor)){
                     dir = DIRN_UP;
                }
                else if (orderBelow(refFloor)) {
                dir = DIRN_DOWN;
                }
                else {
                    dir = DIRN_STOP;
                }
            }    

            if(floor != -1 && shouldStop(floor, dir)){
                dir = DIRN_STOP;
                elevio_motorDirection(DIRN_STOP);

                clearOrders(floor);

                doorOpen = 1;
                doorCloseTime = time(NULL) + DOOR_OPEN_SECONDS;
                elevio_doorOpenLamp(1);

                usleep(20 * 1000);
                continue;

            }

            if(dir == DIRN_UP && !orderAbove(refFloor)) {
                if(orderBelow(refFloor)) dir = DIRN_DOWN;
                else dir = DIRN_STOP; //H4
            }
            else if(dir == DIRN_DOWN && !orderBelow(refFloor)) {
                if(orderAbove(refFloor)) dir = DIRN_UP;
                else dir = DIRN_STOP; //H4
            }

        } else {
            dir = DIRN_STOP;
        }

        elevio_motorDirection(dir);

        usleep(20*1000);

        //nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
        
    } 

    return 0;
}


