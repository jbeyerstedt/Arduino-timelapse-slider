/*
 * project: slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * for debouncing the buttons the Bounce2 library is used. Download it here:
 * https://github.com/thomasfredericks/Bounce-Arduino-Wiring
 * 
 * copyright: Jannik Beyerstedt | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * license: http://www.gnu.org/licenses/gpl-3.0.txt GPLv3 License
 *
 * version 2.1.1 (08.11.2014)
 * changelog: see readme.md
 */


#include <Bounce2.h>

#include "config.h"
#include "button.h"
#include "display.h"
#include "my_classes.h"
#include "slow_impulses.h"


int currentState = -1; 
int opMode  = 0;       // operation mode - 1: IN - 2: SL - 3: CO

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
  
  #ifndef NO_SERIAL_COM
  Serial.begin(9600);
  #endif
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
        #ifdef INFO_COM
        Serial.println("--switch to state 2");
        #endif
      }
      
      break;
    
    case 2:  // initialization / set mode
      buttons.setInterval(0,3);
      opMode = buttons.getValue();
      switch (opMode) {
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
        Serial.print("mode: "); Serial.println(opMode);
        switch (opMode) {
          case 1: // IN
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 12;
            #ifdef INFO_COM
            Serial.println("--switch to state 12");
            #endif
            break;
          case 2: // SL
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 22;
            #ifdef INFO_COM
            Serial.println("--switch to state 22");
            #endif
            break;
          case 3: // CO
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 32;
            #ifdef INFO_COM
            Serial.println("--switch to state 32");
            #endif
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
        switch (opMode) {
          case 1: // IN
            buttons.setInterval(1,numberLimitTriggerTime);
            buttons.presetValue(triggerInterval); // preset last entered value
            
            displaySymbol(setInt); delay(500);
            currentState = 13;
            #ifdef INFO_COM
            Serial.println("--switch to state 13");
            #endif
            break;
          case 2: // SL
            buttons.setInterval(1,numberLimitSlideTime);
            buttons.presetValue(slideTime); // preset last entered value
            
            displaySymbol(setTime); delay(500);
            currentState = 23;
            #ifdef INFO_COM
            Serial.println("--switch to state 23");
            #endif
            break;
          case 3: // CO
            buttons.setInterval(1,numberLimitSlideTime);
            buttons.presetValue(slideTime); // preset last entered value
            
            displaySymbol(setTime); delay(500);
            currentState = 33;
            #ifdef INFO_COM
            Serial.println("--switch to state 33");
            #endif
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
        switch (opMode) {
          case 2: // SL
            buttons.setInterval(0,1);
            buttons.presetValue(slideDirection); // preset last entered value
            currentState = 24;
            #ifdef INFO_COM
            Serial.println("--switch to state 24");
            #endif
            break;
          case 3: // CO
            buttons.setInterval(0,1);
            buttons.presetValue(slideDirection); // preset last entered value
            currentState = 34;
            #ifdef INFO_COM
            Serial.println("--switch to state 34");
            #endif
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
        switch (opMode) {
          case 1: // IN
            buttons.reset();
            currentState = 15;
            #ifdef INFO_COM
            Serial.println("--switch to state 15");
            #endif
            break;
          case 2: // SL
            buttons.setInterval(1,numberLimitTriggerTime);
            buttons.presetValue(triggerInterval); // preset last entered value
            
            displaySymbol(setInt); delay(500);
            currentState = 25;
            #ifdef INFO_COM
            Serial.println("--switch to state 25");
            #endif
            break;
          case 3: // CO
            buttons.reset();
            currentState = 35;
            #ifdef INFO_COM
            Serial.println("--switch to state 35");
            #endif
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
        switch (opMode) {
          case 1: // IN
            buttons.reset();
            currentState = 14;
            #ifdef INFO_COM
            Serial.println("--switch to state 14");
            #endif
            break;
          case 2: // SL
            buttons.reset();
            currentState = 26;
            #ifdef INFO_COM
            Serial.println("--switch to state 26");
            #endif
            break;
          default:
            displaySymbol(waitInput);
            currentState = 0;
            Serial.println("ERROR: no mode set -> reset");
            break;
        }
      }
      break;
    
    case 14:  // prepare data / set slide parameters
    case 26:
    case 35:
      #ifdef INFO_COM
      Serial.println("--data preparation");
      #endif
      displaySymbol(waitGo);
      
      switch (opMode) {
        case 1: // IN
          mySlider.setParameters(opMode, 0, triggerInterval, 0);
          
          buttons.reset();
          currentState = 15;
          #ifdef INFO_COM
          Serial.println("--switch to state 15");
          #endif
          break;
        case 2: // SL
          mySlider.setParameters(opMode, slideTime, triggerInterval, slideDirection);
          
          buttons.reset();
          currentState = 27;
          #ifdef INFO_COM
          Serial.println("--switch to state 27");
          #endif
          break;
        case 3: // CO
          mySlider.setParameters(opMode, slideTime, 0, slideDirection);
          
          buttons.reset();
          currentState = 36;
          #ifdef INFO_COM
          Serial.println("--switch to state 36");
          #endif
          break;
        default:
          displaySymbol(waitInput);
          currentState = 0;
          Serial.println("ERROR: no mode set -> reset");
          break;
      }
      break;
    
    case 15:  // wait for go
    case 27:
    case 36:
      
      if (buttonEnter.triggered() ) {
        switch (opMode) {
          case 1: // IN
            buttons.reset();
            mySlider.startSequence();
            currentState = 16;
            #ifdef INFO_COM
            Serial.println("--switch to state 16");
            #endif
            break;
          case 2: // SL
            buttons.reset();
            mySlider.startSequence();
            currentState = 28;
            #ifdef INFO_COM
            Serial.println("--switch to state 28");
            #endif
            break;
          case 3: // CO
            buttons.reset();
            mySlider.startSequence();
            currentState = 37;
            #ifdef INFO_COM
            Serial.println("--switch to state 37");
            #endif
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
    case 28:
    case 37:
                  
      if ( mySlider.update() ) {
        // normal operation
        slideError = false;
      }else {
        // virtual endstop hit
        slideError = true;
        displaySymbol(errorGen);
        delay(1000);
      }
      
      
      if (buttonEnter.triggered() || (slideError) ) {
        switch (opMode) {
          case 1: // IN
            mySlider.stopSequence();
            
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 12;
            #ifdef INFO_COM
            Serial.println("--switch to state 12");
            #endif
            break;
          case 2: // SL
            mySlider.stopSequence();
            
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 22;
            #ifdef INFO_COM
            Serial.println("--switch to state 22");
            #endif
            break;
          case 3: // CO
            mySlider.stopSequence();
            
            buttons.reset();
            buttons.setInterval(-maxSteps ,maxSteps, (1000/maxVelocity) ); // step interval = 1/maxVelocity in ms
            currentState = 22;
            #ifdef INFO_COM
            Serial.println("--switch to state 32");
            #endif
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
