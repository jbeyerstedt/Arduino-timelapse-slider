/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * version 2.0.0 beta1 (19.10.2014)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */


#include "my_classes.h"


Slider::Slider (SlowImpulses *stepper) {
  stepperInstance = stepper;
  
  maxPosition = maxSteps;
  slideRunning = false;
  slideDir = 1;
}


void Slider::initCarriagePosition(unsigned int pos) {
  carriagePosition = pos;
}


void Slider::setParameters (int operationMode, int travelTime, int triggerInterval, int slidingDirection) {
  mode = operationMode;
  
  #ifdef INVERT_STEPPER_DIR
    slideDir = !slidingDirection; // invert if necessary
  #else
    slideDir = slidingDirection;
  #endif
  
  switch (mode) {
    case 1:  // TL-mode
      // TODO: write a function with millis() to do camTrigger() each interval.
      
    case 2:  // SL-mode
      // calculate speed (steps per interval)
      // maxSteps / travelTime [min] * 60 [sek/min] = stepsPerSecond
      stepsPerSecond = maxSteps / travelTime / 60;
          
      // duration of one interval [ms] = triggerInterval [s] * 1000 [ms/s] - triggerDuration [ms]
      intervalDuration = triggerInterval * 1000 - triggerDuration;
      
      if (stepsPerSecond > maxVelocity) {
        stepsPerSecond = maxVelocity;
      }
      
      stepsPerInterval = intervalDuration * stepsPerSecond / 1000;
      
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
  
  
  Serial.println("\n---- configuration of the slide: ----");
  Serial.print("mode             ");Serial.println(mode);
  Serial.print("stepsPerSecond   ");Serial.println(stepsPerSecond);
  Serial.print("intervalDuration ");Serial.println(intervalDuration);
  Serial.print("stepsPerInterval ");Serial.println(stepsPerInterval);
  Serial.print("direction        ");Serial.println(slideDir);
  Serial.println("----- end config slide ----\n");
  
}

void Slider::startSequence () {
  int nextPosition = 0;
  
  switch (mode) {
    case 1:  // TL-mode
      // TODO: write a function with millis() to do camTrigger() each interval.
      
      break;
    
    case 2:  // SL-mode
      digitalWrite(stepperSleep, HIGH);
      digitalWrite(stepperDir, slideDir);
      
      if ( stepperInstance->set(stepsPerSecond, intervalDuration) ) {
        
        // workaround for bad position tracking
//        if (carriagePosition < 0) {
//          carriagePosition = 0;
//        }else if (carriagePosition > maxPosition) {
//          carriagePosition = maxPosition;
//        }
        
        // check position: calculate next position
        switch (slideDir) {
          case 0: // right +
            nextPosition = carriagePosition + stepsPerInterval;
            break;
          case 1: // left -
            nextPosition = carriagePosition - stepsPerInterval;
            break;
          default:
            Serial.println("ERROR: Slider::startSequence - wrong slideDir");
            break;
        }
        
        // check position: enough space for one sequence ?
        if ( (nextPosition >= 0) && (nextPosition <= maxPosition) ) {
          slideRunning = true;
          
          carriagePosition = nextPosition;
          stepperInstance->start();
          displaySymbol(slideMve);
          Serial.print("---- current position: "); Serial.println(carriagePosition); // debug
        }else {
          slideRunning = false;
          Serial.println("ERROR: virtual endstop hit - slide aborted");
        }
        
      }else {
        Serial.println("ERROR: slow_impulses::set - wrong frequency");
        slideRunning = false;
      }
      
      break;
    
    case 3:  // CO-mode
      
      break;
    
    default:
      Serial.println("ERROR: Slider::startSequence - wrong mode");
      break;
  }
}


boolean Slider::update() {
  if (slideRunning == true) {
  
    int nextPosition = 0;
    
    switch (mode) {
      case 1:  // TL-mode
        // TODO: write a function with millis() to do camTrigger() each interval.
        
        break;
        
      case 2:  // SL-mode
        
        // check position: calculate next position
        switch (slideDir) {
          case 0: // right +
            nextPosition = carriagePosition + stepsPerInterval;
            break;
          case 1: // left -
            nextPosition = carriagePosition - stepsPerInterval;
            break;
          default:
            Serial.println("ERROR: Slider::update - wrong slideDir");
            break;
        }
        
        // check if sequence is still running
        if (stepperInstance->getStatus()) {
          // still running, do nothing
        }else {
          stepperInstance->stop();
          
          camTrigger();
          
          // check position: enough space for one sequence ?
          if ( (nextPosition >= 0) && (nextPosition <= maxPosition) ) {
            carriagePosition = nextPosition;
            stepperInstance->start();
            displaySymbol(slideMve);
          }else {
            slideRunning = false;
            Serial.println("ERROR: virtual endstop hit - slide aborted");
          }
          Serial.print("---- current position: "); Serial.println(carriagePosition);
        }
        
        break;
        
      case 3:  // CO-mode
        
        break;
        
      default:
        Serial.println("ERROR: Slider::startSequence - wrong mode");
        break;
    }
  
  }else {
    Serial.println("ERROR: slide has not been started");
    return false;
  }
}


void Slider::stopSequence () {
  stepperInstance->stop();
}


void Slider::camTrigger () {
  Serial.println("trigger");
  displaySymbol(slideTrg);
  digitalWrite(trigger, HIGH);
  delay(triggerDuration);
  digitalWrite(trigger, LOW);
}


void Slider::manualRight() {
  
  if ( carriagePosition+1 <= maxPosition ) {
    carriagePosition++;
    Serial.print("---- current position: "); Serial.println(carriagePosition); // debug
    #ifdef INVERT_STEPPER_DIR
      digitalWrite(stepperDir, 0);
    #else
      digitalWrite(stepperDir, 1);
    #endif
    digitalWrite(stepperStep, HIGH);
    digitalWrite(stepperStep, LOW);
  }else {
    Serial.println("ERROR: manualRight: virtual endstop hit");
  }
}

void Slider::manualLeft() {
  if ( carriagePosition-1 >= 0 ) {
    carriagePosition--;
    Serial.print("---- current position: "); Serial.println(carriagePosition); // debug
    #ifdef INVERT_STEPPER_DIR
      digitalWrite(stepperDir, 1);
    #else
      digitalWrite(stepperDir, 0);
    #endif
    digitalWrite(stepperStep, HIGH);
    digitalWrite(stepperStep, LOW);
  }else {
    Serial.println("ERROR: manualLeft: virtual endstop hit");
  }
}



