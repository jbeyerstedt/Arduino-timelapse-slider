// slider OS
// version 0.3
// Jannik Beyerstedt

// debug variables
int delayVal = 20;
int numberLimitSlideTime = 30;    // maximum time input to slide (in minutes)
int numberLimitTriggerTime = 10;  // maximum trigger interval time (in seconds)

// constants for the slider (mechanical)
int triggerDuration = 500;  // how long should your camera be triggered (exposure time is still set in camera)
int maxSteps = 2200;        // amount of steps to slide the slider along
int workingDelay = 72;      // time to execute one the slide code in ms (7200 ms for 100 steps measured)

// setup I/O and helper integers
const int trigger = 2;
// const int servoTilt = 3;

const int shiftLatch = 12; // RCK (register clock)
const int shiftClock = 9; // SCK (shift clock)
const int shiftData = 8; // SI (serial in) 
const int ledPin = 13;
const int stepperStep = 6;
const int stepperDir = 7;
const int stepperSleep = 4;

const int buttonPlus = A1;
const int buttonMinus = A2;
const int buttonEnter = A0;
// button helper integers
int readValuePlus = 0;
int readValueMinus = 0;
int readValueEnter = 0;
int lastPlus = 0;
int lastMinus = 0;
int lastEnter = 0;

int choosenNumber = 0;

// values before conversion
int setSlideTime = 0;
int slideDirection = 0; // 0 = right, 1 = left
int setTriggerTime = 0;

// helper for the 3 main while-loops
int loop1SetupHelper = 0;
int loop2SetupHelper = 0;
int loop3SetupHelper = 0;
int loop4ConvertHelper = 0;
int loop4WaitStart = 0;
int loop5AbortSlide = 0;

// counters for THE SLIDE
int totalStepCounter = 0;
int intervalStepCounter = 0;

// converted values
int slideStepsPerInterval = 0;
int slideStepDelay = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(shiftLatch, OUTPUT);
  pinMode(shiftClock, OUTPUT);
  pinMode(shiftData, OUTPUT);
  
  pinMode(buttonPlus, INPUT);
  pinMode(buttonMinus, INPUT);
  pinMode(buttonEnter, INPUT);
  
  pinMode(stepperStep, OUTPUT);
  pinMode(stepperDir, OUTPUT);
  pinMode(stepperSleep, OUTPUT);
  
  pinMode(trigger, OUTPUT);
  
  digitalWrite(stepperSleep, LOW); // set stepper driver to sleep
  
  Serial.begin(9600);
}

