/* 
 * slow impulses library
 * generates low frequency rectangular signals over a given time
 * for driving stepper motor drivers like Pololu A4988
 *
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 * v1.0 (05.10.2014)
 */
 
#ifndef __SLOW_IMPULSES__
#define __SLOW_IMPULSES__

#include <Arduino.h>
 
const int PRESCALER    = 1024;
const long CPU_FREQU   = 16000000;
const int FUNCT_PERIOD = 2;         // number on interrupts your function uses to do one period

class SlowImpulses {
  long timerCompareVal;
  boolean no_error;
  
  public: boolean durationStatus;        // false if duration is over, true if not
  public: unsigned int durationCount;    // counts the number of interrupts
  public: long durationCompare;          // max number of interrupts (based on duration)
  public: int pinNo;
  
  public:
    SlowImpulses ();
    void init(int pin);  // use in setup loop
    boolean set(unsigned int frequency, unsigned int duration); // returns false if frequency is not valid
    void start();        // use set first, then start. Starts a new duration cycle
    void stop();         // disables interrupt, does not reset frequency and duration
    boolean getStatus(); // returns true if duration is not due, false after duration over
        
};

// timer compare interrupt service routine
// must be in main file / can´t figure out how to do it in the impulse.h or impule.cpp
//ISR(TIMER1_COMPA_vect)
//{ 
//  if (stepper.durationCount <= stepper.durationCompare ) {
//    stepper.durationCount++;
//    digitalWrite(stepper.pinNo, digitalRead(stepper.pinNo) ^ 1);
//  }else {
//    stepper.durationStatus = false;
//    stepper.stop();
//  }
//}
 
#endif
