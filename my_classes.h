/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * version 2.0.0 beta1 (dd.mm.yyyy)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */

/*
  library for all other classes and functions
*/


#ifndef __objects_h__
#define __objects_h__


#include <Arduino.h>
#include "config.h"
#include "display.h"


class Slider {
  int carriagePosition;
  int maxPosition;
  int slideDir;
  
  int slideStepsPerInterval;
  int slideStepDelay;
  
  unsigned int stepsPerSecond;
  unsigned int stepsPerInterval; // helping variable for carriagePosition
  unsigned long intervalDuration;
  unsigned long timeIntervalStart;
  
  boolean slideRunning;

  void camTrigger();
  void doSlide();
  
  public:
    Slider ();
    void setParameters (int travelTime, int triggerInterval, int slidingDirection);
    void setParametersOld (int travelTime, int triggerInterval);
    
    boolean update (); // returns false if end is hit, so the slide needs to be aborted
    void startSlide();
    
    
    boolean doSteps (int dir); // returns false if end is hit, so the slide needs to be aborted
    
    
};


#endif
