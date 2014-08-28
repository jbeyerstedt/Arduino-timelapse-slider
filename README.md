# Arduino-timelapse-slider
by jtByt.Pictures, Jannik Beyerstedt from Hamburg, Germany  
[jannik.beyerstedt.de](http://jannik.beyerstedt.de) | [Github](https://github.com/jtByt-Pictures)  
**software documentation**  


## HEY - I´m writing a completely new version! I´ll be uploaded soon!
After studying and getting the state machine koan, I decided to completely rewrite the code of the timelapse-slider, because this is a state machine, so it should be programmed as state machine.  
I also will separate the software in smaller modules.  
See the results soon!


#### about this / über dieses Dokument  
**english:** 
This readme contains the introduction of the project, changelog and the **user manual** for the code/ software. Check also the "hardware_readme.md" and "electronics_readme.md".

**deutsch:** 
Dieses readme enthält eine Einführung zum Projekt, das changelog und das **Benutzerhandbuch** für den Code/ die Software. Schau also auch das "hardware_readme.md" und das "electronics_readme.md" für die vollständige Dokumentation an.


## introductions / Einleitung
**english:** 
I'm beginning shooting time lapses with my DSLR camera and wanted to have a slider for more interesting camera movements in my time lapses. So I began to write some code for the Arduino UNO board to control a stepper motor and the trigger of my camera.
To control this little thing I will use three buttons and two 7 digit displays.

You can find photos I made and in the future also videos on my website [jtByt-Pictures.de](http://jtByt-Pictures.de "website").

**deutsch:** 
Ich habe vor kurzer Zeit angefangen Zeitrafferaufnahmen mit meiner DSLR zu machen und wollte jetzt einen Kamera-Slider haben, da mit diesem noch spannendere Bewegungen möglich sind.
Also habe ich mich für Arduino als Plattform für mein Projekt entschieden und angefangen zu programmieren und an der Elektronik zu basteln.
Das Board und die Software sollten als erstes einen Schrittmotor und den Fernauslöser der Kamera ansteuern. Für die Eingaben habe ich mich für zwei 7-Segment-Anzeigen und drei Taster entschieden.

Fotos, die ich gemacht habe, sowie (hoffentlich) bald auch Videos findet Ihr auch auf meiner Website [jtByt-Pictures.de](http://jtByt-Pictures.de "website").

## user manual / Benutzerhandbuch  
**english:** 

1. set the time in minutes to slide along the slider
2. choose the slide direction
3. set the trigger interval in seconds
   
**deutsch:**  

1. Eingabe der Zeit in Minuten, die für eine Fahrt gebraucht werden soll  
2. Wahl der Richtung  
3. Eingabe des Auslöseintervalls in Sekunden  

## setup manual / Einrichtungshandbuch
follows soon / folgt bald


## what should the software do? / Was soll die Software können?
#### in general
* input values: time to slide the whole slider, slide direction, trigger interval
* have a stop button to escape the slide
* trigger the camera
* stop the slide at triggering the camera
* tree buttons as inputs (plus, minus, enter)
* drive two 8-bit shift registers for the displays
* drive a stepper motor (via stepper motor driver IC)
* turn off the stepper motor if it´s not needed (like at setting values or waiting for a new start)


#### additional in version 1.0
* movie mode with constant velocity
* (more or less) smart symbols on the display indicating the direction, current mode and errors

#### additional in version 1.1
* HOLD option having the stepper motor turned on all the time to hold the camera in place

#### additional in version 2.0 (far far away)
* movie mode (perhaps at entering trigger interval 0) with a constant or s-curve slide
* a servo motor for tilting the camera


## changelog
* 1.0.1:
	* setSlideTime=0 will get an division through 0 in the calculation which causes malfunction (because stepsPerInterval will be 0)
	* corrected the calculation of the stepsPerInterval at high speed / setSlideTime < 3 / calculated stepsPerSecond < than the highest possible speed
* 1.0.2:  
	* little modification at big slide times