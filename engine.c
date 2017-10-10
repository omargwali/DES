//Standard IO, Standard C library, Math and Similation Header
#include <stdio.h>
#include <stdlib.h>
#include "sim.h"
#include <math.h>
/*

Data Structures: Include BinaryHeap file

*/

#include "BinaryHeap.h"

typedef struct Event {
	double timestamp;		// event timestamp
    void *AppData;			// pointer to event parameters
	struct Event *Next;		// priority queue pointer
} Event;

// Simulation clock variable and Event Root Node
Event FEl = NULL;
double Now = 0.0;

// Uniformly distributed random value between 0 and 1
double urand(void) {
	double x = ((double) rand() / (double) RAND_MAX);
	return x;
}

double randexp(double mean) {
	double x = -1 * mean * (log(1.0 - urand()));
	return x;
}



