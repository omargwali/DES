//Standard IO, Standard C library, Math and Similation Header
#include <stdio.h>
#include <stdlib.h>
#include "sim.h"
#include <math.h>
/*

Data Structures: Include BinaryHeap file

*/

#include "heap.h"




// Simulation clock variable and BinaryHeap creation
BinaryHeap* priorityQueue = createHeap();
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

double CurrentTime (void) {
	return (Now);
}

void Schedule(double ts, void* data) {
	insert(priorityQueue, data, ts);
}

void RunSim(double EndTime) {
	while (priorityQueue->root != NULL) {
		Now = priorityQueue->root->key;
		if (Now > EndTime) break;
		Node* event = extract_min(priorityQueue);
		EventHandler(event->data);
		free(event);
	}
}

