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


// parametric constants for different symbols
const int D1_CLEAR  = 0x00;
const int D1_DP     = d1[7];
const int D2_CLEAR  = 0x00;
const int D2_DP     = d2[7];

const int WAIT_INPT = ((d1[3]|D1_DP)<<8) | (d2[3]|D2_DP); // d+DP | d+DP
//const int WAIT_IDLE = ((d1[1]|d1[2])<<8) | (d2[1]|d2[2]|d2[3]|d2[4]|d2[6]); // b+c | b+c+d+e+g
const int WAIT_IDLE = ((D1_DP)<<8) | 0x0000; // DP | 0
const int INIT_POS  = ((d1[0]|d1[1]|d1[4]|d1[5]|d1[6])<<8) | (d2[2]|d2[3]|d2[4]|d2[6]); // a+b+e+f+g | c+d+e+g
const int INIT_LEFT = ((d1[4]|d1[5])<<8) | 0x0000;  // e+f | 0
const int INIT_RIGT = 0x0000 | (d2[1]|d2[2]); // 0 | b+c

const int MODE_IN   = ((d1[1]|d1[2])<<8) | (d2[2]|d2[4]|d2[6]); // b+c | c+e+g
const int MODE_SL   = ((d1[0]|d1[2]|d1[3]|d1[5]|d1[6])<<8) | (d2[3]|d2[4]|d2[5]);; // a+c+d+f+g | d+e+f
const int MODE_CO   = ((d1[3]|d1[4]|d1[6])<<8) | (d2[2]|d2[3]|d2[4]|d2[6]);; // d+e+g | c+d+e+g

const int SET_TIME  = ((d1[1]|d1[2]|d1[3]|d1[4]|d1[6])<<8) | (d2[4]|d2[6]);; // b+c+d+e+g | e+g
const int SET_INT   = ((d1[1]|d1[2])<<8) | (d2[2]|d2[4]|d2[6]); // b+c | c+e+g
const int DIR_LEFT  = ((d1[3]|d1[4])<<8) | 0x0000; // d+e | 0
const int DIR_RIGT  = (0x0000 | (d2[0]|d2[1]) ); // 0 | a+b

const int WAIT_GO   = ((d1[0]|d1[2]|d1[3]|d1[4]|d1[5]|d1[6])<<8) | (d2[2]|d2[3]|d2[4]|d2[6]); // a+c+d+e+f+g | c+d+e+g

const int SLIDE_MVE = ((d1[4]|d1[6])<<8) | ( d2[2]|d2[6] ); // e+g | c+g
const int SLIDE_TRG = ((d1[0]|d1[3]|d1[4]|d1[5])<<8) | ( d2[0]|d2[1]|d2[2]|d2[3] ); // a+d+e+f | a+b+c+d
const int SLIDE_INT = ((d1[6])<<8) | (d2[6]);; // g | g

const int ERROR_NUM = (D1_DP<<8)|(d2[0]|d2[3]); // DP | a+d

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
    0x00,                                     // clear
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
    0x00,                                     // clear
  };



void shift2digits (int shift16bits) {
  digitalWrite(shiftLatch, LOW);
  shiftOut(shiftData, shiftClock, MSBFIRST, (shift16bits>>8) );
  shiftOut(shiftData, shiftClock, MSBFIRST, shift16bits );
  digitalWrite(shiftLatch, HIGH);
}



void displayNumber (int displayNum) {  
  int numbersBits = 0x0000;
  
  if ( (displayNum <= 99) && (displayNum >= -9) ) { // [-9, 99]
    if ( displayNum < 0 ) {       // [-9, -1]
      numbersBits  = (d1[6]<<8); // dash
      numbersBits |= OnesArray[-displayNum];
    }else if (displayNum < 10) {  // [ 0,  9]
      numbersBits  = D1_CLEAR<<8;
      numbersBits |= OnesArray[displayNum];
    }else {                       // [10, 99]
      numbersBits  = (TensArray[ displayNum / 10 ] << 8);
      numbersBits |= OnesArray[ displayNum % 10 ];
    }
    
    shift2digits(numbersBits);

  }
  else { // error, too big number
    shift2digits(ERROR_NUM );
  }
}



void displaySymbol (mySymbols displaySym) {
  switch (displaySym) {
    case waitInput:
      shift2digits(WAIT_INPT);
      break;
    case waitIdle:
      shift2digits(WAIT_IDLE);
      break;
    case initPos:
      shift2digits(INIT_POS);
      break;
    case initLeft:
      shift2digits(INIT_LEFT);
      break;
    case initRigt:
      shift2digits(INIT_RIGT);
      break;
    case modeIn:
      shift2digits(MODE_IN);
      break;
    case modeSl:
      shift2digits(MODE_SL);
      break;
    case modeCo:
      shift2digits(MODE_CO);
      break;
    case setTime:
      shift2digits(SET_TIME);
      break;
    case setInt:
      shift2digits(SET_INT);
      break;
    case dirLeft:
      shift2digits(DIR_LEFT);
      break;
    case dirRigt:
      shift2digits(DIR_RIGT);
      break;
    case waitGo:
      shift2digits(WAIT_GO);
      break;
    case slideMve:
      shift2digits(SLIDE_MVE);
      break;
    case slideTrg:
      shift2digits(SLIDE_TRG);
      break;
    case slideInt:
      shift2digits(SLIDE_INT);
      break;
    default:
      Serial.println("ERROR: invalid symbol requested");
      break;
  }
}
