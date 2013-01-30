/*----------------------------------------------------------------------*
 * GPS Frequency Counter Library for Arduino                            *
 * Uses 1 PPS from a GPS receiver as an accurate time base.             *
 * Developed with Arduino 1.0.3, tested with an Arduino Uno.            *
 * Connect the 1 PPS signal to INT0 (D2 pin).                           *
 * Connect the input frequency to be measured to T1 (D5 pin).           *
 *                                                                      *
 * Jack Christensen 24Jan2013                                           *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/ 

#ifndef GPS_FREQ_H
#define GPS_FREQ_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

class freqCounter
{
    public:
        freqCounter();                      //constructor
        void start(uint8_t gatePeriod);     //gatePeriod in seconds (1, 10, 100)
        void formatFreq(char *c);           //returns freq as a formatted string
        volatile boolean isBusy;            //flag to indicate counting complete
		volatile unsigned int ppsTotal;		//cumulative count of pps interrupts
        volatile uint16_t _t1ovf;           //timer1 overflow count
        volatile uint8_t _gateInterrupts;   //number of interrupts (caused by the 1PPS gate signal)
        uint8_t _gatePeriod;                //gate period in seconds
        unsigned long freq;                 //the counted frequency
};

extern freqCounter gpsFreq;

#endif
