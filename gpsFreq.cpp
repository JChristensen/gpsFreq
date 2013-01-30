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

#include <gpsFreq.h>              //http://github.com/JChristensen/gpsFreq

//constructor
freqCounter::freqCounter()
{
}

//starts counting
void freqCounter::start(uint8_t gatePeriod)
{
    isBusy = true;
    _gatePeriod = gatePeriod;
    _gateInterrupts = 0;
    
    EICRA = _BV(ISC01);          //external interrupt on falling edge
    EIFR = _BV(INTF0);           //clear the interrupt flag (setting ISCnn can cause an interrupt)
    EIMSK = _BV(INT0);           //enable external interrupt
}

//return the frequency as a formatted string
void freqCounter::formatFreq(char *c)
{
    char f[16];
    char *pf;
    uint8_t len;
    
    ltoa(freq / _gatePeriod, f, 10);
    pf = f;
    len = strlen(f);
    for (uint8_t i=0; i<len; i++) {
        *c++ = *pf++;
        if ((len - i - 1) % 3 == 0 && i < len-1) *c++ = ',';
    }
    
    if (_gatePeriod > 1) {
        itoa(freq % _gatePeriod, f, 10);
        *c++ = '.';
        if (strlen(f) < 2 && _gatePeriod > 10) *c++ = '0';
        pf = f;
        while (*c++ = *pf++);
    }
    else {
        *c++ = 0;
    }    
}

freqCounter gpsFreq = freqCounter();    //instantiate the frequency counter object for the user

ISR(INT0_vect)
{
    if (gpsFreq._gateInterrupts == 0) {                //start counting with the first interrupt
        TCCR1B = 0;
        TCCR1A = 0;                                    //stop timer 1
        TCCR1C = 0;
        TIMSK1 = 0;
        TCNT1 = 0;                                     //zero timer 1
        TIFR1 = _BV(TOV1);                             //clear timer 1 overflow flag
        gpsFreq._t1ovf = 0;
        TIMSK1 = _BV(TOIE1);                           //interrupt on timer 1 overflow
        TCCR1B = _BV(CS12) | _BV(CS11);                //start timer 1, external clock on falling edge
    }
    else if (gpsFreq._gateInterrupts >= gpsFreq._gatePeriod) {   //time to stop counting?
        TCCR1B = 0;                                    //stop timer 1
        TIMSK1 = 0;                                    //stop timer 1 overflow interrupt
        EIMSK = 0;                                     //stop external interrupt
        gpsFreq.freq = ((unsigned long)gpsFreq._t1ovf << 16) + TCNT1;
        gpsFreq.isBusy = false;
    }
    ++gpsFreq._gateInterrupts;
	++gpsFreq.ppsTotal;
}

ISR(TIMER1_OVF_vect)
{
    ++gpsFreq._t1ovf;
}
