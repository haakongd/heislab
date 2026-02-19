#include "start_up.h"
#include "elevio.h"

void start (void){
    
    while(1){
        
        int definert = elevio_floorSensor();
        if (definert == -1){
        
            elevio_motorDirection(DIRN_DOWN);

        }
        else{
            elevio_motorDirection(DIRN_STOP);
            break;
        
        }
    }
    int definert = elevio_floorSensor;
    if (0 > definert || 3 < definert){
        start();
    }    
        


}
