// slider OS
// version 0.1
// Jannik Beyerstedt

// debug variables
int delayVal = 20;
int numberLimitSlideTime = 30;
int numberLimitTriggerTime = 10;

// constants for the slider
int triggerDuration = 500;
int maxSteps = 400;

// setup I/O and helper integers
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

const int trigger = 2;
// const int servoTilt = 3;

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

// counters for THE SLIDE
int totalStepCounter = 0;
int intervalStepCounter = 0;

// converted values
int stepsPerInterval = 0;

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
  
  digitalWrite(stepperSleep, LOW); // set stepper driver to sleep
  
  digitalWrite(shiftLatch, LOW);
  shiftOut(shiftData, shiftClock, MSBFIRST, 126); // decimals
  shiftOut(shiftData, shiftClock, MSBFIRST, 222); // ones
  digitalWrite(shiftLatch, HIGH);
  
  Serial.begin(9600);
  
}

void loop() {
  displayNumber(111);
  // setup all the parameters
  while (loop1SetupHelper == 0){   // set slideTime
    
    readValuePlus = digitalRead(buttonPlus);
    delay(delayVal);
    if (readValuePlus == HIGH && lastPlus != readValuePlus){
      choosenNumber ++;
      if(choosenNumber < 0) {choosenNumber = numberLimitSlideTime;} if(choosenNumber > numberLimitSlideTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastPlus = readValuePlus;
      delay(delayVal);
      // for debug
      Serial.print("setSlideTime: "); Serial.println(choosenNumber);
    }
    if (readValuePlus == LOW && lastPlus != readValuePlus) {
      lastPlus = readValuePlus; delay (delayVal);}
    
    readValueMinus = digitalRead(buttonMinus);
    delay(delayVal);
    if (readValueMinus == HIGH && lastMinus != readValueMinus){
      choosenNumber --;
      if(choosenNumber < 0) {choosenNumber = numberLimitSlideTime;} if(choosenNumber > numberLimitSlideTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastMinus = readValueMinus;
      delay(delayVal);
      // for debug
      Serial.print("setSlideTime: "); Serial.println(choosenNumber);
    }
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
      
      // for debug
      Serial.print("setSlideTime entered: "); Serial.println(setSlideTime);
    }
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}   
  }
  
  
  
  while (loop2SetupHelper == 0){   // set slideDirection
    readValuePlus = digitalRead(buttonPlus);
    delay(delayVal);
    if (readValuePlus == HIGH && lastPlus != readValuePlus){
      slideDirection = 1; // right
      displayNumber(100+slideDirection);
      lastPlus = readValuePlus;
      delay(delayVal);
      // for debug
      Serial.print("setSlideDirection: "); Serial.println(slideDirection);
    }
    if (readValuePlus == LOW && lastPlus != readValuePlus) {
      lastPlus = readValuePlus; delay (delayVal);}
    
    readValueMinus = digitalRead(buttonMinus);
    delay(delayVal);
    if (readValueMinus == HIGH && lastMinus != readValueMinus){
      slideDirection = 0; // left
      displayNumber(100+slideDirection);
      lastMinus = readValueMinus;
      delay(delayVal);
      // for debug
      Serial.print("setSlideDirection: "); Serial.println(slideDirection);
    }
    if (readValueMinus == LOW && lastMinus != readValueMinus) {
      lastMinus = readValueMinus; delay(delayVal);}
    
    readValueEnter = digitalRead(buttonEnter);
    delay(delayVal);
    if (readValueEnter == HIGH && lastEnter != readValueEnter){
      displayNumber(111);
      lastEnter = readValueEnter;
      loop2SetupHelper = 1; // kick me out of the 2nd setup loop
      delay(delayVal);
      
      // for debug
      Serial.print("SlideDirection entered: "); Serial.println(slideDirection);
    }
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}
  }
  
  
  
  while (loop3SetupHelper == 0){   // set triggerTime
    readValuePlus = digitalRead(buttonPlus);
    delay(delayVal);
    if (readValuePlus == HIGH && lastPlus != readValuePlus){
      choosenNumber ++;
      if(choosenNumber < 0) {choosenNumber = numberLimitTriggerTime;} if(choosenNumber > numberLimitTriggerTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastPlus = readValuePlus;
      delay(delayVal);
      // for debug
      Serial.print("setTriggerTime: "); Serial.println(choosenNumber);
    }
    if (readValuePlus == LOW && lastPlus != readValuePlus) {
      lastPlus = readValuePlus; delay (delayVal);}
    
    readValueMinus = digitalRead(buttonMinus);
    delay(delayVal);
    if (readValueMinus == HIGH && lastMinus != readValueMinus){
      choosenNumber --;
      if(choosenNumber < 0) {choosenNumber = numberLimitTriggerTime;} if(choosenNumber > numberLimitTriggerTime) {choosenNumber = 0;}
      displayNumber(choosenNumber);
      lastMinus = readValueMinus;
      delay(delayVal);
      // for debug
      Serial.print("setTriggerTime: "); Serial.println(choosenNumber);
    }
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
      
      // for debug
      Serial.print("setTriggerTime entered: "); Serial.println(setTriggerTime);
    }
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}
  }
  
  // prepare some variables (at this time because of a second loop run)
  int waitHepler = 0;
  int wakeupHelper = 0;
  
  // CONVERT
  // startParameter umrechnen !!
  while (loop4ConvertHelper == 0) {
    // code
    // stepsPerInterval = 100; // oder was auch immer errechnet wird
    
    stepsPerInterval = setSlideTime * 40; // code for testing
    
    loop4ConvertHelper ++;
    
    // for debug
    Serial.println("converted");
  }
  
  // WAIT FOR START
  while (loop4WaitStart == 0) {
    for (waitHepler = 0; waitHepler == 0; waitHepler++) { // display wait-code only once
      displayNumber(102);
    }
    
    readValueEnter = digitalRead(buttonEnter);
    delay(delayVal);
    if (readValueEnter == HIGH && lastEnter != readValueEnter){
      lastEnter = readValueEnter;
      loop4WaitStart ++;
      delay(delayVal);
      
      // for debug
      Serial.println("started: ");
    }
    if (readValueEnter == LOW && lastEnter != readValueEnter) {
    lastEnter = readValueEnter; delay(delayVal);}
  }
  
  // DO THE SLIDE
  while (totalStepCounter <= maxSteps) {
    displayNumber(103);
    
    for (wakeupHelper = 0; wakeupHelper == 0; wakeupHelper++) { // wake up stepper driver and set direction only once
      digitalWrite(stepperSleep, HIGH);
      delay(50);
      digitalWrite(stepperDir, slideDirection);
    }
    
    if (intervalStepCounter < stepsPerInterval) {
      // do one Step
      digitalWrite(stepperStep, HIGH);
      delay(15);
      digitalWrite(stepperStep, LOW);
      
      intervalStepCounter ++;
      totalStepCounter ++;
    }
    else {
      displayNumber(104);
      digitalWrite(trigger, HIGH);
      delay (triggerDuration);
      digitalWrite(trigger, LOW);
      intervalStepCounter = 0;
    }
  }
  
  // END, RESET VALUES FOR ANOTHER START
  if (slideDirection == 0) {
    slideDirection = 1;
  }
  if (slideDirection == 1) {
    slideDirection = 0;
  }
  
  loop1SetupHelper = 0;
  loop2SetupHelper = 0;
  loop3SetupHelper = 0;
  loop4ConvertHelper = 0;
  loop4WaitStart = 0;
  totalStepCounter = 0;
  intervalStepCounter = 0;
  
  // set steper driver to sleep
  digitalWrite(stepperSleep, LOW);
  
  // for debug
  Serial.println("reset");
}




