/* 
 * slow impulses library
 * generates low frequency rectangular signals over a given time
 * for driving stepper motor drivers like Pololu A4988
 *
 * copyright: Jannik Beyerstedt | http://jannikbeyerstedt.de | code@jannikbeyerstedt.de
 * license: http://www.gnu.org/licenses/gpl-3.0.txt GPLv3 License
 *
 * v1.0.2 (24.10.2014)
 */


#include "slow_impulses.h"

 
SlowImpulses::SlowImpulses() {
  no_error = false;        // set default to error
  durationCompare = 0;
  durationStatus = false;
}


void SlowImpulses::init(int pin) {  
  pinNo = pin;             // save pin for later use
  
  noInterrupts();          // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;              // set counter register to 0
  
  TCCR1B |= (1 << WGM12);  // set CTC mode
  switch (PRESCALER) {
    case 1:
      TCCR1B |= 0x01;    
      break;
    case 8:
      TCCR1B |= 0x02;    
      break;
    case 64:
      TCCR1B |= 0x03;    
      break;
    case 256:
      TCCR1B |= 0x04;    
      break;
    case 1024:
      TCCR1B |= 0x05;    
      break;
    default:
      no_error = false;
      break;
  }
  interrupts();            // reenable all interrupts
  
}


boolean SlowImpulses::set(unsigned int frequency, unsigned long duration) {  
  TCNT1 = 0;               // reset counter register
  
  timerCompareVal = CPU_FREQU / PRESCALER / frequency / FUNCT_PERIOD;
  
  if ( (timerCompareVal < 65536) && (timerCompareVal > 0) ) {
    OCR1A = timerCompareVal;  // set counter compare register
    no_error = true;
  }else {
    no_error = false;
  }
  
  // max number of interrupts = desired duration[s] * desired frequency * FUNCT_PERIOD
  durationCompare = long (duration) * frequency * FUNCT_PERIOD / 1000 - 1;
    
  durationStatus = false;    // false -> SlowImpulses interval not started
  
  return no_error;
}


void SlowImpulses::start() {  
  if (no_error) {             // prevent executing with invalid values
    durationCount = 0;        // reset interrupts count
    durationStatus = true;    // reset -> SlowImpulses interval started
    TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  }
}


void SlowImpulses::stop() {
  TIMSK1 ^= (1 << OCIE1A);    // disable timer bit
  digitalWrite(pinNo, LOW);   // disable output
}


boolean SlowImpulses::getStatus() {  
  return durationStatus;
}
