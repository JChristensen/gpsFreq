ReadMe file for Arduino GPS Frequency Counter Library v1.0
http://github.com/JChristensen/gpsFreq
Jack Christensen Jan 2013

This work is licensed under the Creative Commons Attribution-ShareAlike 3.0
Unported License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to Creative
Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

--------------------------------------------------------------------------------
This library uses the 1 PPS signal from a GPS receiver as an accurate time base.
Developed with Arduino 1.0.3, tested with an Arduino Uno.

To use the library:
(1) Go to http://github.com/JChristensen/gpsFreq and click the ZIP button to
    download the repository as a ZIP file to a convenient location on your PC.
(2) Uncompress the downloaded file. This will result in a folder containing all
    the files for the library, that has a name that includes the branch name,
    for example "gpsFreq-master".
(3) Rename the folder to just "gpsFreq".
(4) Copy the renamed folder to the Arduino sketchbook\libraries folder.

--------------------------------------------------------------------------------
Example sketches included with the library:

1. gpsFreqCounter -- Frequency counter sketch, output to Serial monitor
   and/or LCD.

2. t2_1MHz -- Outputs a 1MHz square wave. Loaded on another microcontroller, 
   can be used to measure its clock frequency using the gpsFreqCounter sketch.
