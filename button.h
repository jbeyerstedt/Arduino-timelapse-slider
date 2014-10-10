/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * version 2.0.0 beta1 (dd.mm.yyyy)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */

/*
  library for reading and deboucing buttons
*/


#ifndef __button_h__
#define __button_h__

#include <Arduino.h>
#include <Bounce2.h>

class ButtonPair {
  int pinPlus;
  int pinMinus;
  int minValue;
  int maxValue;
  
  int value;
  
  Bounce *plusButton;
  Bounce *minusButton;
  unsigned long plusTimeStamp;
  unsigned long minusTimeStamp;
  int plusState;
  int minusState;
  
  boolean readPin(); // not used
  
  boolean isTriggered();
  
  public:
    ButtonPair(int PlusPin, int MinusPin);  
    void setInterval (int start, int end);
    boolean getValue ();
    void update ();
    void reset();
};

// detect only rising edge (press down)
// nromal class with Bounce2 library
class Button {
  int pin;
  
  boolean value;
  Bounce *button;
  
  public:
    Button(int pinNo);
    
    boolean triggered();
    void update ();
};


// detect only rising edge (press down)
// custom for my broken button
class Button2 {
  int pin;
  
  boolean value;
  
  unsigned long timeStamp;
  int buttonState;
  int lastButtonState;
  boolean triggerState;
  boolean lastTrigger;
  
  boolean readPin();
  
  public:
    Button2(int pinNo);
    
    boolean triggered();
    void update ();
};



#endif
