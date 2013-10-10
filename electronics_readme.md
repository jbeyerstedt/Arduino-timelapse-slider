# Arduino-Timelapse-Slider
by jtByt.Pictures, Jannik Beyerstedt from Hamburg, Germany  
[jannik.beyerstedt.de](http://jannik.beyerstedt.de) | [github](https://github.com/jtByt-Pictures)  
**electronics documentation**  



#### about this / über dieses Dokument  
**english:** 
This document describes the electronics part of the Arduino timelapse slider, but it will not be a complete instuction to rebuild everything I made. It´s more like all the parts you´ll need to build it, but there is enough you must do by yourself. I can´t explain every wire I used.  
Kepp in mind, that your camera may be triggered in a different way. Canon DSLRs are easy. You only must connect two pins of the remote connector. Some cameras are using different resistors to detect focus and trigger. Please ask google to find out more about your camera.

**deutsch:** 
deutscher Text folgt noch.


## parts I used / verwendete Teile:
**english:**  

amount | description  
--	| ---  
1	| Arduino Uno Rev3
1	| A4988 Stepper Motor Driver Carrier
2	| 8 bit shift registers
2	| 7 segment display (+ resistors)
1	| miniature relais (to trigger my Canon DSLR)
3	| miniature buttons
1	| T-connector (used at LiPo-batteries) for power supply

*	dot matrix circuit boards
*	pins to connect to the arduino
*	DIN-connectors for the wires to camera and motor
*	jumpers for different modes of the stepper motor driver
*	small wires with different colors to connect everything

**weblinks**

*	stepper motor driver: [http://www.exp-tech.de/Shields/A4988-Stepper-Motor-Driver-Carrier.html]()
*	stepper motor [http://www.exp-tech.de/Servos-und-Motoren/Stepper-Motor-Bipolar-200-Steps-Rev-35x36mm-2-7V-1000mA.html]()


**deutsch:** 

* deutscher Text folgt noch.



## installation notes / Aufbauanweisungen:
##### english:
###### display

*	you can drive one seven-segment-display each with one 8-bit shift register and you can connect two shift registers at one output of your arduino (but you will need tree pins for clock, latch and data)
*	connect each output of your shift register to one segment of the display with an restistor between them. Keep in mind, whether the display is collected kathode or anode and whether you can get positive or negative voltage from your shift register.
*	driving a shift register with an arduino is explained at the arduino website.
*	If you use my code copy and paste, than you have to check whether everything is connected to the same pin I used. Otherwise you must change the constants at the beginning of the code.

###### stepper motor

*	connect the stepper motor driver your arduino, an power supply and your stepper motor. Do never switch the driver on without an stepper motor (see manual of the driver).
*	read the manual and try the driver with a code snippet. If that works, you can put that code in your project.
*	If you use my code copy and paste, than you have to check whether everything is connected to the same pin I used. Otherwise you must change the constants at the beginning of the code.


**deutsch:** 

* deutscher Text folgt noch.