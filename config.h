/*
 * project: slider OS 2
 * file: config.h
 *
 * copyright: Jannik Beyerstedt | http://jannikbeyerstedt.de | code@jannikbeyerstedt.de
 * license: http://www.gnu.org/licenses/gpl-3.0.txt GPLv3 License
 *
 * version 2.1.1 (08.11.2014)
 */

/*
  configurations file
*/


#ifndef __config_h__
#define __config_h__
 
// basic config
// --------------------

// define input pins
#define trigger 2
//#define servoTilt 3

#define buttonPlusPin  A1
#define buttonMinusPin A2
#define buttonEnterPin A0

// define output pins
#define shiftLatch   12
#define shiftClock    9
#define shiftData     8
#define ledPin       13
#define stepperStep   6
#define stepperDir    7
#define stepperSleep  4

// set mechanical properties
const int triggerDuration = 500;   // how long should your camera be triggered (exposure time is still set in camera)
const int maxSteps        = 1740;  // amount of steps to slide the slider along {my calue for half step: 2200}
const int maxVelocity     = 50;    // maximum number of steps per second you stepper motor can perform
#define INVERT_STEPPER_DIR         // invert stepper direction, that it matches your buttons (right button, slide right)


// advanced config
// --------------------

// define the maximum number displayed
const int numberLimitSlideTime = 30;    // maximum time input to slide (in minutes)
const int numberLimitTriggerTime = 10;  // maximum trigger interval time (in seconds)

// define the number os milliseconds to retrigger a count-up if button is held down
const int fastCountInterval = 150;

//#define DEBUG_COM                 // enable detailed info (position values) (default: off)
//#define INFO_COM                  // enable info log (state changes) messages via serial 
//#define NO_SERIAL_COM             // disable all serial communication (default: off)

// debug config
// --------------------
const int debounceDelay = 20;
const int readThreshold = 900;


#endif
