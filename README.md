# arduino timelapse camera slider
This is the readme for the project and code itself, for a user manual check user_readme.md, the electronically setup is under electronics.md  
Dieses readme beschäftigt sich mit dem Projekt und Code an sich, eine Bedienungsanleitung findet sich unter user_readme.md, die Elektronik und alles andere technische drum herum ist in electronics.md zusammengefasst.

## introduction / Einleitung
**english**  
I'm beginning shooting time lapses with my DSLR camera and wanted to have a slider for more interesting camera movements in my time lapses. So I began to write some code for the arduino UNO board to control a stepper motor and the trigger of my camera.
To control this little thing I will use three buttons and two 7 digit displays.

You can find photos I made and in the future also videos on my website [jtByt-Pictures.de](http://jtByt-Pictures.de "my website").

**deutsch**  
Ich habe vor kurzer Zeit angefangen Zeitrafferaufnahmen mit meiner DSLR zu machen und wollte jetzt einen Kamera slider haben, da mit diesem noch spannendere Bewegungen möglich sind.
Also habe ich mich für arduino als Plattform für mein Projekt entschieden und angefangen zu programmieren und an der Elektronik zu basteln.
Das Board und die Software sollten als erstes einen Schrittmotor und den Fernauslöser der Kamera ansteuern. Für die Eingaben habe ich mich für zwei 7-Segment-Anzeigen und drei Taster entschieden.

Fotos, die ich gemacht habe, sowie (hoffentlich) bald auch Videos findet Ihr auch auf meiner Website [jtByt-Pictures.de](http://jtByt-Pictures.de "my website").

## user manual / Benutzerhandbuch  
**english**  

1. set the time in minutes to slide along the slider
2. choose the slide direction
3. set the trigger interval in seconds
   
**deutsch**  

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
* turn off the stepper motor if it´s not needed (like at setting values or wating for a new start)


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
	* setSlideTime=0 will get an division throgh 0 in the caclulation which causes malfunction (because stepsPerInterval will be 0)
	* corrected the calculation of the stepsPerInterval at high speed / setSlideTime < 3 / calculated stepsPerSecond < than the highest possible speed
* 1.0.2:  
	* little modification at big slide times