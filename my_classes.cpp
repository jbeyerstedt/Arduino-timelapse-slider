/*
 * project: slider OS 2
 * my custom classes
 *
 * copyright: Jannik Beyerstedt | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * license: http://www.gnu.org/licenses/gpl-3.0.txt GPLv3 License
 *
 * version 2.1.0 (26.10.2014)
 */


#include "my_classes.h"


Slider::Slider (SlowImpulses *stepper) {
  stepperInstance = stepper;
  
  maxPosition = maxSteps;
  sequenceRunning = false;
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
      // create new IntervalTrigger instance
      intTrigger = new IntervalTrigger();
      
      intervalDuration = triggerInterval;
      
      break;
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
      
      if (stepsPerSecond > maxVelocity) {
        stepsPerSecond = maxVelocity;
      }
      
      // set intervalDuration to max time until endstop will be hit
      switch (slideDir) {
        case 0: // right +
          intervalDuration = float(maxSteps-carriagePosition) / stepsPerSecond * 1000;
          break;
        case 1: // left -
          intervalDuration = float(carriagePosition) / stepsPerSecond * 1000;
          break;
        default:
          Serial.println("ERROR: Slider::set (mode3) - wrong slideDir");
          break;
      }
            
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
      
      if ( intTrigger->set(intervalDuration*1000) ) {
        sequenceRunning = true;
        
        intTrigger->start();
        displaySymbol(slideInt);
        
      }else {
        sequenceRunning = false;
        // error
      }
      break;
    
    case 2:  // SL-mode
      digitalWrite(stepperSleep, HIGH);
      digitalWrite(stepperDir, slideDir);
      
      if ( stepperInstance->set(stepsPerSecond, intervalDuration) ) {
        
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
          sequenceRunning = true;
          
          carriagePosition = nextPosition;
          stepperInstance->start();
          displaySymbol(slideMve);
          #ifdef DEBUG_COM
          Serial.print("---- current position: "); Serial.println(carriagePosition); // debug
          #endif
        }else {
          sequenceRunning = false;
          Serial.println("ERROR: virtual endstop hit - slide aborted");
        }
        
      }else {
        Serial.println("ERROR: slow_impulses::set - wrong frequency");
        sequenceRunning = false;
      }
      
      break;
    
    case 3:  // CO-mode
      digitalWrite(stepperSleep, HIGH);
      digitalWrite(stepperDir, slideDir);
      
      if ( stepperInstance->set(stepsPerSecond, intervalDuration) ) {
        sequenceRunning = true;
        
        mode3StartTime = millis();
        stepperInstance->start();
        displaySymbol(slideMve);
        mode3CarriageStartPos = carriagePosition;
      }else {
        Serial.println("ERROR: slow_impulses::set - wrong frequency");
        sequenceRunning = false;
      }
      
      break;
    
    default:
      Serial.println("ERROR: Slider::startSequence - wrong mode");
      break;
  }
}


boolean Slider::update() {
  if (sequenceRunning == true) {
  
    int nextPosition = 0;
    
    switch (mode) {
      case 1:  // TL-mode
        
        // check if one interval is over
        if ( intTrigger->getStatus() ) {
          // still running, do nothing
        }else {
          camTrigger();
          displaySymbol(slideInt);
        }
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
          stepperInstance->stop(); // stop before restart
          
          camTrigger();
          
          // check position: enough space for one sequence ?
          if ( (nextPosition >= 0) && (nextPosition <= maxPosition) ) {
            carriagePosition = nextPosition;
            stepperInstance->start();
            displaySymbol(slideMve);
          }else {
            sequenceRunning = false;
            Serial.println("ERROR: virtual endstop hit - slide aborted");
          }
          #ifdef DEBUG_COM
          Serial.print("---- current position: "); Serial.println(carriagePosition);
          #endif
        }
        
        break;
        
      case 3:  // CO-mode
        
        // calculate position by calculating the position change since start
        switch (slideDir) {
          case 0: // right +
            nextPosition = mode3CarriageStartPos + ( (millis()-mode3StartTime)*stepsPerSecond/1000 );
            break;
          case 1: // left -
            nextPosition = mode3CarriageStartPos - ( (millis()-mode3StartTime)*stepsPerSecond/1000 );
            break;
          default:
            Serial.println("ERROR: Slider::update - wrong slideDir");
            break;
        }
        
        if ( (nextPosition >= 0) && (nextPosition <= maxSteps) ) {
          // carriage still in range
          carriagePosition = nextPosition;
        }else {
          if (nextPosition <= 0) {
            carriagePosition = 0;
          }else if (nextPosition >= maxSteps) {
            carriagePosition = maxSteps;
          }
          sequenceRunning = false;
          stepperInstance->stop();
          Serial.println("ERROR: virtual endstop hit - slide aborted");
        }
        #ifdef DEBUG_COM
        Serial.print("---- current position: "); Serial.println(carriagePosition);
        #endif
        
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
  switch (mode) {
    case 1:  // TL-mode
      intTrigger->stop();
      break;
    case 2:  // SL-mode
      stepperInstance->stop();
      break;
    case 3:  // CO-mode
      stepperInstance->stop();
      break;
    default:
      break;
  }
  
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
    
    #ifdef DEBUG_COM
    Serial.print("---- current position: "); Serial.println(carriagePosition); // debug
    #endif
    
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
    
    #ifdef DEBUG_COM
    Serial.print("---- current position: "); Serial.println(carriagePosition); // debug
    #endif
    
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




IntervalTrigger::IntervalTrigger() {
  started = false;
  counter = 0;
}

boolean IntervalTrigger::set(int intervalDuration) {
  if (intervalDuration > 0) {
    interval = intervalDuration;
    return true;
  }else {
    Serial.println("ERROR: IntervalTrigger::start - no interval duration set");
    return false;
  }
}

void IntervalTrigger::start() {
  startTime = millis();
  started = true;
}

boolean IntervalTrigger::getStatus () {
  if (started == true) {
    if ( ( millis()-startTime-(long(interval)*counter) ) > interval ) {
      // one interval is over
      counter++;
      return false;
    }else {
      return true;
    }
  }else {
    return true; // because you will not interrupt
    Serial.println("ERROR: IntervalTrigger::start - not started!");
  }
}

void IntervalTrigger::stop() {
  started = false;
  counter = 0;
}
