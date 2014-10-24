/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * version 2.0.2 (24.10.2014)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */

/*
  library for all other classes and functions
  - Slider
*/


#ifndef __objects_h__
#define __objects_h__


#include <Arduino.h>
#include "config.h"
#include "display.h"
#include "slow_impulses.h"


class IntervalTrigger {
  unsigned int interval;
  unsigned long startTime;
  unsigned int counter;
  boolean started;
  
public:
  IntervalTrigger();
  boolean set(int intervalDuration); // intervalDuration in ms
  void start();                   // start once, intervals will continue until stop()
  boolean getStatus();            // returns false if interval is done
  void stop();                    // stops execution and resets counter and enabler
  
};


class Slider {
  SlowImpulses *stepperInstance;
  IntervalTrigger *intTrigger;
  
  int mode;                       // operation mode: TL, SL, CO
  int carriagePosition;           // 0 is left position
  int maxPosition;
  int slideDir;                   // 1 = left, 0 = right (for pololu stepper driver)
  
  unsigned int stepsPerSecond;    // frequency for output
  unsigned long intervalDuration; // duration for output
  unsigned int stepsPerInterval;  // helping variable for carriagePosition

  boolean sequenceRunning;        // helper for error and misuse management
  
  
  void camTrigger();
  
  
  public:
    Slider (SlowImpulses *stepper);
  
    void initCarriagePosition(unsigned int position);
  
    //                  1 - 3              in sec          in sec               0 or 1
    void setParameters (int operationMode, int travelTime, int triggerInterval, int slidingDirection);
  
    // needs to be executed every cycle of the main loop
    boolean update (); // returns false if end is hit, so the slide needs to be aborted
  
    void startSequence();
    void stopSequence();
    
    void manualRight();
    void manualLeft();
};


#endif
