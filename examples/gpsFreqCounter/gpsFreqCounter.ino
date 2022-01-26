/*----------------------------------------------------------------------*
 * GPS Frequency Counter Example Sketch for Arduino.                    *
 * Uses 1 PPS from a GPS receiver as an accurate time base.             *
 * Developed with Arduino 1.0.3, tested with an Arduino Uno.            *
 * Connect the 1 PPS signal to INT0 (D2 pin).                           *
 * Connect the input signal to be measured to T1 (D5 pin).              *
 * Connect a tactile button switch or similar from the D8 pin to        *
 * ground. This selects the sample period (1, 10, or 100 seconds).      *
 * This example sketch displays the frequency on the Serial monitor     *
 * as well as on a 16x2 LCD. Connecting the LCD is optional.            *
 * Connecting the button is optional if only a 1 second gate period     *
 * is needed.                                                           *
 *                                                                      *
 * Timer 2 can optionally be set to output a 1MHz square wave on the    *
 * MOSI pin (D11). This sketch can then measure its own system clock    *
 * frequency by connecting D11 to D5. This option is on by default;     *
 * to disable, set OUTPUT_1MHZ to zero in the #define statement.        *
 * To measure another MCU's clock, load the t2_1MHz sketch included     *
 * in the examples on the other MCU and connect its D11 pin to D5       *
 * on the MCU running this sketch.                                      *
 *                                                                      *
 * The highest frequency the counter can measure is limited to about    *
 * system clock frequency divided by 2.5, so about 6MHz with a          *
 * 16MHz clock, and about 3MHz with an 8MHz clock.                      *
 *                                                                      *
 * Jack Christensen 24Jan2013                                           *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/ 

#include <gpsFreq.h>        // https://github.com/JChristensen/gpsFreq
#include <JC_Button.h>      // https://github.com/JChristensen/Button
#include <LiquidCrystal.h>  // https://arduino.cc/en/Reference/LiquidCrystal (included with Arduino IDE)
#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming

//pin assignments
#define LCD_RS 10                  //16x2 LCD display (your pins may vary)
#define LCD_EN 11
#define LCD_D4 A3
#define LCD_D5 A2
#define LCD_D6 A1
#define LCD_D7 A0
#define GATE_BUTTON 4
#define GATE_LED LED_BUILTIN

#if F_CPU == 16000000             //calculate OCR2A value for Timer 2
#define ocr2a 7                   //16MHz, so divide by 8
#else
#define ocr2a 3                   //assume 8MHz, so divide by 4
#endif

unsigned long freq;
unsigned int nSample;
boolean gateLedState;
uint8_t gateTime = 1;
char strFreq[16];
Button btnGate(GATE_BUTTON);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup()
{
    pinMode(GATE_LED, OUTPUT);
    lcd.begin(16, 2);
    btnGate.begin();
    Serial.begin(115200);
    Serial << F( "\n" __FILE__ "\n" __DATE__ " " __TIME__ "\n" );

    #if OUTPUT_1MHZ == 1
    pinMode(MOSI, OUTPUT);                  //set up Timer 2 for 1MHz
    TCCR2B = 0;                             //stop timer 2
    TCCR2A = _BV(WGM21) | _BV(COM2A0);      //CTC mode, toggle OC2A on compare match
    OCR2A = ocr2a;                    
    TCCR2B = _BV(CS20);                     //start timer 2, no prescaling
    #endif

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd << F("Gate: ") << gateTime << F(" sec");
}

void loop()
{
    btnGate.read();
    if (btnGate.wasReleased()) {
        if (gateTime == 100)
            gateTime = 1;
        else
            gateTime *= 10;
        Serial << F("Gate time: ") << gateTime << F(" sec") << endl;
        lcd.setCursor(0, 1);
        lcd << F("Gate: ") << gateTime << F(" sec  ");
    }
    
    if (!gpsFreq.isBusy) {
        if (nSample > 0) {
            gpsFreq.formatFreq(strFreq);
            Serial << gpsFreq.ppsTotal << F(": ") << strFreq << F(" Hz") << endl;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd << strFreq << F(" Hz");
            lcd.setCursor(0, 1);
            lcd << F("Gate: ") << gateTime << F(" sec");
            digitalWrite(GATE_LED, gateLedState = !gateLedState);
        }
        ++nSample;
        gpsFreq.start(gateTime);
    }
}

