#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sim.h"

// Two-Way Traffic Simulator

// STATE VARIABLES/GLOBAL INFORMATION
char state;     // state, can be set to north (n), south (s), or empty (e) 
int g_on = 0;   // number of cars in current group that are on the bridge
int g_off = 0;  // number of cars in the current group that have not yet entered the bridge
int wait_1 = 0; // number of cars in first waiting group (next group after g)
int wait_2 = 0; // number of cars in second waiting group (next group after f)
int car_num = 1;    // number of the car in its group

double min_wait_time; // minimum time between arrival of a vehicle at bridge until its group begins crossing the bridge
double max_wait_time; // maximum time between arrival of a vehicle at bridge until its group begins crossing the bridge
double avg_wait_time; // average time between arrival of a vehicle at bridge until its group begins crossing the bridge

double NB_InterArrivalTime = 5.00; // mean seconds between northbound car arrivals
double SB_InterArrivalTime = 20.0; // mean seconds between southbound car arrivals

double iat; //inter-arrival time variable
double* g_T; //pointer to traversing time of the group G

// Events have parameters:
// Event type (Arrival (1), Entry (2), Exit (3))
// Direction (North (n), South (s))
// Time of Arrival
// Number Car in Group
struct EventData {
    int EventType;
    char direction;
    double arrival_time;
    int car;
};

// FUNCTION PROTOTYPES
void EventHandler (void *data);
void Arrival (struct EventData *e); // Arrival at bridge
void Entry (struct EventData *e);   // Entry to bridge
void Exit (struct EventData *e);    // Exiting the bridge
double gen_rand(double min, double max);

// EVENT HANDLERS

/* General Event Handler */
void EventHandler (void *data) {

  struct EventData *d;
    
    // coerce type
    d = (struct EventData *) data;

    // call an event handler based on the type of event

    if (d->EventType == 1) {
      Arrival(d);
    } else if (d->EventType == 2) {
      Entry(d);
    } else if (d->EventType == 3) {
      Exit(d);
    } else {
      fprintf (stderr, "Illegal event found\n");
      exit(1);
    }
    free(d);
}

/* Arrival Event Handler */
void Arrival (struct EventData *e) {

  if (e->EventType != 1) {
    fprintf(stderr, "Unexpected event type\n");
    exit(1);
  } else {
    
// schedule another Arrival event IAT seconds in the future in the same direction
      struct EventData *f;
      double ts;
      
      if((f=malloc(sizeof(struct EventData)))==NULL) {
          fprintf(stderr, "malloc error\n");
          exit(1);
          
      } else {
          if (e->direction == 'n') {
              iat = randexp(NB_InterArrivalTime);
          } else if (e->direction == 's') {
              iat = randexp(SB_InterArrivalTime);
          }
          
          f->EventType = 1;
          f->direction = e->direction;
          
          ts = CurrentTime() + iat;
          f->arrival_time = ts;
          f->car = car_num++;

          Schedule(ts, f);
          
          printf("\nCar %d Arrival Scheduled at Time t = %f\n", f->car, ts);
          
      }

// if the state is empty at Arrival
      if (state == 'e') {
          state = e->direction;                             // change the state to the direction of the Arrival
          g_off++;                                          // add the car to the current group

    // immediately schedule Entry into the bridge
          struct EventData *d;
          double ts;
          
          if((d=malloc(sizeof(struct EventData)))==NULL) {
              fprintf(stderr, "malloc error\n");
              exit(1);
              
          } else {
              ts = CurrentTime();
              d->EventType = 2;
              d->direction = e->direction;
              d->arrival_time = e->arrival_time;
              d->car = e->car;
              
              Schedule(ts, d);
              printf("\nCar %d Entry Scheduled at Time t = %f\n", d->car, ts);
              
          }
    
// if the state is the same as the direction of the Arrival event
      } else if (state == e->direction) {
    
// if there are no cars remaining off the bridge but cars on the bridge
          if (g_off == 0 && g_on > 0) {
              wait_2++;                                       // car added to second waiting group (same-side)
          }

// if there is at least 1 car not already traveling on the bridge in the direction of the Arrival event
          else if (g_off > 0) {
              g_off++;                                        // add the car to the current group
      
      // schedule an Entry event in the future
              struct EventData *d;
              double ts;
              
              ts = CurrentTime() + gen_rand(2.5, 3.5);        // add time delay
              
              if((d=malloc(sizeof(struct EventData)))==NULL) {
                  fprintf(stderr, "malloc error\n");
                  exit(1);
                  
              } else {
                  d->EventType = 2;
                  d->direction = e->direction;
                  d->arrival_time = e->arrival_time;
                  d->car = e->car;
                  
                  Schedule(ts, d);
                  printf("\nCar %d Entry Scheduled at Time t = %f\n", ts);
                  
              }
              
          }

// if the state is the opposite as the direction of the Arrival event
          
      } else if (state != (e->direction) && state != 'e') {
          wait_1++;                                        // add car to waiting group (opposite-side)
          
      }
      printf("\nProcessed Arrival at time %f, State = %c, Group Size = %d, On = %d, Off = %d, Waiting 1 = %d, Waiting 2 = %d\n\n", CurrentTime(), state, g_on+g_off, g_on, g_off, wait_1, wait_2);
      
  }
    
}

