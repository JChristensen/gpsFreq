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

#include <gpsFreq.h>        // https://github.com/JChristensen/gpsFreq

// starts counting
void FreqCounter::start(uint8_t gatePeriod)
{
    isBusy = true;
    m_gatePeriod = gatePeriod;
    m_gateInterrupts = 0;

    TIMSK0 &= ~_BV(TOIE0);  // disable timer 0 overflow interrupt -- disables millis(), delay()
    EICRA = _BV(ISC01);     // external interrupt on falling edge
    EIFR = _BV(INTF0);      // clear the interrupt flag (setting ISCnn can cause an interrupt)
    EIMSK = _BV(INT0);      // enable external interrupt
}

// return the frequency as a formatted string
void FreqCounter::formatFreq(char* c)
{
    char f[16];

    ltoa(freq / m_gatePeriod, f, 10);
    char* pf = f;
    uint8_t len = strlen(f);
    for (uint8_t i=0; i<len; i++) {
        *c++ = *pf++;
        if ((len - i - 1) % 3 == 0 && i < len-1) *c++ = ',';
    }

    if (m_gatePeriod > 1) {
        itoa(freq % m_gatePeriod, f, 10);
        *c++ = '.';
        if (strlen(f) < 2 && m_gatePeriod > 10) *c++ = '0';
        pf = f;
        while ( (*c++ = *pf++) );
    }
    else {
        *c++ = 0;
    }
}

FreqCounter gpsFreq;    // instantiate the frequency counter object for the user

ISR(INT0_vect)
{
    if (gpsFreq.m_gateInterrupts == 0) {    // start counting with the first interrupt
        TCCR1B = 0;
        TCCR1A = 0;                     // stop timer 1
        TCCR1C = 0;
        TIMSK1 = 0;
        TCNT1 = 0;                      // zero timer 1
        TIFR1 = _BV(TOV1);              // clear timer 1 overflow flag
        gpsFreq.m_t1ovf = 0;
        TIMSK1 = _BV(TOIE1);            // interrupt on timer 1 overflow
        TCCR1B = _BV(CS12) | _BV(CS11); // start timer 1, external clock on falling edge
    }
    else if (gpsFreq.m_gateInterrupts >= gpsFreq.m_gatePeriod) {    // time to stop counting?
        TCCR1B = 0;                     // stop timer 1
        TIMSK1 = 0;                     // stop timer 1 overflow interrupt
        EIMSK = 0;                      // stop external interrupt
        TIMSK0 |= _BV(TOIE0);           // enable timer 0 overflow interrupt
        gpsFreq.freq = ((uint32_t)gpsFreq.m_t1ovf << 16) + TCNT1;
        gpsFreq.isBusy = false;
    }
    ++gpsFreq.m_gateInterrupts;
    ++gpsFreq.ppsTotal;
}

ISR(TIMER1_OVF_vect)
{
    ++gpsFreq.m_t1ovf;
}