void loop() {
  displayNumber(111);
  
// SETUP all the parameters
// LOOP 1
  while (loop1SetupHelper == 0){// set slideTime
    readValuePlus = digitalRead(buttonPlus);
    delay(delayVal);
    if (readValuePlus == HIGH && lastPlus != readValuePlus){
      choosenNumber ++;
      if(choosenNumber < 0) {choosenNumber = numberLimitSlideTime;} if(choosenNumber > numberLimitSlideTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastPlus = readValuePlus;
      delay(delayVal);}
    if (readValuePlus == LOW && lastPlus != readValuePlus) {
      lastPlus = readValuePlus; delay (delayVal);}
    
    readValueMinus = digitalRead(buttonMinus);
    delay(delayVal);
    if (readValueMinus == HIGH && lastMinus != readValueMinus){
      choosenNumber --;
      if(choosenNumber < 0) {choosenNumber = numberLimitSlideTime;} if(choosenNumber > numberLimitSlideTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastMinus = readValueMinus;
      delay(delayVal);}
    if (readValueMinus == LOW && lastMinus != readValueMinus) {
      lastMinus = readValueMinus; delay(delayVal);}
    
    readValueEnter = digitalRead(buttonEnter);
    delay(delayVal);
    if (readValueEnter == HIGH && lastEnter != readValueEnter){
      setSlideTime = choosenNumber;
      choosenNumber = 0;
      displayNumber(111);
      lastEnter = readValueEnter;
      loop1SetupHelper = 1; // kick me out of the 1st setup loop
      delay(delayVal);
      Serial.print("setSlideTime entered: "); Serial.println(setSlideTime);}
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}   
  }
  
// LOOP 2  
  while (loop2SetupHelper == 0){   // set slideDirection
    readValuePlus = digitalRead(buttonPlus);
    delay(delayVal);
    if (readValuePlus == HIGH && lastPlus != readValuePlus){
      slideDirection = 0; // right
      displayNumber(100+slideDirection);
      lastPlus = readValuePlus;
      delay(delayVal);}
    if (readValuePlus == LOW && lastPlus != readValuePlus) {
      lastPlus = readValuePlus; delay (delayVal);}
    
    readValueMinus = digitalRead(buttonMinus);
    delay(delayVal);
    if (readValueMinus == HIGH && lastMinus != readValueMinus){
      slideDirection = 1; // left
      displayNumber(100+slideDirection);
      lastMinus = readValueMinus;
      delay(delayVal);}
    if (readValueMinus == LOW && lastMinus != readValueMinus) {
      lastMinus = readValueMinus; delay(delayVal);}
    
    readValueEnter = digitalRead(buttonEnter);
    delay(delayVal);
    if (readValueEnter == HIGH && lastEnter != readValueEnter){
      displayNumber(111);
      lastEnter = readValueEnter;
      loop2SetupHelper = 1; // kick me out of the 2nd setup loop
      delay(delayVal);
      Serial.print("SlideDirection entered: "); Serial.println(slideDirection);}
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}
  }
  
// LOOP 3
  while (loop3SetupHelper == 0){   // set triggerTime
    readValuePlus = digitalRead(buttonPlus);
    delay(delayVal);
    if (readValuePlus == HIGH && lastPlus != readValuePlus){
      choosenNumber ++;
      if(choosenNumber < 0) {choosenNumber = numberLimitTriggerTime;} if(choosenNumber > numberLimitTriggerTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastPlus = readValuePlus;
      delay(delayVal);}
    if (readValuePlus == LOW && lastPlus != readValuePlus) {
      lastPlus = readValuePlus; delay (delayVal);}
    
    readValueMinus = digitalRead(buttonMinus);
    delay(delayVal);
    if (readValueMinus == HIGH && lastMinus != readValueMinus){
      choosenNumber --;
      if(choosenNumber < 0) {choosenNumber = numberLimitTriggerTime;} if(choosenNumber > numberLimitTriggerTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastMinus = readValueMinus;
      delay(delayVal);}
    if (readValueMinus == LOW && lastMinus != readValueMinus) {
      lastMinus = readValueMinus; delay(delayVal);}
    
    readValueEnter = digitalRead(buttonEnter);
    delay(delayVal);
    if (readValueEnter == HIGH && lastEnter != readValueEnter){
      setTriggerTime = choosenNumber;
      choosenNumber = 0;
      displayNumber(111);
      lastEnter = readValueEnter;
      loop3SetupHelper = 1; // kick me out of the 3rd setup loop
      delay(delayVal);
      Serial.print("setTriggerTime entered: "); Serial.println(setTriggerTime);}
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}
  }
  
  
// CONVERT the input values
// LOOP 4
  while (loop4ConvertHelper == 0) {
    
    // calculate velocity:
        
    // maxSteps / setSlideTime [min] * 60 [sek/min] = stepsPerSecond
    // 1000 [ms] / stepsPerSecond = timePerStep [ms/step]
    int stepsPerSecond = maxSteps / setSlideTime / 60;
    int timePerStep = 1000 / stepsPerSecond;
    slideStepDelay = timePerStep - workingDelay;
    if (slideStepDelay <= 1) {slideStepDelay = 1;} // safety that slideStepDelay is not too small
  
    if (setTriggerTime == 0) { // movie mode
      slideStepsPerInterval = maxSteps;
      Serial.println("movie mode active");
    }
    else { // normal timelapse operation
      // slideStepsPerInterval = stepsPerSecond * setTriggerTime
      slideStepsPerInterval = stepsPerSecond * setTriggerTime;
    }
    
    loop4ConvertHelper ++;
    Serial.print("slideStepDelay: "); Serial.println(slideStepDelay);
    Serial.print("slideStepsPerInterval: ");Serial.println(slideStepsPerInterval);
    Serial.println("converted");
   }
   
  // prepare some more variables (at this time because of a second loop run)
  int waitHepler = 0;
  int wakeupHelper = 0;
  int displaySlideHelper = 0;
  
  
// WAIT FOR START
// LOOP 4 wait
  while (loop4WaitStart == 0) {
    for (waitHepler = 0; waitHepler == 0; waitHepler++) { // display wait-sign only once in this loop
      displayNumber(102);}
    
    readValueEnter = digitalRead(buttonEnter);
    delay(delayVal);
    if (readValueEnter == HIGH && lastEnter != readValueEnter){
      lastEnter = readValueEnter;
      loop4WaitStart ++;
      delay(delayVal);
      Serial.println("started");}
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
      lastEnter = readValueEnter; delay(delayVal);}
  }
  
// DO THE SLIDE
// LOOP 5
  while (totalStepCounter <= maxSteps && loop5AbortSlide == 0) {
    
    // wake up stepper driver and set direction only once
    for (wakeupHelper = 0; wakeupHelper == 0; wakeupHelper++) {digitalWrite(stepperSleep, HIGH); delay(50); digitalWrite(stepperDir, slideDirection);}
    // display slide symbol only at beginning of the slide inerval
    for (displaySlideHelper = 0; displaySlideHelper == 0; displaySlideHelper ++) {displayNumber(103);}
    
    // step trigger interval
    if (intervalStepCounter < slideStepsPerInterval) { // step
      digitalWrite(stepperStep, HIGH);
      delay(slideStepDelay);
      digitalWrite(stepperStep, LOW);
      intervalStepCounter ++;
      totalStepCounter ++;}
    else if (setTriggerTime >= 1) { // trigger if setTriggerTime is not 0
      displayNumber(104);
      digitalWrite(trigger, HIGH);
      delay (triggerDuration);
      digitalWrite(trigger, LOW);
      intervalStepCounter = 0;
      displaySlideHelper = 0;}
    else {                          // do not trigger in movie mode (setTriggerTime == 0) (backup-function, because of movie mode in conversion-loop
      intervalStepCounter = 0;}
    
    // abort the slide with enter button
    readValueEnter = digitalRead(buttonEnter);
    delay(delayVal);
    if (readValueEnter == HIGH && lastEnter != readValueEnter){
      lastEnter = readValueEnter;
      loop5AbortSlide ++;
      delay(delayVal);
      Serial.println("escape the slide");}
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}
  }
  
