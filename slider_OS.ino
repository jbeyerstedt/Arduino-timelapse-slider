/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * for debouncing the buttons the Bounce2 library is used. Download it here:
 * https://github.com/thomasfredericks/Bounce-Arduino-Wiring
 * 
 * version 2.0.0 beta1 (dd.mm.yyyy)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */


#include <Bounce2.h>

#include "config.h"
#include "button.h"
#include "display.h"
#include "my_classes.h"
#include "slow_impulses.h"


int currentState = 0;

int choosenNumber = 0;

int slideTime = 1;
int slideDirection = 0; // 1 = right, 0 = left
int triggerInterval = 0;




ButtonPair buttons (buttonPlusPin, buttonMinusPin);
//Button buttonEnter (buttonPlusPin); // for a normally working button
Button2 buttonEnter (buttonEnterPin); // special for my broken button


SlowImpulses stepper;

Slider mySlider;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(shiftLatch, OUTPUT);
  pinMode(shiftClock, OUTPUT);
  pinMode(shiftData, OUTPUT);
  
  pinMode(buttonPlusPin, INPUT);
  pinMode(buttonMinusPin, INPUT);
  pinMode(buttonEnterPin, INPUT);
  
  pinMode(stepperStep, OUTPUT);
  pinMode(stepperDir, OUTPUT);
  pinMode(stepperSleep, OUTPUT);
  
  pinMode(trigger, OUTPUT);
  
  digitalWrite(stepperSleep, LOW); // set stepper driver to sleep
  
  Serial.begin(9600);
}

void loop() {
  
  // read all buttons
  buttons.update();
  buttonEnter.update();
  
  
  switch (currentState) {
    case 1: // set total slide time
      
      buttons.setInterval(0,numberLimitSlideTime);
      slideTime = buttons.getValue();
      displayNumber(slideTime);
      
      if (buttonEnter.triggered() ) {
        buttons.reset();
        currentState = 2;
        Serial.println("--switch to state 2");
      }
      
      break;
    case 2: // set slide direction
      
      buttons.setInterval(0,1);
      slideDirection = buttons.getValue();  
      if (slideDirection == 1) { // right
        displaySymbol(dirRigt);
      }else if (slideDirection == 0) { // left
        displaySymbol(dirLeft);
      }
      
      if (buttonEnter.triggered() ) {
        buttons.reset();
        currentState = 3;
        Serial.println("--switch to state 3");
      }
      
      break;
    case 3: // set trigger interval
      
      buttons.setInterval(0,numberLimitTriggerTime);
      triggerInterval = buttons.getValue();
      displayNumber(triggerInterval);
      
      if (buttonEnter.triggered() ) {
        buttons.reset();
        currentState = 4;
        Serial.println("--switch to state 4");
        
        displaySymbol(waitGo);
        mySlider.setParameters(slideTime, triggerInterval, slideDirection);
      }
      
      break;
    case 4: // wait for go
      
      if (buttonEnter.triggered() ) {
        mySlider.startSlide();
        
        currentState = 10;
        Serial.println("--switch to state 10");
      }
      
      break;
    case 10: // make a timelapse (or movie) slide. mode is controlled by Slider class
      
      if ( mySlider.update() ) {
        // normal operation
      }else {
        Serial.println("ERROR: virtual endstop hit - slide aborted");
      }
      
      if (buttonEnter.triggered() ) {
        displaySymbol(waitInput);
        currentState = 11;
        Serial.println("--switch to state 11");
      }
      
      break;
    case 11: 
      
      break;
    default:
      // initialise all systems
      // or something wrong happened
      currentState = 1;
      displaySymbol(waitInput);
      Serial.println("--reset (default case)");
      break;
  }
  
}





// timer compare interrupt service routine
// must be in main file / can´t figure out how to do it in the impulse.h or impule.cpp
ISR(TIMER1_COMPA_vect)
{ 
  if (stepper.durationCount <= stepper.durationCompare ) {
    stepper.durationCount++;
    digitalWrite(stepper.pinNo, digitalRead(stepper.pinNo) ^ 1);
  }else {
    stepper.durationStatus = false;
    stepper.stop();
  }
  
}