// method for displaying a 2 digit number with 2 shift registers
// 101: display shows two decimal points
// one decimal Point and an eight ist the error for a too big number > 99

void displayNumber (int displayNum) {
  int NumPartTens = 0;  // 10, 20, ...
  int NumPartOnes = 0; // 1, 2, ...
  int TensArray[10] = {126, 6, 188, 158, 198, 218, 242, 14, 254, 206};
  int OnesArray[10] = {222, 12, 182, 62, 108, 122, 248, 14, 254, 110};
  
  if (displayNum == 111) { 
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, 1);
    shiftOut(shiftData, shiftClock, MSBFIRST, 1);
    digitalWrite(shiftLatch, HIGH);
  }
  else if (displayNum == 100){ // slide direction 0
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, 178);
    shiftOut(shiftData, shiftClock, MSBFIRST, 144);
    digitalWrite(shiftLatch, HIGH);
  }
  else if (displayNum == 101){ // slide direction 1
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, 178);
    shiftOut(shiftData, shiftClock, MSBFIRST, 6);
    digitalWrite(shiftLatch, HIGH);
  }
  else if (displayNum == 102){ // wait for an input
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, 128);
    shiftOut(shiftData, shiftClock, MSBFIRST, 40);
    digitalWrite(shiftLatch, HIGH);
  }
  else if (displayNum == 103){ // slide
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, 8);
    shiftOut(shiftData, shiftClock, MSBFIRST, 2);
    digitalWrite(shiftLatch, HIGH);
  }
  else if (displayNum == 104){ // trigger
    digitalWrite(shiftLatch, LOW);
    shiftOut(shiftData, shiftClock, MSBFIRST, 1);
    shiftOut(shiftData, shiftClock, MSBFIRST, 0);
    digitalWrite(shiftLatch, HIGH);
  }
  
  else if (displayNum <= 99) {
    if (displayNum < 10) {NumPartTens = 0;}
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
