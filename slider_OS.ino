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

#include "slow_impulses.h"

#include "config.h"
#include "button.h"
#include "display.h"

#include "my_classes.h"

#include <MemoryFree.h>


int currentState = -1; // TODO: increase all state numbers by 1
int currentMode  = 0; // 1: IN - 2: SL - 3: CO

int choosenNumber = 0;

int slideTime = 1;
int slideDirection = 0; // 1 = right, 0 = left
int triggerInterval = 0;



ButtonPair buttons (buttonPlusPin, buttonMinusPin);
//Button buttonEnter (buttonEnterPin); // for a normally working button
Button2 buttonEnter (buttonEnterPin); // special for my broken button

SlowImpulses stepper;

Slider mySlider(&stepper);


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
  
  stepper.init(stepperStep);
  //stepper.init(6); // debug
  
  Serial.begin(9600);
}


void loop() {
  
  // read all buttons
  buttons.update();
  buttonEnter.update();
  
  
  switch (currentState) {
    case 0:  // initialize carriage position
      // TODO
      // set initial carriage position
      
      //currentState = 1;
      
      currentMode = 2; // debug
      
      // debug
      displaySymbol(waitInput);
      
      // debug:
      if(buttonEnter.triggered() ) {
        currentState = 25;
      }
      
      break;
    
    case 14:  // prespare data / set slide parameters
    case 25:
    case 34:
      Serial.println("--- data preparation (state 14, 25, 35)");
      displaySymbol(waitGo);
      
      switch (currentMode) {
        case 2: // SL
          // TODO
          
          //mySlider.setParameters(currentMode, slideTime, triggerInterval, slideDirection);
          mySlider.setParameters(2, 10, 2, 1); // debug
          
          buttons.reset();
          currentState = 26;
          Serial.println("--switch to state 26");
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
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 2: // SL
            buttons.reset();
            
            
            mySlider.startSlide();
            
//            stepper.set(4,5000); // debug
//            stepper.start();
            
            
            
            
            currentState = 27;
            Serial.println("--switch to state 27");
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
      
      //Serial.print("durationCount "); Serial.println(stepper.durationCount);
      
//      if ( mySlider.update() ) {
//        // normal operation
//      }else {
//        Serial.println("ERROR: virtual endstop hit - slide aborted");
//      }
      
      
      // direct
      if (stepper.getStatus()) {
        
      }else {
        stepper.stop();
        Serial.println("------------TRIGGER----------");
        delay(500);
        
        stepper.start();
      }
      
      
      //Serial.print("status: "); Serial.println(stepper.getStatus());
      //Serial.print("count: "); Serial.println(stepper.durationCount);
      
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 2: // SL
            buttons.reset();
            currentState = 21;
            Serial.println("--switch to state 21");
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
      currentState = 0;
      
      break;
  }
  
  
  
}





// timer compare interrupt service routine
// must be in main file / can´t figure out how to do it in the impulse.h or impule.cpp
ISR(TIMER1_COMPA_vect)
{ 
  stepper.durationCount++;
  if (stepper.durationCount <= stepper.durationCompare ) {
    stepper.durationCount++;
    digitalWrite(stepper.pinNo, digitalRead(stepper.pinNo) ^ 1);
  }else {
    stepper.durationStatus = false;
    stepper.stop();
  }
  
}




