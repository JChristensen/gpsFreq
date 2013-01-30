/*----------------------------------------------------------------------*
 * One megahertz frequency reference for Arduino.                       *
 * Outputs a 1MHz square wave on the MOSI (D11) pin.                    *
 * Developed with Arduino 1.0.3, tested with an Arduino Uno.            *
 * Use this sketch in conjunction with the gpsFreqCounter sketch to     *
 * measure the accuracy of the system clock.                            *
 *                                                                      *
 * This sketch will work on MCUs with 16MHz or 8MHz system clock.       *
 *                                                                      *
 * This sketch configures Timer 2 to output a 1MHz square wave on the   *
 * MOSI pin (D11 on Arduino Uno). Since the frequency counter is        *
 * limited to ~6MHz (with 16MHz system clock), we divide the system     *
 * clock down to 1MHz, which makes it easy to estimate PPM accuracy     *
 * directly from the frequency measurment.                              *
 *                                                                      *
 * Jack Christensen 24Jan2013                                           *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/ 

#define BLINK_THAT_LED 0          //optionally set to 1 to blink the built-in LED

#define LED LED_BUILTIN           //LED pin number
#define LED_INTERVAL 1000

#if F_CPU == 16000000             //calculate OCR2A value for Timer 2
#define ocr2a 7                   //16MHz, so divide by 8
#else
#define ocr2a 3                   //assume 8MHz, so divide by 4
#endif

void setup(void)
{
    #if BLINK_THAT_LED == 1
    pinMode(LED, OUTPUT);
    #endif
    pinMode(11, OUTPUT);                    //D11, MOSI, PB3, DIP 17
    //Set timer 2 to output a 1MHz square wave on MOSI (D11 on Arduino Uno)
    TCCR2B = 0;                             //stop timer 2
    TCCR2A = _BV(WGM21) | _BV(COM2A0);      //CTC mode, toggle OC2A on compare match
    OCR2A = ocr2a;                    
    TCCR2B = _BV(CS20);                     //start timer 2, no prescaling    
}

void loop(void)
{
    #if BLINK_THAT_LED == 1
    //nothing else to do here, so we might as well blink an LED ;-)
    static unsigned long ms, msLast;
    static boolean ledState;

    ms = millis();
    if (ms - msLast >= LED_INTERVAL) {
        msLast = ms;
        digitalWrite(LED, ledState = !ledState);
    }
    #endif
}

