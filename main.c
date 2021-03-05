#include <stdio.h>
#include <math.h>
#include "lcgrand.h"

#define Q_LIMIT 100
#define BUSY 1
#define IDLE 0

// Model settings variables
float meanInterarrival, meanService, numDelaysRequired;

// Simulation clock
float simulationTime;

// State variables
int numInQueue, serverStatus;
float timeOfArrival[ Q_LIMIT + 1 ], timeOfNextEvent[ 3 ], timeOfLastEvent;

// Statistical counters
int numCustsDelayed;
float areaNumInQueue, areaServerStatus, totaOfDelays;

int nextEventType, numEvents;

FILE *inFile, *outFile;

void initialize ( void );
void timing ( void );
void arrive ( void );
void depart ( void );
void report ( void );
void updateTimeAvgStats ( void );
float exponential ( float mean );

int main ( ) {
   
   inFile  = fopen( "mm1.in",  "r" );
   outFile = fopen( "mm1.out", "w" );

   numEvents = 2;

   fscanf( inFile, "%f %f %d", &meanInterarrival, &meanService, &numDelaysRequired );

   fprintf( outFile, "Single-server queueing system\n\n" );
   fprintf( outFile, "Mean interarrival time%11.3f minutes\n\n", meanInterarrival );
   fprintf( outFile, "Mean service time%16.3f minutes\n\n", meanService );
   fprintf( outFile, "Number of customers%14d\n\n", numDelaysRequired );
   
   initialize();

   while ( numCustsDelayed < numDelaysRequired ) {

      timing();
      updateTimeAvgStats();

      switch ( nextEventType ) {
         case 1: 
            arrive(); 
            break; 
         case 2:
            depart();
            break;
      }
   }

   report();

   fclose( inFile );
   fclose( outFile );

   return 0;
}

void initialize ( void ) {
   
   simulationTime  = 0.0;
   
   serverStatus    = IDLE;
   numInQueue      = 0.0;
   timeOfLastEvent = 0.0;

   timeOfNextEvent[1] = simulationTime + exponential( meanInterarrival );
   timeOfNextEvent[2] = 1.0e+30;   
} 

void timing ( void ) {

   float minTimeOfNextEvent = 1.0e+29;
   
   nextEventType = 0;

   for ( int i = 1; i <= numEvents; ++i ) {
      if ( timeOfNextEvent[ i ] < minTimeOfNextEvent ) {
         minTimeOfNextEvent = timeOfNextEvent[i];
         nextEventType = i;
      }
   }

   if ( nextEventType == 0 ) {
      fprintf( outFile, "\nEvent list empty at time %f", simulationTime );
      return -1;
   }

   simulationTime = minTimeOfNextEvent;
}

