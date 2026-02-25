#include "start_up.h"
#include "elevio.h"
#include <time.h>


void start(void){
    while(elevio_floorSensor() == -1){
        elevio_motorDirection(DIRN_DOWN);
    }

    elevio_motorDirection(DIRN_STOP);
}

