# Arduino-Timelapse-Slider
# Arduino - timelapse-slider
by Jannik Beyerstedt from Hamburg, Germany  
[jannikbeyerstedt.de](http://jannikbeyerstedt.de) | [Github](https://github.com/jbeyerstedt)  
**license:** GNU GPL v3

## Hardware Documentation 
This is the documentation for the technical drawings and all the hardware.
all the technical drawings are specific to the parts I used. If you use other parts, some measurements must be changed.  


## basic Measurements

* distance between the tubes (inner edges): 45 mm
* tubes diameter: 12 mm
* bearings: 10mm x 4 mm


## parts I used

amount | description  
--	| ---  
12	| ball bearings MR 104 ZZ / 4 x 10 x 4 mm  
8	| zylindrical head screws M4x15  
4	| zylindrical head screws M4x20  
24	| nuts M4 (8 as distance roll, 16 as counter nut and for the correct distance)  
4	| rope reel with ball bearings (detailed information see weblink section) <br> outer diameter 24 mm, inner diameter 20 mm, height 11 mm  
4	| screws M5x15  
4	| nuts M5 (as counter nut and distance roll)  
1	| M5 screw and washer to fasten the quick release plate
1	| manfrotto 200PL quick release plate
1	| manfrotto ball head (using the 1/4 zoll screw of the QR-plate to fasten it)
1 m | toothed belt 5M-9 
2 	| toothed disk (gear) 12-5M-9 
4	| tiny M3 screws to secure the belt
1	| stepper motor (bipolar, 2.7V, 1000mA) by pololu

*	small screws to fasten the stepper motor


**weblinks**  

*	rope reel: [http://jost-technik.de/Klappensteller/Zubehoer---Sonstiges/Umlenkrolle-fuer-Huehnerklappe.html]()
*	ball brearings from ebay
*	toothed belt and disk: [https://zahnriemen24.de]()
*	stepper motor [http://www.exp-tech.de/Servos-und-Motoren/Stepper-Motor-Bipolar-200-Steps-Rev-35x36mm-2-7V-1000mA.html]()


## installation notes:
##### english:
###### tubes, tube support and stepper motor
*	build one tube support, that holds the two tubes and a gear for the belt
*	build another tube support, that can hold the stepper motor with the other gear on it.

###### inner sledge
*	the inner sledge uses the 10 mm bearings to be held in position. On every side (left and right) are 2 bearings at the top, 2 at the bottom and two on the inner side. All of them are on M4-Screws.
* 	the inner bearings have a nut on top and bottom as distance tubes, you can also use 3 mm thick tubes instead. The screw of these bearings is also used to hold the tree parts of the inner sledge together. They must be at least 15 mm long
* 	the outer bearings will be placed on the shorter M4 screws with two nut (each 3 mm) to hold them in place and keep them at the correct distance from the sledge.

###### outer sledge
*   put the rope reel on the M5 screw and secure with one nut. Screw it carefully to the aluminium base AND put it on the rails (tubes).

###### toothed belt
*	cut the belt in two pieces, drill a hole for the M3 screws betwen the tooths to one side of each belt
*	hold the belt on the slider to get the correct legth and mark the position for the screw. Kepp in mind, that the two belts will have different lenghts.
*	drill the other two holes
*	screw the belt to your slider
