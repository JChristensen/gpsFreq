// Arduino gpsFreq Library
// https://github.com/JChristensen/gpsFreq
// Copyright (C) 2013-2022 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// GPS Frequency Counter Library for Arduino
// Uses 1 PPS from a GPS receiver as an accurate time base.
// Tested with Arduino 1.8.19 and an Arduino Uno.
// Connect the 1 PPS signal to INT0 (D2 pin).
// Connect the input frequency to be measured to T1 (D5 pin).

#ifndef GPSFREQ_H_INCLUDED
#define GPSFREQ_H_INCLUDED
#include <Arduino.h>

class FreqCounter
{
    public:
        FreqCounter() {};                   // constructor
        void start(uint8_t gatePeriod);     // gatePeriod in seconds (1, 10, 100)
        void formatFreq(char* c);           // returns freq as a formatted string
        volatile bool isBusy;               // flag to indicate counting complete
        volatile uint16_t ppsTotal;         // cumulative count of pps interrupts
        volatile uint16_t m_t1ovf;          // timer1 overflow count
        volatile uint8_t m_gateInterrupts;  // number of interrupts (caused by the 1PPS gate signal)
        uint8_t m_gatePeriod;               // gate period in seconds
        uint32_t freq;                      // the counted frequency
};

extern FreqCounter gpsFreq;

#endif
