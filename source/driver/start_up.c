#include "start_up.h"
#include "elevio.h"
#include <time.h>


void start(void){
    while(elevio_floorSensor() == -1){
        elevio_motorDirection(DIRN_DOWN);

        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    elevio_motorDirection(DIRN_STOP);
}

