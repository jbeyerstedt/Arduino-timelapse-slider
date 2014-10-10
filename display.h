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
  
  
// define bits for the segments of digit 1 (left)
// a, b, c, d, e, f, g, DP (numbered from the top clockwise, middle segment equals g)
const int d1[8] = {0x08, 0x04, 0x02, 0x10, 0x20, 0x40, 0x80, 0x01}; // tens
const int d2[8] = {0x02, 0x04, 0x08, 0x10, 0x80, 0x40, 0x20, 0x01}; // ones

//const int TensArray[12] = {126, 6, 188, 158, 198, 218, 242, 14, 254, 206, 0, 1}; // 0 to 9 + clear + DP
//const int OnesArray[12] = {222, 12, 182, 62, 108, 122, 248, 14, 254, 110, 0, 1}; // 0 to 9 + clear + DP
  



class display2digits {
  int TensArray[10];
  int OnesArray[10];
  
  public:
  display2digits();
  void displayNumber (int displayNum);
  
  
  
};




#endif
