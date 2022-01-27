# Arduino GPS Frequency Counter Library
http://github.com/JChristensen/gpsFreq  
README file  
Jack Christensen  
Jan 2013

## License
Arduino gpsFreq Library Copyright (C) Jack Christensen GNU GPL v3.0

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License v3.0 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/gpl.html>

## Introduction
This library uses the 1 PPS signal from a GPS receiver as an accurate time base.  
Tested with Arduino 1.8.19 and an Arduino Uno.

## Example sketches
The following example sketches are included with the library:

- **gpsFreqCounter:** Frequency counter sketch, output to Serial monitor and/or LCD.
- **t2_1MHz:** Outputs a 1MHz square wave. Loaded on another microcontroller, can be used to measure its clock frequency using the gpsFreqCounter sketch.