// END and RESET VALUES FOR ANOTHER START
  slideDirection = !slideDirection;
  loop1SetupHelper = 0;
  loop2SetupHelper = 0;
  loop3SetupHelper = 0;
  loop4ConvertHelper = 0;
  loop4WaitStart = 0;
  loop5AbortSlide = 0;
  totalStepCounter = 0;
  intervalStepCounter = 0;
  
  digitalWrite(stepperSleep, LOW); // set steper driver to sleep
  
  Serial.println("reset");

}




// method for displaying a 2 digit number with 2 shift registers
// for special symbols see the code annotations itself

void displayNumber (int displayNum) {
  int NumPartTens = 0;  // 10, 20, ...
  int NumPartOnes = 0; // 1, 2, ...
  int TensArray[12] = {126, 6, 188, 158, 198, 218, 242, 14, 254, 206, 0, 1}; // 0 - 9 + clear + DP
  int OnesArray[12] = {222, 12, 182, 62, 108, 122, 248, 14, 254, 110, 0, 1}; // 0 - 9 + clear + DP
  
  // special symbols
  if (displayNum == 111) { // value entered symbol
    digitalWrite(shiftLatch, LOW); shiftOut(shiftData, shiftClock, MSBFIRST, 17); shiftOut(shiftData, shiftClock, MSBFIRST, 17); digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 100){ // slide direction 0 (right) symbol
    digitalWrite(shiftLatch, LOW); shiftOut(shiftData, shiftClock, MSBFIRST, 0); shiftOut(shiftData, shiftClock, MSBFIRST, 6); digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 101){ // slide direction 1 (left) symbol
    digitalWrite(shiftLatch, LOW); shiftOut(shiftData, shiftClock, MSBFIRST, 48); shiftOut(shiftData, shiftClock, MSBFIRST, 0); digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 102){ // wait for an input (Go)
    digitalWrite(shiftLatch, LOW); shiftOut(shiftData, shiftClock, MSBFIRST, 250); shiftOut(shiftData, shiftClock, MSBFIRST, 184); digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 103){ // slide symbol
    digitalWrite(shiftLatch, LOW); shiftOut(shiftData, shiftClock, MSBFIRST, 160); shiftOut(shiftData, shiftClock, MSBFIRST, 40); digitalWrite(shiftLatch, HIGH);}
  else if (displayNum == 104){ // trigger symbol
    digitalWrite(shiftLatch, LOW); shiftOut(shiftData, shiftClock, MSBFIRST, 120); shiftOut(shiftData, shiftClock, MSBFIRST, 30); digitalWrite(shiftLatch, HIGH);}
  
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
    shiftOut(shiftData, shiftClock, MSBFIRST, 1);
    shiftOut(shiftData, shiftClock, MSBFIRST, 18); 
    digitalWrite(shiftLatch, HIGH);
  }
}
