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
int currentMode  = 0; // 1: IN - 2: SL - 3: CO

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
    case 1:  // initialization / set mode
      buttons.setInterval(0,3);
      currentMode = buttons.getValue();
      switch (currentMode) {
        case 1: // IN
          displaySymbol(modeIn);
          break;
        case 2: // SL
          displaySymbol(modeSl);
          break;
        case 3: // CO
          displaySymbol(modeCo);
          break;
        default:
          displaySymbol(waitInput);
          //Serial.println("no mode set");
          break;
      }
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 11;
            Serial.println("--switch to state 11");
            break;
          case 2: // SL
            buttons.reset();
            currentState = 21;
            Serial.println("--switch to state 21");
            break;
          case 3: // CO
            buttons.reset();
            currentState = 31;
            Serial.println("--switch to state 31");
            break;
          default:
            displaySymbol(waitInput);
            Serial.println("ERROR: no mode set");
            break;
        }
      }
      
      break;
      
    case 11:  // manual operation / idle
    case 21:
    case 31:
      // TODO implement manual operation
      displaySymbol(waitIdle);
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 12;
            Serial.println("--switch to state 12");
            break;
          case 2: // SL
            buttons.reset();
            currentState = 22;
            Serial.println("--switch to state 22");
            break;
          case 3: // CO
            buttons.reset();
            currentState = 32;
            Serial.println("--switch to state 32");
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;  
    
    case 22:  // set total time
    case 32:
      buttons.setInterval(0,numberLimitSlideTime);
      slideTime = buttons.getValue();
      displayNumber(slideTime);
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 2: // SL
            buttons.reset();
            currentState = 23;
            Serial.println("--switch to state 23");
            break;
          case 3: // CO
            buttons.reset();
            currentState = 33;
            Serial.println("--switch to state 33");
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;
    
    case 13:  // set carriage movement direction
    case 23:
    case 33:
      buttons.setInterval(0,1);
      slideDirection = buttons.getValue();  
      if (slideDirection == 1) { // right
        displaySymbol(dirRigt);
      }else if (slideDirection == 0) { // left
        displaySymbol(dirLeft);
      }
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 14;
            Serial.println("--switch to state 14");
            break;
          case 2: // SL
            buttons.reset();
            currentState = 24;
            Serial.println("--switch to state 24");
            break;
          case 3: // CO
            buttons.reset();
            currentState = 34;
            Serial.println("--switch to state 34");
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;
    
    case 12:  // set trigger interval
    case 24:
      buttons.setInterval(0,numberLimitTriggerTime);
      triggerInterval = buttons.getValue();
      displayNumber(triggerInterval);
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 13;
            Serial.println("--switch to state 13");
            break;
          case 2: // SL
            buttons.reset();
            currentState = 25;
            Serial.println("--switch to state 25");
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;
    
    case 14:  // prespare data / set slide parameters
    case 25:
    case 34:
      Serial.println("--- data preparation (state 14, 25, 35)");
      displaySymbol(waitGo);
      
      switch (currentMode) {
        case 1: // IN
          // TODO
          //mySlider.setParameters(slideTime, triggerInterval, slideDirection);
          
          buttons.reset();
          currentState = 15;
          Serial.println("--switch to state 15");
          break;
        case 2: // SL
          // TODO
          mySlider.setParameters(slideTime, triggerInterval, slideDirection);
          
          buttons.reset();
          currentState = 26;
          Serial.println("--switch to state 26");
          break;
        case 3: // CO
          // TODO
          //mySlider.setParameters(slideTime, triggerInterval, slideDirection);
          
          buttons.reset();
          currentState = 35;
          Serial.println("--switch to state 35");
          break;
        default:
          displaySymbol(waitInput);
          currentState = 0;
          Serial.println("ERROR: no mode set -> reset");
          break;
      }
      break;
    
    case 15:  // wait for go
    case 26:
    case 35:
      // TODO do slide
      // mySlider.startSlide();
      
      
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 16;
            Serial.println("--switch to state 16");
            break;
          case 2: // SL
            buttons.reset();
            currentState = 27;
            Serial.println("--switch to state 27");
            break;
          case 3: // CO
            buttons.reset();
            currentState = 36;
            Serial.println("--switch to state 36");
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;
    
    case 16:  // supervise slide operation
    case 27:
    case 36:
      // TODO
      // supervise slide
      
      if ( mySlider.update() ) {
        // normal operation
      }else {
        Serial.println("ERROR: virtual endstop hit - slide aborted");
      }
      
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 11;
            Serial.println("--switch to state 11");
            break;
          case 2: // SL
            buttons.reset();
            currentState = 21;
            Serial.println("--switch to state 21");
            break;
          case 3: // CO
            buttons.reset();
            currentState = 31;
            Serial.println("--switch to state 31");
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;
    
    default:
      // initialise all systems - get mode
      // or something wrong happened
      Serial.println("--reset (default case)");
      currentState = 1;
      
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




