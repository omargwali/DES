#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sim.h"

// One-Way Traffic Simulator

// STATE VARIABLES/GLOBAL INFORMATION
// state
// number of cars in current group
// number of cars in waiting group


// Event types

// Events have parameters:
// *Event type
// *Time created
// *Direction (1-North, 2-South)
// *Traversing time


// FUNCTION PROTOTYPES


// EVENT HANDLERS


/* Entry Event */

// if the state is empty, change the state to the direction of the entry and schedule a Departure event T seconds in the future

// if the state is the same as the direction of the entry event
//     check if there is a current group of traveling cars

//            if there are no cars remaining in the current group (all cars are already traveling on the bridge)
//                 increment the number of cars waiting

//            if there is at least 1 car not already traveling on the bridge in the direction of the entry event
//                 increment the number of cars in the current traveling group
//                 get the traversing time and add time delay
//                 schedule a departure event in the future

// if the state is the opposite as the direction of the entry event
//     increment the waiting group

/* Exit Event */

// if there are no cars in the current traveling group and no cars in the waiting group
//     set the state to empty

// if there are no cars in the current traveling group but nonzero cars in the waiting group
//     set the state to the direction of the cars in the waiting group
//     set the number of cars in the new current traveling group to the number of cars in the waiting group
//     set the number of cars in the waiting group to be zero

// if there ARE cars in the current traveling group and no cars in the waiting group
//     decrement the number of cars in the current traveling group


// MAIN PROGRAM

int main (void)
{
  //declare an initial stucture and time variable
	struct EventData *d;
	double ts;

	// initialize event list with first arrival

}
