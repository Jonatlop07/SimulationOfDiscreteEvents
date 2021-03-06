#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../rand_gen/lcgrand.h"

#define Q_LIMIT 100
#define BUSY 1
#define IDLE 0

// Model settings variables
float meanInterarrival, meanService, timeEnd;

// Simulation clock
float simulationTime;

// State variables
int numInQueue, serverStatus;
float timeOfArrival[ Q_LIMIT + 1 ], timeOfNextEvent[ 4 ], timeOfLastEvent;

// Statistical counters
int numCustsDelayed;
float areaNumInQueue, areaServerStatus, totalOfDelays;

int nextEventType, numEvents;

FILE *inFile, *outFile;

void initialize ( void );
void timing ( void );
void arrive ( void );
void depart ( void );
void report ( void );
void updateTimeAvgStats ( void );
float exponentialDistribution ( float mean );

int main ( void ) {
   
   inFile  = fopen( "mm1.in",  "r" );
   outFile = fopen( "mm1.out", "w" );

   numEvents = 3;

   fscanf( inFile, "%f %f %f", &meanInterarrival, &meanService, &timeEnd );

   fprintf( outFile, "Single-server queueing system\n\n" );
   fprintf( outFile, "Mean interarrival time%11.3f minutes\n\n", meanInterarrival );
   fprintf( outFile, "Mean service time%16.3f minutes\n\n", meanService );
   fprintf( outFile, "Length of the simulation%9.3f minutes\n\n", timeEnd );
   
   initialize();

   do {
      timing();
      updateTimeAvgStats();

      switch ( nextEventType ) {
         case 1: 
            arrive(); 
            break; 
         case 2:
            depart();
            break;
         case 3:
            report();
            break;
      }
   } while ( nextEventType != 3 );

   fclose( inFile );
   fclose( outFile );

   return 0;
}

void initialize ( void ) {
   
   simulationTime  = 0.0;
   
   serverStatus    = IDLE;
   numInQueue      = 0.0;
   timeOfLastEvent = 0.0;

   numCustsDelayed  = 0;
   totalOfDelays    = 0;
   areaNumInQueue   = 0.0;
   areaServerStatus = 0.0;

   timeOfNextEvent[1] = simulationTime + exponentialDistribution( meanInterarrival );
   timeOfNextEvent[2] = 1.0e+30;   
   timeOfNextEvent[3] = timeEnd;
} 

void timing ( void ) {

   float minTimeOfNextEvent = 1.0e+29;
   
   nextEventType = 0;

   for ( int i = 1; i <= numEvents; ++i ) {
      if ( timeOfNextEvent[ i ] < minTimeOfNextEvent ) {

         minTimeOfNextEvent = timeOfNextEvent[ i ];
         nextEventType = i;
      }
   }

   if ( nextEventType == 0 ) {

      fprintf( outFile, "\nEvent list empty at time %f", simulationTime );
      exit( 1 );
   }

   simulationTime = minTimeOfNextEvent;
}

void arrive ( void ) {
   
   timeOfNextEvent[ 1 ] = simulationTime + exponentialDistribution( meanInterarrival );

   if ( serverStatus == BUSY ) {

      ++numInQueue;

      if ( numInQueue > Q_LIMIT )  {

         fprintf( outFile, "\nOverflow of the array timeOfArrival at");
         fprintf( outFile, "time %f", simulationTime );
         exit( 2 );
      }

      timeOfArrival[ numInQueue ] = simulationTime;

   } else {

      float delay    = 0.0;
      totalOfDelays += delay;

      ++numCustsDelayed;
      serverStatus = BUSY;

      timeOfNextEvent[ 2 ] = simulationTime + exponentialDistribution( meanService ); 
   }
}

void depart ( void ) {

   if ( numInQueue <= 0 ) {

      serverStatus = IDLE;
      timeOfNextEvent[ 2 ] = 1.0e+30;

   } else {

      --numInQueue;

      float delay    = simulationTime - timeOfArrival[ 1 ];
      totalOfDelays += delay;

      ++numCustsDelayed;
      timeOfNextEvent[ 2 ] = simulationTime + exponentialDistribution( meanService );

      for ( int i = 1; i <= numInQueue; ++i )
         timeOfArrival[ i ] = timeOfArrival[ i + 1 ];
   }
}

void report ( void ) {
   
   fprintf( outFile, "\n\nAverage delay in queue%11.3f minutes\n\n",
            totalOfDelays / numCustsDelayed );
   fprintf( outFile, "Average number in queue%10.3f\n\n",
            areaNumInQueue / simulationTime );
   fprintf( outFile, "Server utilization%15.3f\n\n",
            areaServerStatus / simulationTime );
   fprintf( outFile, "Number of delays completed%7d\n\n",
            numCustsDelayed );
}

void updateTimeAvgStats ( void ) {
   
   float timeSinceLastEvent = simulationTime - timeOfLastEvent;
   timeOfLastEvent = simulationTime;

   areaNumInQueue += numInQueue * timeSinceLastEvent;
   areaServerStatus += serverStatus * timeSinceLastEvent;
}

float exponentialDistribution ( float mean ) {
   return -mean * log( lcgrand( 1 ) );
}
