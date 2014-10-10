/*
 * slider OS 2
 * for controlling a timelapse slider and triggering the Camera
 * with an arduino, 3 buttons, 2 7-segment-displays, stepper driver
 *
 * version 2.0.0 beta1 (dd.mm.yyyy)
 * Jannik Beyerstedt, Hamburg, Germany | http://jannikbeyerstedt.de | jtByt.Pictures@gmail.com
 * CC BY-NC-SA 3.0
 */


#include "display.h"


// constants to be substituted by parametric ones
const int D1_CLEAR  = 0x00;
const int D1_DP     = d1[7];
const int D2_CLEAR  = 0x00;
const int D2_DP     = d2[7];

const int S_111     = ((d1[3]|D1_DP)<<8) | (d2[3]|D2_DP); // d+DP | d+DP
const int SLD_LEFT  = ((d1[3]|d1[4])<<8) | 0x0000; // d+e | 0
const int SLD_RIGT  = (0x0000 | (d1[0]|d1[1]) ); // 0 | a+b
const int WAIT_GO   = ((d1[0]|d1[2]|d1[3]|d1[4]|d1[5]|d1[6])<<8) | (d2[2]|d2[3]|d2[4]|d2[6]); // a+c+d+e+f+g | c+d+e+g
const int SLIDE_DO  = ((d1[4]|d1[6])<<8) | ( d2[2]|d2[6] ); // e+g | c+g
const int SLIDE_TRG = ((d1[0]|d1[3]|d1[4]|d1[5])<<8) | ( d1[0]|d1[1]|d1[2]|d1[3] ); // a+d+e+f | a+b+c+d
const int ERROR_NUM = (D1_DP<<8)|(d2[0]|d2[3]);


const int OnesArray[11] = {
    d2[0]|d2[1]|d2[2]|d2[3]|d2[4]|d2[5],      // 0 = a+b+c+d+e+f
    d2[1]|d2[2],                              // 1 =   b+c
    d2[0]|d2[1]|d2[3]|d2[4]|d2[6],            // 2 = a+b+  d+e+  g
    d2[0]|d2[1]|d2[2]|d2[3]|d2[6],            // 3 = a+b+c+d+    g
    d2[1]|d2[2]|d2[5]|d2[6],                  // 4 =   b+c+    f+g
    d2[0]|d2[2]|d2[3]|d2[5]|d2[6],            // 5 = a+  c+d+  f+g
    d2[2]|d2[3]|d2[4]|d2[5]|d2[6],            // 6 =     c+d+e+f+g
    d2[0]|d2[1]|d2[2],                        // 7 = a+b+c
    d2[0]|d2[1]|d2[2]|d2[3]|d2[4]|d2[5]|d2[6],// 8 = a+b+c+d+e+f+g
    d2[0]|d2[1]|d2[2]|d2[5]|d2[6],            // 9 = a+b+c+    f+g
    0x0000,                                   // clear
  };
const int TensArray[11] = {
    d1[0]|d1[1]|d1[2]|d1[3]|d1[4]|d1[5],      // 0 = a+b+c+d+e+f
    d1[1]|d1[2],                              // 1 =   b+c
    d1[0]|d1[1]|d1[3]|d1[4]|d1[6],            // 2 = a+b+  d+e+  g
    d1[0]|d1[1]|d1[2]|d1[3]|d1[6],            // 3 = a+b+c+d+    g
    d1[1]|d1[2]|d1[5]|d1[6],                  // 4 =   b+c+    f+g
    d1[0]|d1[2]|d1[3]|d1[5]|d1[6],            // 5 = a+  c+d+  f+g
    d1[2]|d1[3]|d1[4]|d1[5]|d1[6],            // 6 =     c+d+e+f+g
    d1[0]|d1[1]|d1[2],                        // 7 = a+b+c
    d1[0]|d1[1]|d1[2]|d1[3]|d1[4]|d1[5]|d1[6],// 8 = a+b+c+d+e+f+g
    d1[0]|d1[1]|d1[2]|d1[5]|d1[6],            // 9 = a+b+c+    f+g
    0x0000,                                   // clear
  };



void displayNumber (int displayNum) {
  int NumPartTens = 0;  // 10, 20, ...
  int NumPartOnes = 0;  // 1, 2, ...
  
  // special symbols
  if (displayNum == 111) { // value entered symbol
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, (S_111>>8) );
    shiftOut(shiftData, shiftClock, MSBFIRST, S_111 );
    digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 100){ // slide direction 0 (left) symbol
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, (SLD_LEFT>>8) );
    shiftOut(shiftData, shiftClock, MSBFIRST, SLD_LEFT );
    digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 101){ // slide direction 1 (right) symbol
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, (SLD_RIGT>>8) );
    shiftOut(shiftData, shiftClock, MSBFIRST, SLD_RIGT );
    digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 102){ // wait for an input (Go)
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, (WAIT_GO>>8) );
    shiftOut(shiftData, shiftClock, MSBFIRST, WAIT_GO );
    digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 103){ // slide symbol
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, (SLIDE_DO>>8) );
    shiftOut(shiftData, shiftClock, MSBFIRST, SLIDE_DO );
    digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 104){ // trigger symbol
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, (SLIDE_TRG>>8) );
    shiftOut(shiftData, shiftClock, MSBFIRST, SLIDE_TRG );
    digitalWrite(shiftLatch, HIGH);}
  
  else if (displayNum <= 99) {
    if (displayNum < 10) {NumPartTens = 10;}
    else {NumPartTens = displayNum / 10;}
    NumPartOnes = displayNum % 10;
    
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, TensArray[NumPartTens]);
    shiftOut(shiftData, shiftClock, MSBFIRST, OnesArray[NumPartOnes]);
    digitalWrite(shiftLatch, HIGH);
  }
  else { // error, too big number
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, (ERROR_NUM>>8) );
    shiftOut(shiftData, shiftClock, MSBFIRST, ERROR_NUM );
    digitalWrite(shiftLatch, HIGH);
  }
}
