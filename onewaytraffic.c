#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sim.h"

// One-Way Traffic Simulator

// STATE VARIABLES/GLOBAL INFORMATION
char state; // state, can be set to north (n), south (s), or empty (e) 
int current_group = 0;// number of cars in current group
int waiting_group = 0;// number of cars in waiting group

double* T; //pointer to group traversing time

// Events have parameters:
// *Event type (Entry (1), Exit (0))
// *Direction (North (n), South (s))
struct EventData {
  int EventType;
  char direction;
};

// FUNCTION PROTOTYPES
void EventHandler (void *data);
void Entry (struct EventData *e);
void Exit (struct EventData *e);
double gen_rand(double min, double max);

// EVENT HANDLERS

/* General Event Handler */
void EventHandler (void *data) {

  struct EventData *d;
    
    // coerce type
    d = (struct EventData *) data;

    // call an event handler based on the type of event
    if (d->EventType == 1) {
      Entry(d);
    } else if (d->EventType == 0) {
      Exit(d);
    } else {
      fprintf (stderr, "Illegal event found\n");
      exit(1);
    }
}

/* Entry Event Handler */
void Entry (struct EventData *e) {

  if (e->EventType != 1) {
    fprintf(stderr, "Unexpected event type \n");
    exit(1);
  }
  
// if the state is empty
  if (state == 'e') {
    state = e->direction; //     change the state to the direction of the entry

    //     schedule a Departure event T seconds in the future
    struct EventData *d;
    double ts;
  
    if((d=malloc(sizeof(struct EventData)))==NULL) {
      fprintf(stderr, "malloc error\n");
      exit(1);
    }

    double r = gen_rand(40, 45);
    T = &r;
    ts = CurrentTime() + *T;
    d->EventType = 0;
    d->direction = e->direction;

    Schedule(ts, d);

    current_group++; // update state variable
  }
    
// if the state is the same as the direction of the entry event
  else if (state == e->direction) {
    
//    if there are no cars remaining in the current group (all cars are already traveling on the bridge)
    if (current_group == 0) {
      waiting_group++;                                //            increment the number of cars waiting
    }

//    if there is at least 1 car not already traveling on the bridge in the direction of the entry event
    else if (current_group > 0) {
      current_group++;                                //            increment the number of cars in the current traveling group

      //            schedule a departure event in the future
      struct EventData *d;
      double ts;

      ts = CurrentTime() + *T + gen_rand(2.5, 3.5);  //            get the traversing time and add time delay

      if((d=malloc(sizeof(struct EventData)))==NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
      }

      d->EventType = 0;
      d->direction = e->direction;

      Schedule(ts, d);

      current_group++; // update state variable
    }

// if the state is the opposite as the direction of the entry event
  } else if (state != (e->direction) && state != 'e') {
    waiting_group++;                                 //     increment the waiting group
  }

  free(e); // free memory for event

  printf("Processed Entry at time %f, State = %c, Group Size = %d, Waiting = %d\n", CurrentTime(), state, current_group, waiting_group);
}

/* Exit Event */
void Exit (struct EventData *e) {
  
 if ((e->EventType) != 0) {
    fprintf(stderr, "Unexpected event type \n");
    exit(1);
  }

// if there are no cars in the current traveling group and no cars in the waiting group
  if (current_group == 0 && waiting_group == 0) {
    state = 'e';                   //     set the state to empty
  }
  
// if there are no cars in the current traveling group but nonzero cars in the waiting group
  else if (current_group == 0 && waiting_group > 0) {

// set the state to the direction of the cars in the waiting group (2-way implementation)
    state = 'n';
    current_group = waiting_group; //     set the number of cars in the new current traveling group to the number of cars in the waiting group
    waiting_group = 0;             //     set the number of cars in the waiting group to be zero
    
    // schedule Entry event for each car in current group
    struct EventData *d;
    double ts;
    
    for (int i = 0; i < current_group; i++) {
      d->EventType = 1;
      d->direction = 'n';
      ts = CurrentTime() + *T + gen_rand(2.5, 3.5);
      
      Schedule(ts, d);
    }
    
  }
  
// if there ARE cars in the current traveling group
  else if (current_group > 0 && waiting_group == 0) {
      current_group--;             //     decrement the number of cars in the current traveling group
    }

    free(e); //free memory for event

    printf("Processed Exit at time %f, State = %c, Group Size = %d, Waiting = %d\n", CurrentTime(), state, current_group, waiting_group);
}

// HELPER FUNCTIONS
double gen_rand(double min, double max) {
  double num;
  num = ((double) rand()/RAND_MAX)*(max-min) + min;
  return num;
}
    
// MAIN PROGRAM

int main (void)
{
  setbuf(stdout, NULL);
  //declare an initial stucture and time variable
	struct EventData *d;
	double ts;

	// initialize event list with first entry
        if ((d=malloc(sizeof(struct EventData))) == NULL) {
            fprintf(stderr, "malloc error\n");
            exit(1);
          }

        srand(time(NULL));
        state = 'e';
        
        d->EventType = 1;
        d->direction = 'n';
	ts = 10.0;
	Schedule (ts, d);

	RunSim(500.0);
}