/* Entry Event Handler */
void Entry (struct EventData *e) {
    
    if (e->EventType != 2) {
        fprintf(stderr, "Unexpected event type \n");
        exit(1);
        
    } else {
        g_on++;                                         // increment the number of cars in the group on the bridge
        g_off--;                                        // decrement the number of cars in the group off the bridge
    
      // schedule an Exit event T seconds in the future
        struct EventData *d;
        double ts;
        
        if((d=malloc(sizeof(struct EventData)))==NULL) {
            fprintf(stderr, "malloc error\n");
            exit(1);
            
        } else {
            double r = gen_rand(40, 45);
            
            g_T = &r;
            ts = CurrentTime() + *g_T;
            
            d->EventType = 3;
            d->direction = e->direction;
            d->arrival_time = e->arrival_time;
            d->car = e->car;
          
            Schedule(ts, d);
            printf("\nExit Scheduled at Time t = %f\n", ts);
            
        }
        
    }
    printf("\nProcessed Entry at time %f, State = %c, Group Size = %d, On = %d, Off = %d, Waiting 1 = %d, Waiting 2 = %d\n\n", CurrentTime(), state, g_on + g_off, g_on, g_off, wait_1, wait_2);
    
}

/* Exit Event */

/*
The Exit Event is generating the trouble for me. I am trying to Schedule the Entry events of the waiting group in the Exit Event, but I don't know how to calculate the wait time because the wait time is calculated as the time from when the car arrives at the bridge until the time the car's group begins crossing the bridge. In the case of the cars that must wait in the queue, I don't know how to get each car's initial arrival time in the Entry Event that is scheduled by the Exit Event. This is the scheduling that happens in lines 280-261 and 295-312.
 */

void Exit (struct EventData *e) {
    
    if ((e->EventType) != 3) {
        fprintf(stderr, "Unexpected event type \n");
        exit(1);
        
    } else {
   //decrement the number of cars on the bridge
        g_on--;

// if there are no cars in the current traveling group and no cars in the waiting group
        if( (g_on == 0) && (g_off == 0) && wait_1 == 0 && wait_2 == 0) {
            state = 'e';                                // set the state to empty
            
        }
  
// if there are no cars in the current traveling group but nonzero cars in the first waiting group (opposite side)
        else if ( (g_on == 0) && (g_off == 0) && (wait_1 > 0)) {

// set the state to the direction of the cars in the first waiting group (opposite side)

     // flip the state
            if (state == 'n') {
                state = 's';
                
            } else if (state == 's') {
                state = 'n';
            }
     
    // schedule Entry event for each car in first waiting group (opposite side)
            struct EventData *d;
            double ts;
            
            double t_0 = CurrentTime();
            
            for (int i = 0; i < wait_1; i++) {
                
                if((d=malloc(sizeof(struct EventData)))==NULL) {
                    fprintf(stderr, "malloc error\n");
                    exit(1);
                    
                } else {
                    d->EventType = 2;
                    if (state == 'n') {
                        d->direction = 's';
                        
                    } else if (state == 's') {
                        d->direction = 'n';
                        
                    }
                    ts = t_0 + gen_rand(2.5, 3.5);
                    Schedule(ts, d);
                    printf("\nEntry Scheduled at Time t = %f\n", ts);
                    
                    t_0 = ts;
                    
                }
                
            }
            g_off = wait_1;
            wait_1 = wait_2;
            wait_2 = 0;
   }
  
// if there are cars on the same side waiting to cross
        else if ( (g_on == 0) && (g_off == 0) && (wait_1 == 0) && (wait_2 > 0)) {
       // schedule Entry event for each car in second waiting group
            struct EventData *d;
            double ts;
            
            for (int i = 0; i < wait_2; i++) {
                double t_0 = CurrentTime();
                
                if((d=malloc(sizeof(struct EventData)))==NULL) {
                    fprintf(stderr, "malloc error\n");
                    exit(1);
                    
                } else {
                    d->EventType = 2;
                    d->direction = state;
                    ts = t_0 + gen_rand(2.5, 3.5);

                    Schedule(ts, d);
                    printf("\nEntry Scheduled at Time t = %f\n", ts);

                    t_0 = ts;
                    
                }
          
                g_off = wait_2;
                wait_2 = 0;
            }
            printf("\nProcessed Exit at time %f, State = %c, Group Size = %d, On = %d, Off = %d, Waiting 1 = %d, Waiting 2 = %d\n\n", CurrentTime(), state, (g_on + g_off), g_on, g_off, wait_1, wait_2);
            
        }
        
    }
    
}

// HELPER FUNCTIONS
double gen_rand(double min, double max) {
    double num = ((double) rand()/RAND_MAX)*(max-min) + min;
    return num;
}
    
// MAIN PROGRAM

int main (void) {
    setbuf(stdout, NULL);
  //declare an initial stucture and time variable
    struct EventData *d;
    double ts;

	// initialize event list with first arrival
    if ((d=malloc(sizeof(struct EventData))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
        
    }
    srand(time(NULL));
    state = 'e';
    
    d->EventType = 1;
    d->direction = 'n';
    
    ts = 0.0;
    d->arrival_time = ts;
    d->car = car_num++;

    Schedule (ts, d);
    printf("\nArrival Scheduled at Time t = %f\n", ts);
    
    RunSim(100.0);
}
