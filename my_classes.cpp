/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * version 2.0.0 beta1 (dd.mm.yyyy)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */


#include "my_classes.h"


Slider::Slider (SlowImpulses *stepper) {
  stepperInstance = *stepper;
  
  maxPosition = maxSteps;
  slideRunning = false;
  timeIntervalStart = 0;
  slideDir = 1;
}


void Slider::setParameters (int operationMode, int travelTime, int triggerInterval, int slidingDirection) {
  mode = operationMode;
  
  slideDir = !slidingDirection; // invert
  
  switch (mode) {
    case 1:  // TL-mode
      // TODO 2.1 - now: workaround by simply reusing the slide mode
      
    case 2:  // SL-mode
      // calculate speed (steps per interval)
      // maxSteps / travelTime [min] * 60 [sek/min] = stepsPerSecond
      stepsPerSecond = maxSteps / travelTime / 60;
      
      // TODO: limit frequency and correct other values
    
      // duration of one interval [ms] = triggerInterval [s] * 1000 [ms/s] - triggerDuration [ms]
      intervalDuration = triggerInterval * 1000 - triggerDuration;
    
      stepsPerInterval = intervalDuration / stepsPerSecond;
    
      if (stepsPerSecond > maxVelocity) {
        stepsPerSecond = maxVelocity;
      }
      
      break;
    case 3:  // CO-mode
      // calculate speed (steps per interval)
      // maxSteps / travelTime [min] * 60 [sek/min] = stepsPerSecond
      stepsPerSecond = maxSteps / travelTime / 60;
      
      intervalDuration = 0;
      
      stepsPerInterval = 0; // TODO: another solution for keeping track of the carriage position
      
      break;
    default:
      Serial.println("ERROR: Slider::setParameters - wrong mode");
      break;
  }
  
  Serial.println("---- configuration of the slide: ----");
  Serial.print("mode ");Serial.println(mode);
  Serial.print("stepsPerSecond ");Serial.println(stepsPerSecond);
  Serial.print("intervalDuration ");Serial.println(intervalDuration);
  Serial.print("stepsPerInterval ");Serial.println(stepsPerInterval);
  Serial.print("direction ");Serial.println(slideDir);
  
  
}

void Slider::startSlide () {
  digitalWrite(stepperSleep, HIGH);
  digitalWrite(stepperDir, slideDir);
  
  // NEW: with slow_impulses
  //boolean stepperNoError = stepperInstance.set(stepsPerSecond, intervalDuration);
  boolean stepperError = true; // debug
  stepperInstance.set(4,5000); // debug
  
  if (stepperNoError) {
    slideRunning = true;
    //stepperInstance.start(); // done by update on first
    
  }else {
    Serial.println("ERROR: slow_impulses::set - wrong frequency");
    slideRunning = false;
  }
  
  timeIntervalStart = 0; // TODO: is this used any more?
}

boolean Slider::update() {
  // enable slide an check for virtual endstop
  if (slideRunning == true && carriagePosition < maxPosition && carriagePosition >= 0 ) {
      
    if (slideFirstStart == 0) {
      slideFirstStart = 1;
      displaySymbol(slideMve);
      
      //stepperInstance.start();
      stepperInstance.set(1,5000);
      stepperInstance.start();
      
      Serial.println("first start");
    }else {
      
      int statusReturn = stepperInstance.getStatus();
      if ( statusReturn ) {
        // still running
        
        Serial.println(stepperInstance.durationCount); // debug

        
      }else {
        
        //carriagePosition += stepsPerInterval;
        
        stepperInstance.stop();
        digitalWrite(stepperStep, LOW);
        
        //displaySymbol(slideTrg);
        //camTrigger();
        Serial.println("trigger");
        delay(500);
        
        stepperInstance.start();
      
      }
        
    }
      
    return true;

  }else if (slideRunning == false) {
    Serial.println("ERROR: slide has not been started");
    return false;
  }else {
    Serial.println("ERROR: endstop hit");
    return false;
  }
}


void Slider::camTrigger () {
  Serial.println("trigger");
  displaySymbol(slideTrg);
  digitalWrite(trigger, HIGH);
  delay(triggerDuration);
  digitalWrite(trigger, LOW);
}





