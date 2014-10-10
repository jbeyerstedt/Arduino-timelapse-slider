/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * version 2.0.0 beta1 (dd.mm.yyyy)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */


#include "button.h"
#include "config.h"

ButtonPair::ButtonPair (int PlusPin, int MinusPin)
{
  pinPlus = PlusPin;
  pinMinus = MinusPin;
  minValue = 0;
  maxValue = 99;
  value = 0;

  plusButton = new Bounce();
  minusButton = new Bounce();

  plusButton->attach(pinPlus);
  plusButton->interval(debounceDelay);
  minusButton->attach(pinMinus);
  minusButton->interval(debounceDelay);
}

boolean ButtonPair::readPin() {
  // return TRUE if button is pressed
  return (analogRead(pinPlus) > readThreshold);
}

void ButtonPair::setInterval (int minimum, int maximum) {
  minValue = minimum;
  maxValue = maximum;
}

void ButtonPair::update () {
  boolean plusStateChanged = plusButton->update();
  boolean minusStateChanged = minusButton->update();

  // increase
  if (plusStateChanged) {
    // Get the update value
    int buttonValue  = plusButton->read();
    if (buttonValue == LOW) {
      plusState = 0;
    } 
    else {
      value++;
      plusState = 1;
      plusTimeStamp = millis();
    }
  }
  if (plusState == 1) {
    if (millis() - plusTimeStamp >= fastCountInterval ) {
      plusTimeStamp = millis();
      value++;
    }
  }

  // decrease
  if (minusStateChanged) {
    // Get the update value
    int buttonValue  = minusButton->read();
    if (buttonValue == LOW) {
      minusState = 0;
    } 
    else {
      value--;
      minusState = 1;
      minusTimeStamp = millis();
    }
  }
  if (minusState == 1) {
    if (millis() - minusTimeStamp >= fastCountInterval ) {
      minusTimeStamp = millis();
      value--;
    }
  }
  
  // chech for interval
  if (value < minValue ) {
    value = minValue;
  } else if (value > maxValue ) {
    value = maxValue;
  } else;

}

boolean ButtonPair::getValue () {
  return value;
}

void ButtonPair::reset () {
  value = 0;
}




Button::Button (int pinNo) {
  pin = pinNo;
  
  button = new Bounce ();
  button->attach(pin);
  button->interval(debounceDelay);
  
}

void Button::update () {
  boolean stateChanged = button->update();
  int state = button->read();
  
  // Detect the rising edge
  if ( stateChanged && state == HIGH ) {    
    value = HIGH;
  } else {
    value = LOW;
  }
  
}

boolean Button::triggered () {
  return value;
}




Button2::Button2 (int pinNo) {
  pin = pinNo;
  value = false;
  lastButtonState = LOW;
  timeStamp = 0;
}

void Button2::update () {  
  
  int reading = readPin();

  if (reading != lastButtonState) {
    timeStamp = millis();
  } 
  
  if ((millis() - timeStamp) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        triggerState = true;
      }else {
        triggerState = false;
      }
    }
  }
  lastButtonState = reading;
  
  if (triggerState != lastTrigger) {
    if (triggerState == true) {
      value = true;
    }
  }else {
    value = false;
  }
  lastTrigger = triggerState;
  
}

boolean Button2::triggered () {
  return value;
}

boolean Button2::readPin () {
  // return TRUE if button is pressed
  return (analogRead(pin) > readThreshold);
}



