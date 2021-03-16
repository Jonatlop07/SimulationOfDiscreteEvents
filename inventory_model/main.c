#include <stdio.h>
#include <math.h>
#include "../rand_gen/lcgrand.h"

// Model settings variables
int initialInventoryLevel, numberOfMonths, numberOfValuesDemand;
float holdingCost, incrementalCost, maximumLag, meanInterdemand, minimumLag, setupCost, shortageCost;

int bigs, smalls;
float probDistribDemand[ 26 ];

int nextEventType, numberOfEvents;

FILE *inFile, *outFile;

void  initialize ( void );
void  timing ( void );
void  orderArrival ( void );
void  demand ( void );
void  evaluate ( void );
void  report ( void );
void  updateTimeAvgStats ( void );
float exponentialDistribution ( float mean );
int   randomInteger ( float probDistrib [] );
float uniformDistribution ( float a, float b );

int main () {

   int i, numberOfPolicies;
   
   inFile  = fopen( "inv.in", "r" );
   outFile = fopen( "inv.out", "w" );
   
   numberOfEvents = 4;

   fscanf( inFile, "%d %d %d %d %f %f %f %f %f %f %f",
           &initialInventoryLevel, &numberOfMonths, &numberOfPolicies, &numberOfValuesDemand,
           &meanInterdemand, &setupCost, &incrementalCost, &holdingCost, &shortageCost,
           &minimumLag, &maximumLag );
   
   for ( i = 1; i <= numberOfValuesDemand; ++i ) {
      fscanf( inFile, "%f", &probDistribDemand[ i ] );
   }

   fprintf( outFile, "Single-product inventory system\n\n" );
   fprintf( outFile, "Initial inventory level%24d items\n\n", initialInventoryLevel );
   fprintf( outFile, "Number of demand sizes%25d\n\n", numberOfValuesDemand );
   fprintf( outFile, "Distribution function of demand sizes  " );

   for ( i = 1; i <= numberOfValuesDemand; ++i ) {
      fprintf( outFile, "%8.3f", probDistribDemand[ i ] );
   }

   fprintf( outFile, "\n\nMean interdemand time %26.2f\n\n", meanInterdemand );
   fprintf( outFile, "Delivery lag range%29.2f to%10.2f months\n\n", minimumLag, maximumLag );
   fprintf( outFile, "Length of the simulation%23d months\n\n", numberOfMonths );
   fprintf( outFile, "K =%6.1f   i =%6.1f   h =%6.1f   pi =%6.1f\n\n",
            setupCost, incrementalCost, holdingCost, shortageCost );
   fprintf( outFile, "Number of policies%29d\n\n", numberOfPolicies );
   fprintf( outFile, "                 Average        Average" );
   fprintf( outFile, "        Average        Average\n" );
   fprintf( outFile, "  Policy       total cost    ordering cost" );
   fprintf( outFile, "  holding cost   shortage cost" );

   for ( i = 1; i <= numberOfPolicies; ++i ) {

      fscanf( inFile, "%d %d", &smalls, &bigs );
      initialize();

      do {
         timing();
         udpdateTimeAvgStats();

         switch ( nextEventType ) {
            case 1:
               orderArrival();
               break;
            case 2:
               demand();
               break;
            case 4:
               evaluate();
               break;
            case 3:
               report();
               break;
         }
      } while ( nextEventType != 3 );
   }

   fclose ( inFile );
   fclose ( outFile );

   return 0;
}
