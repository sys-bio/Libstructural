#ifndef STOPWATCH_H
#define	STOPWATCH_H

#include <time.h>

/**
 * This class is used for timing the code.
 */
class Stopwatch {
public:
   // Elapsed time
   double elapsedTime;

   /**
    * Constructor. It resets the stopwatch.
    */
   Stopwatch() {
      reset();
   }

   ~Stopwatch() {
   }

   /**
    * Resets the stopwatch.
    */
   void reset() {
      elapsedTime = 0;
   }

   /**
    * Resets and starts the stopwatch.
    */
   void restart() {
      reset();
      start();
   }

   /**
    * Starts the stopwatch.
    */
   void start() {
      startTime = clock();
   }

   /**
    * Stops the stopwatch.
    */
   void stop() {
      endTime = clock();
      elapsedTime += ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
   }

private:
   //Starting time
   clock_t startTime;
   //Ending time
   clock_t endTime;

};

#endif	/* STOPWATCH_H */
