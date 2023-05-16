# COMP522-ArduinoRCCar
Semester project for COMP 522

What project did you end up implementing? (If nothing has changed since your original proposal, you can simply re-state/copy it here.)

RC Car using Arduino

I planned to power an RC Car with the Arduino UNO Board we've been using in previous projects. I planned to buy an existing RC car and connect the parts (motors, wires, etc.) to an Arduino UNO board which would be the microprocessor for the entire system. The Arduino UNO Board will take in input wirelessly from a controller (in my implementation it became a bluetooth connection with a phone application).

How much of your project is working? (How close are you to finishing the project?)

The project is done, but improvements can still be made.

Improvements that can be made:
Moving to a bigger frame
Better/Stronger DC motors (possibly servo motor) to increase speed
Better steering system to increase maneuverability
Smaller portable battery to decrease weight

Indicate all steps you took to implement this project and debug it (Specifically, what was
your debugging strategy, and where did you get stuck? How much time did you spend on each bug, and what resources did you use?)

Steps:
Evaluated RC Car (took everything apart to see how and what made it run) (3 hours)
Gathered the materials/tools for the project (passively took 3 days for the parts to come in)
Assembled the parts related to the car’s movement (6 hours)
I cut out the original RC Components (PCB, Original Wires connecting to the PCB, etc.)
I cut and stripped the wires going to both DC motors
Connected the DC motor to the L298n H Bridge and connected the bridge to the Arduino
Connected the HC-06 Bluetooth Module to the Arduino
I cut a USB cable and stripped the wires
I then connected the USB cable to the portable battery and the L298n bridge’s 12v pin
Took the code from different tutorials and tested the RC car (30 mins)
Added Lights and Buzzer (10 mins)
Tested and debugged
Added Music player and music
Tested and debugged

Key issues I ran into:
Implementing FreeRTOS into project
Had trouble multitasking (e.g. playing music and RC car at the same time)
Using 2 serial input/output in a program
Hardware
Had trouble finding a place to power all items
Some of the connections were unreliable

I debugged by reading/searching up solutions. In addition, I tested the parts individually and together.

I took approximately 80 hours debugging which includes research. I can’t really say how long it took for each bug, but I think the most troubling/time-consuming problem had to do with implementing FreeRTOS.

Here are some of the main sources I found useful information from:
https://circuitjournal.com/how-to-use-the-dfplayer-mini-mp3-module-with-an-arduino
https://www.youtube.com/watch?v=VStxCmDqqeg
https://forum.arduino.cc/t/multiple-serial-communication-on-arduino-uno/245102/4
https://www.hackster.io/muhammad-aqib/high-speed-arduino-rc-car-5c2a3d
https://www.instructables.com/Arduino-Bluetooth-Controlled-RC-Car/


What, if any, extra hardware did you purchase?

New Hardware/Materials: 
RC Car base (Came with DC motors, frame, etc.)
L298n H Bridge
HC-06 Bluetooth module
HiLetgo DFPlayer Mini MP3 Module
Micro SD Card
Female to Female Dupont jumper wires
Portable Power bank
USB Cable
Speaker 
1 in 4 out Terminal Blocks (I tried to not solder wires, but eventually I did for the speaker)
LEDs with Dupont connectors

Tools:
Precision/Hobby Screwdrivers
Soldering Iron (optional)
Scissors
Pliers
Wire stripper (optional)
Zip Ties
Tape

How many person-hours did you spend (or did your team spend) implementing the project?

Started the project around Mar 18, 2023
Finished the project around May 14, 2023
I Worked on the project during Weekends and Fridays. Approximately, 100 hours more or less. This includes everything from researching to assembling the project.

Which code, if any, did you make use of that you did not write? Where did you find this code? 

I tried to reuse some code from the tutorials I found, but I had to ultimately modify it to make it compatible with FreeRTOS, the 3rd party application I used, and the tasks I wanted it to perform. Therefore, the main parts of the code that have been reused are library functions like DF player’s and Arduino’s in-built analogReads. 

Here are some of the main sources I found useful information/code from:
https://circuitjournal.com/how-to-use-the-dfplayer-mini-mp3-module-with-an-arduino
https://www.youtube.com/watch?v=VStxCmDqqeg
https://forum.arduino.cc/t/multiple-serial-communication-on-arduino-uno/245102/4
https://www.hackster.io/muhammad-aqib/high-speed-arduino-rc-car-5c2a3d
https://www.instructables.com/Arduino-Bluetooth-Controlled-RC-Car/
