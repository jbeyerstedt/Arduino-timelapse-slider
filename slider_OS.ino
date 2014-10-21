/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * for debouncing the buttons the Bounce2 library is used. Download it here:
 * https://github.com/thomasfredericks/Bounce-Arduino-Wiring
 * 
 * version 2.0.0 beta1 (19.10.2014)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */


#include <Bounce2.h>

#include "config.h"
#include "button.h"
#include "display.h"
#include "my_classes.h"
#include "slow_impulses.h"


int currentState = -1; 
int currentMode  = 0; // 1: IN - 2: SL - 3: CO

int choosenNumber = 0;
boolean slideError = false;
int buttonsHelper = 0; // variable to temporarly store button return values
int buttonsLastVal = 0;

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
  
  Serial.begin(9600);
}


void loop() {
  
  // read all buttons
  buttons.update();
  buttonEnter.update();
  
  
  // do something based on the case
  switch (currentState) {
    
    case 1:  // initialize carriage position
      
      buttons.setInterval(0,2);
      buttonsHelper = buttons.getValue();  
      switch (buttonsHelper) {
        case 0: // left
          displaySymbol(initLeft);
          mySlider.initCarriagePosition(0);
          break;
        case 2: // right
          displaySymbol(initRigt);
          mySlider.initCarriagePosition(maxSteps);
          break;
        default:
          displaySymbol(initPos);
          break;
      }
      
      if (buttonEnter.triggered() ) {
        Serial.print("initial position: "); Serial.println(buttonsHelper);
        
        currentState = 2;
        Serial.println("--switch to state 2");
      }
      
      break;
    
    case 2:  // initialization / set mode
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
          break;
      }
      
      if (buttonEnter.triggered() ) {
        Serial.print("mode: "); Serial.println(currentMode);
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 12;
            Serial.println("--switch to state 12");
            break;
          case 2: // SL
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 22;
            Serial.println("--switch to state 22");
            break;
          case 3: // CO
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 32;
            Serial.println("--switch to state 32");
            break;
          default:
            displaySymbol(waitInput);
            Serial.println("ERROR: no mode set");
            break;
        }
      }
      
      break;
      
    case 12:  // manual operation / idle
    case 22:
    case 32:
      displaySymbol(waitIdle);
      
      buttonsHelper = buttons.getValue();
      
      digitalWrite(stepperSleep, HIGH);
      
      if (buttonsHelper > buttonsLastVal ) {        // right
        buttonsLastVal = buttonsHelper;
        mySlider.manualRight();
      }else if (buttonsHelper < buttonsLastVal ) {  // left
        buttonsLastVal = buttonsHelper;
        mySlider.manualLeft();
      }
      
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.setInterval(1,numberLimitTriggerTime);
            buttons.presetValue(triggerInterval); // preset last entered value
            currentState = 13;
            Serial.println("--switch to state 13");
            break;
          case 2: // SL
            buttons.setInterval(1,numberLimitSlideTime);
            buttons.presetValue(slideTime); // preset last entered value
            currentState = 23;
            Serial.println("--switch to state 23");
            break;
          case 3: // CO
            buttons.setInterval(1,numberLimitSlideTime);
            buttons.presetValue(slideTime); // preset last entered value
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
    
    case 23:  // set total time
    case 33:
      slideTime = buttons.getValue();
      displayNumber(slideTime);
      
      if (buttonEnter.triggered() ) {
        Serial.print("slideTime: "); Serial.println(slideTime);
        switch (currentMode) {
          case 2: // SL
            buttons.setInterval(0,1);
            buttons.presetValue(slideDirection); // preset last entered value
            currentState = 24;
            Serial.println("--switch to state 24");
            break;
          case 3: // CO
            buttons.setInterval(0,1);
            buttons.presetValue(slideDirection); // preset last entered value
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
    
    case 24:  // set carriage movement direction
    case 34:
      slideDirection = buttons.getValue();  
      if (slideDirection == 1) { // right
        displaySymbol(dirRigt);
      }else if (slideDirection == 0) { // left
        displaySymbol(dirLeft);
      }
      
      if (buttonEnter.triggered() ) {
        Serial.print("slideDirection: "); Serial.println(slideDirection);
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 15;
            Serial.println("--switch to state 15");
            break;
          case 2: // SL
            buttons.setInterval(1,numberLimitTriggerTime);
            buttons.presetValue(triggerInterval); // preset last entered value
            currentState = 25;
            Serial.println("--switch to state 25");
            break;
          case 3: // CO
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
      }
      break;
    
    case 13:  // set trigger interval
    case 25:
      triggerInterval = buttons.getValue();
      displayNumber(triggerInterval);
      
      if (buttonEnter.triggered() ) {
        Serial.print("triggerInterval: "); Serial.println(triggerInterval);
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            currentState = 15;
            Serial.println("--switch to state 14");
            break;
          case 2: // SL
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
      }
      break;
    
    case 15:  // prespare data / set slide parameters
    case 26:
    case 35:
      Serial.println("--data preparation");
      displaySymbol(waitGo);
      
      switch (currentMode) {
        case 1: // IN
          // TODO: functionality in Slider::setParameters special for IN-mode
          //mySlider.setParameters(currentMode, slideTime, triggerInterval, slideDirection);
          
          buttons.reset();
          currentState = 16;
          Serial.println("--switch to state 16");
          break;
        case 2: // SL
          mySlider.setParameters(currentMode, slideTime, triggerInterval, slideDirection);
          
          buttons.reset();
          currentState = 27;
          Serial.println("--switch to state 27");
          break;
        case 3: // CO
          // TODO: functionality in Slider::setParameters special for CO-mode
          //mySlider.setParameters(currentMode, slideTime, triggerInterval, slideDirection);
          
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
      break;
    
    case 16:  // wait for go
    case 27:
    case 36:
      
      if (buttonEnter.triggered() ) {
        switch (currentMode) {
          case 1: // IN
            buttons.reset();
            mySlider.startSequence();
            currentState = 17;
            Serial.println("--switch to state 17");
            break;
          case 2: // SL
            buttons.reset();
            mySlider.startSequence();
            currentState = 28;
            Serial.println("--switch to state 28");
            break;
          case 3: // CO
            buttons.reset();
            mySlider.startSequence();
            currentState = 37;
            Serial.println("--switch to state 37");
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;
    
    case 17:  // supervise slide operation
    case 28:
    case 37:
                  
      if ( mySlider.update() ) {
        // normal operation
        slideError = false;
      }else {
        // virtual endstop hit
        slideError = true;
      }
      
      
      if (buttonEnter.triggered() || (slideError) ) {
        switch (currentMode) {
          case 1: // IN
            mySlider.stopSequence();
            
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 12;
            Serial.println("--switch to state 12");
            break;
          case 2: // SL
            mySlider.stopSequence();
            
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 22;
            Serial.println("--switch to state 22");
            break;
          case 3: // CO
            mySlider.stopSequence();
            
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 22;
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
    
    default:
      // initialise all systems
      // or something wrong happened
      Serial.println("--reset (default case)");
      currentState = 1;
      buttons.presetValue (1); // init position: display "initPos" first.
      
      break;
  } // end switch
  
  
} // end loop





// timer compare interrupt service routine
// must be in main file / can´t figure out how to do it in the impulse.h or impule.cpp
ISR(TIMER1_COMPA_vect)
{ 
  if (stepper.durationCount < stepper.durationCompare ) {
    stepper.durationCount++;
    digitalWrite(stepper.pinNo, digitalRead(stepper.pinNo) ^ 1);
  }else {
    stepper.durationStatus = false;
    stepper.stop();
  }
}
