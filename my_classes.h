/*
 * project: slider OS 2
 * my custom classes
 *
 * copyright: Jannik Beyerstedt | http://jannikbeyerstedt.de | code@jannikbeyerstedt.de
 * license: http://www.gnu.org/licenses/gpl-3.0.txt GPLv3 License
 *
 * version 2.1.0 (26.10.2014)
 */

/*
  library for all other classes and functions
  - Slider - all things that move the carriage + pure interval trigger (using another class)
  - IntervalTrigger - an interval trigger
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
  unsigned long mode3StartTime;   // helper for mode 3 current position calculation
  int mode3CarriageStartPos;      // helper for mode 3 current position calcualtion

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
