#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sim.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//
// One Pump Gas Station Simulation
// Arriving vehicles use the gas pump; queue if pump being used
//
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//
// State variables  and other global information
//
/////////////////////////////////////////////////////////////////////////////////////////////

int	AtPump=0;	// #vehicles at the pump or waiting to use it; 0 if pump is free

// Event types
#define	ARRIVAL     1
#define	DEPARTURE   2


/////////////////////////////////////////////////////////////////////////////////////////////
//
// Data structures for event data
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Events only have one parameter, the event type
struct EventData {
	int EventType;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Function prototypes
//
/////////////////////////////////////////////////////////////////////////////////////////////

// prototypes for event handlers
void Arrival (struct EventData *e);		// car arrival event
void Departure (struct EventData *e);	// car departure event

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Event Handlers
// Parameter is a pointer to the data portion of the event
//
/////////////////////////////////////////////////////////////////////////////////////////////

// General Event Handler Procedure define in simulation engine interface
void EventHandler (void *data)
{
    struct EventData *d;
    
    // coerce type
    d = (struct EventData *) data;
    // call an event handler based on the type of event
    if (d->EventType == ARRIVAL) Arrival (d);
    else if (d->EventType == DEPARTURE) Departure (d);
    else {fprintf (stderr, "Illegal event found\n"); exit(1); }
}

// event handler for arrival events
void Arrival (struct EventData *e)
{
	struct EventData *d;
	double ts;

    printf ("Processing Arrival event at time %f, AtPump=%d\n", CurrentTime(), AtPump);
	if (e->EventType != ARRIVAL) {fprintf (stderr, "Unexpected event type\n"); exit(1);}

	// schedule next arrival event, here, 10 time units from now
    if((d=malloc(sizeof(struct EventData)))==NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    d->EventType = ARRIVAL;
    ts = CurrentTime() + 10.0;
    Schedule (ts, d);

    // if the pump is free, vehicle will use the pump for 15 time unts; schedule departure event
	if (AtPump == 0) {
		if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
		d->EventType = DEPARTURE;
		ts = CurrentTime() + 16.0;
		Schedule (ts, d);
	}
    AtPump++;   // update state variable
	free (e);	// don't forget to free storage for event!
}

// event handler for departure events
void Departure (struct EventData *e)
{
    struct EventData *d;
    double ts;
    
    printf ("Processing Departure event at time %f, AtPump=%d\n", CurrentTime(), AtPump);
	if (e->EventType != DEPARTURE) {fprintf (stderr, "Unexpected event type\n"); exit(1);}

	AtPump--;   // one fewer vehicle at pump

    // If another vehicle waiting to use pump, allocate pump to vehicle, schedule its departure event
    if (AtPump>0) {
        if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
        d->EventType = DEPARTURE;
        ts = CurrentTime() + 16.0;
        Schedule (ts, d);
    }
	free (e);	// release memory for data part of event
}

///////////////////////////////////////////////////////////////////////////////////////
//////////// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////

int main (void)
{
	struct EventData *d;
	double ts;

	// initialize event list with first arrival
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = ARRIVAL;
	ts = 10.0;
	Schedule (ts, d);

	RunSim(50.0);
}
