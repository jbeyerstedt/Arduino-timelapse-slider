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
  library for displaying numbers and specific symbols on 2 7-segment-displays
  
  usage: displayNum = 
  0 - 99 -> normal decimal number
  100    -> symbol: slide direction 0 (right)
  101    -> symbol: slide direction 1 (left)
  102    -> symbol: wait for an input to start (Go)
  103    -> symbol: carriage is moving
  104    -> symbol: camera is being triggered
  111    -> symbol: value entered
*/


#ifndef __display_h__
#define __display_h__


#include <Arduino.h>
#include "config.h"


// define bits for the segments of digit 1 and 2 (from the left)
// a, b, c, d, e, f, g, DP (numbered from the top clockwise, middle segment equals g)
const int d1[8] = {0x08, 0x04, 0x02, 0x10, 0x20, 0x40, 0x80, 0x01}; // tens
const int d2[8] = {0x02, 0x04, 0x08, 0x10, 0x80, 0x40, 0x20, 0x01}; // ones
  

// enum for accessing all symbols more easy than with numbers
typedef enum {
  waitInput,  // some input is expected
  waitIdle,   // idle / manual control mode
  initPos,    // enter initial position
  initLeft,   // initial position left
  initRigt,   // initial position right
  modeIn,     // interval trigger mode (IN mode)
  modeSl,     // timelapse slider mode (SL mode)
  modeCo,     // movie move (CO mode)
  setTime,    // set total time
  setInt,     // set interval
  dirLeft,    // direction left
  dirRigt,    // direction right
  waitGo,     // wait for GO
  slideMve,   // slide moving carriage
  slideTrg,   // slide triggering camera
  slideInt    // slide waiting for next triggering (only IN mode)
}mySymbols;


// function for displaying numbers
void displayNumber (int displayNum);

// function for displaying the symbols defined in the enum
void displaySymbol (mySymbols displaySym);



#endif
