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
#include <Arduino.h>


Slider::Slider () {
  maxPosition = maxSteps;
  slideRunning = false;
  timeIntervalStart = 0;
  slideDir = 1;
}



void Slider::setParameters (int travelTime, int triggerInterval, int slidingDirection) {
  slideDir = !slidingDirection; // invert
  
  // calculate speed (steps per interval)
  // maxSteps / travelTime [min] * 60 [sek/min] = stepsPerSecond
  stepsPerSecond = maxSteps / travelTime / 60;

  // duration of one interval [ms] = trigegrInterval [s] * 1000 [ms/s] - triggerDuration [ms]
  intervalDuration = triggerInterval * 1000 - triggerDuration;

  stepsPerInterval = intervalDuration / stepsPerSecond;

  if (stepsPerSecond > maxVelocity) {
    stepsPerSecond = maxVelocity;
  }
  
  Serial.print("stepsPerSecond ");Serial.println(stepsPerSecond);
  Serial.print("intervalDuration ");Serial.println(intervalDuration);
  Serial.print("stepsPerInterval ");Serial.println(stepsPerInterval);
  Serial.print("direction ");Serial.println(slideDir);
  
  // TODO implement movie slide
}

void Slider::startSlide () {
  digitalWrite(stepperSleep, HIGH);
  digitalWrite(stepperDir, slideDir);
  slideRunning = true;
  timeIntervalStart = 0;
}

boolean Slider::update() {
  // enable slide an check for virtual endstop
  if (slideRunning == true && carriagePosition < maxPosition && carriagePosition >= 0 ) {
    
    if (timeIntervalStart == 0) { // slide has been started, but not executed yet
      doSlide();
      Serial.println("first start");
    }
    
    // check if one stepping interval is done
    if ( (millis()-timeIntervalStart) >= intervalDuration ) { 
      camTrigger();
      doSlide();
      return true;
    }
    else { // still moving
      return true;
    }

  }else if (slideRunning == false) {
    Serial.println("ERROR: slide has not been started");
    return false;
  }else {
    Serial.println("ERROR: endstop hit");
    return false;
  }
}

// starts stepper for one slide interval 
void Slider::doSlide () {
  Serial.println("retrigger tone");
  displaySymbol(slideMve);
  timeIntervalStart = millis();
  //tone(stepperStep, stepsPerSecond, intervalDuration);
  
  //toneAC();
  
  carriagePosition += stepsPerInterval;
}

void Slider::camTrigger () {
  Serial.println("trigger");
  displaySymbol(slideTrg);
  digitalWrite(trigger, HIGH);
  delay(triggerDuration);
  digitalWrite(trigger, LOW);
}

















boolean Slider::doSteps (int dir) {

  if (carriagePosition >= maxSteps ) {
    Serial.println("virtual endstop hit");
    return false;
  }
  else {

    //displayNumber(103);
    displaySymbol(slideMve);
    // make x steps
    for (int i = 0; i < slideStepsPerInterval; i++) {
      // make one step
      if (dir == 1) {
        carriagePosition++;
      }
      else if (dir == 0) {
        carriagePosition--;
      }
      else {
        return false;
      }
    }


    delay(1000);

    Serial.println("trigger");
    displaySymbol(slideTrg);
    digitalWrite(trigger, HIGH);
    delay(triggerDuration);
    digitalWrite(trigger, LOW);

    return true;
  }
}






void Slider::setParametersOld (int travelTime, int triggerInterval) {
  // calulate parameters like slide speed

  // calculate velocity:  
  // maxSteps / travelTime [min] * 60 [sek/min] = stepsPerSecond
  // 1000 [ms] / stepsPerSecond = timePerStep [ms/step]
  int stepsPerSecond = maxSteps / travelTime / 60;
  int timePerStep = 1000 / stepsPerSecond;
  slideStepDelay = timePerStep - workingDelay;
  if (slideStepDelay <= 0) {
    slideStepDelay = 0;
  } // safety that slideStepDelay is not too small

  // calculate the steps per Interval
  if (triggerInterval == 0) { // movie mode
    slideStepsPerInterval = maxSteps;
    Serial.println("movie mode active");
  }
  else { // normal timelapse operation



    // a too big, but correct caculated number, will make this calculation wrong. The slider isn´t faster than 14 steps per Second 
    if (stepsPerSecond >= 14) {
      stepsPerSecond = 14;
    }
    slideStepsPerInterval = stepsPerSecond * triggerInterval;
  }

  //slideStepsPerInterval = 0;
  //slideStepDelay = 0;
}




