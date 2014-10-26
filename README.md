# Arduino-timelapse-slider
by jtByt.Pictures, Jannik Beyerstedt from Hamburg, Germany  
[jannik.beyerstedt.de](http://jannik.beyerstedt.de) | [Github](https://github.com/jtByt-Pictures)  
**software documentation**  


#### about this document  
This readme contains the introduction of the project, changelog and the **user manual** for the code/ software. Check also the "hardware_readme.md" and "electronics_readme.md".


## introduction
I'm beginning shooting time lapses with my DSLR camera and wanted to have a slider for more interesting camera movements in my time lapses. So I began to write some code for the Arduino UNO board to control a stepper motor and the trigger of my camera.
To control this little thing I will use three buttons and two 7 digit displays.

You can find photos I made and in the future also videos on my website [jtByt-Pictures.de](http://jtByt-Pictures.de "my homepage").


## user manual  
#### general information
1. the total slide time influences the sliding speed!

#### step by step guide
1. switch on power
2. The software initializes
	1. give it the starting point (left or right)
3. choose mode: interval trigger (**in**terval), timelapse slide (**sl**ide), movie (**co**ntinuous)
4. manual control (all modes the same)
	1. move carriage with plus and minus buttons
5. at interval trigger mode (it´s an interval trigger only)
	1. set trigger interval
	2. set direction
	3. push enter to begin
	4. end with enter -> back to step 4
6. at timelapse slide mode (interval trigger and carriage movement)
	1. set total slide time
	2. set direction
	3. set trigger interval
	4. push enter to begin
	5. it´ll end if time is elapsed or endstop hit -> back to step 4
	6. or abort by clicking enter -> back to step 4
7. at movie mode (no trigger, only carriage movement)
	1. set total movement time
	2. set direction
	3. push enter to begin
	4. it´ll end if time is elapsed or endstop hit -> back to step 4
	5. or abort by clicking enter -> back to step 4
8. change mode by reconnecting power or resetting the arduino

Symbols:

- In -> interval mode
- SL -> slider mode
- co -> movie mode (continuous)
- dr -> set total time (drive)
- In -> set trigger interval
- ┌ ┐ -> moving carriage
- \[ \] -> triggering camera
- – –  -> waiting for next triggering (interval trigger mode only)
- . -> idle/ manual drive possible
 

## setup manual
This will explain the options that can be set in the config.h file. But it´s quite well commented in the file itself as well.  
more follows soon / mehr folgt bald  


## what should the software do?
#### in general
* input values: time to slide the whole slider, slide direction, trigger interval
* have a stop button to escape the slide
* trigger the camera
* stop the slide at triggering the camera
* tree buttons as inputs (plus, minus, enter)
* drive two 8-bit shift registers for the displays
* drive a stepper motor (via stepper motor driver IC)



#### additional in version 1.0
* movie mode with constant velocity
* (more or less) smart symbols on the display indicating the direction, current mode and errors

#### additional in version 2.0
* INFO: movie and interval mode are disabeled until ready in v2.1
* coded as finite state machine
* 3 modes for timelapse, timelapse with slider, movie
* virtual endstops
* steps are done in the background
	* faster stepping possible
	* more responsive exit from slide
* set initial carriage position
* remember last settings (start at last entered number for slideTime, etc.)
* better virtual endstop methods
* some extensive serial prints can be disabled
* functionality for pure interval triggering added 

#### additional in version 2.1
* functionality for movie mode



## changelog
* 1.0.1:
	* setSlideTime=0 will get an division through 0 in the calculation which causes malfunction (because stepsPerInterval will be 0)
	* corrected the calculation of the stepsPerInterval at high speed / setSlideTime < 3 / calculated stepsPerSecond < than the highest possible speed
* 1.0.2:  
	* little modification at big slide times
* 2.0 beta1 :
	* INFO: movie and interval mode are disabeled until ready in v2.1
	* all new rewritten version. New software concept and it´s C++ now.
	* 3 modes for timelapse, timelapse with slider, movie
	* classes for:
		* the slider (carriage)
		* the stepper motort
		* the buttons
	* display.h restructured and enhanced:
		* "dynamic" calcualtion of the symbols
		* new function for symbols only
		* number function does ONLY numbers now
		* number function can do negative numbers up to -9
	* enter start position (for virtual endstops)
	* move carriage with plus and minus buttons (for position counting)
* 2.0.1:
	* remember last settings (start at last entered number for slideTime, etc.)
	* better virtual endstop methods
	* some extensive serial prints can be disabled
	* functionality for pure interval triggering added
	* several bugfixes
* 2.1.0:
	* movie mode added
	
* TODO:
	* add a state machine documentation