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
    printf("Press the stop button on the elevator panel to exit\n");

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
            elevio_doorOpenLamp(0);
            break;
        }

        if(doorOpen){
            elevio_motorDirection(DIRN_STOP);

            if(time(NULL) >= doorCloseTime){
                doorOpen = 0;
                elevio_doorOpenLamp(0);
            }

            usleep(20 * 1000);
            continue;
        }

        int refFloor = (floor == -1) ? lastFloor : floor;
        /*
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed); // L1, L2
            }
        }

        if(floor != -1) lastFloor = floor;
        elevio_floorIndicator(lastFloor); // L3,L4,L5
        */
        if(orderExist()){

            if(dir == DIRN_STOP) {
                if(orderAbove(refFloor)) dir = DIRN_UP;
                else if (orderBelow(refFloor)) dir = DIRN_DOWN;
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


/*
int main(){
    elevio_init();
    start();

    MotorDirection dir = DIRN_STOP;

    int lastFloor = elevio_floorSensor();
    if(lastFloor == -1) lastFloor = 0;

    // "Door/hold"-tilstand
    int doorOpen = 0;
    time_t doorCloseTime = 0;
    const int DOOR_OPEN_SECONDS = 3;

    elevio_motorDirection(DIRN_STOP);
    elevio_doorOpenLamp(0);

    while(1){
        int floor = elevio_floorSensor();
        if(floor != -1) lastFloor = floor;
        elevio_floorIndicator(lastFloor);

        // Les inn ordre (lagrer i orders[][])
        orderIn();

        // Stop-knapp
        elevio_stopLamp(elevio_stopButton());
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            elevio_doorOpenLamp(0);
            break;
        }

        // Hvis vi har "dør åpen/hold": ikke velg retning, bare vent
        if(doorOpen){
            elevio_motorDirection(DIRN_STOP);

            if(time(NULL) >= doorCloseTime){
                doorOpen = 0;
                elevio_doorOpenLamp(0);
            }

            usleep(20 * 1000);
            continue;
        }

        int refFloor = (floor == -1) ? lastFloor : floor;

        if(orderExist()){

            // Hvis vi står stille: velg retning basert på refFloor
            if(dir == DIRN_STOP){
                if(orderAbove(refFloor)) dir = DIRN_UP;
                else if(orderBelow(refFloor)) dir = DIRN_DOWN;
                else {
                    // ordre finnes, men kun "her" (samme etasje) -> håndteres når vi er i etasje
                    dir = DIRN_STOP;
                }
            }

            // Stopp-beslutning KUN når vi faktisk er i etasje
            if(floor != -1 && shouldStop(floor, dir)){
                dir = DIRN_STOP;
                elevio_motorDirection(DIRN_STOP);

                clearOrders(floor);

                // Åpne dør og hold
                doorOpen = 1;
                doorCloseTime = time(NULL) + DOOR_OPEN_SECONDS;
                elevio_doorOpenLamp(1);

                usleep(20 * 1000);
                continue;
            }

            // Oppdater retning hvis vi “går tom” i den retningen
            if(dir == DIRN_UP && !orderAbove(refFloor)){
                if(orderBelow(refFloor)) dir = DIRN_DOWN;
                else dir = DIRN_STOP;
            }
            else if(dir == DIRN_DOWN && !orderBelow(refFloor)){
                if(orderAbove(refFloor)) dir = DIRN_UP;
                else dir = DIRN_STOP;
            }

        } else {
            dir = DIRN_STOP;
        }

        elevio_motorDirection(dir);

        usleep(20 * 1000);
    }

    return 0;
}
*/