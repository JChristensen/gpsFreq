// Arduino gpsFreq Library
// https://github.com/JChristensen/gpsFreq
// Copyright (C) 2018 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// GPS Frequency Counter Example Sketch.
// Uses 1 PPS from a GPS receiver as an accurate time base.
// Tested with Arduino 1.8.19 and an Arduino Uno.
// Connect the 1 PPS signal to INT0 (D2 pin).
// Connect the input signal to be measured to T1 (D5 pin).
// Connect a tactile button switch or similar from the D8 pin to
// ground. This selects the sample period (1, 10, or 100 seconds).
// This example sketch displays the frequency on the Serial monitor
// as well as on a 16x2 LCD. Connecting the LCD is optional.
// Connecting the button is optional if only a 1 second gate period
// is needed.
//
// The highest frequency the counter can measure is limited to about
// system clock frequency divided by 2.5, so about 6MHz with a
// 16MHz clock, and about 3MHz with an 8MHz clock.
//
// Jack Christensen 24Jan2013

#include <gpsFreq.h>        // https://github.com/JChristensen/gpsFreq
#include <JC_Button.h>      // https://github.com/JChristensen/Button
#include <LiquidCrystal.h>  // https://arduino.cc/en/Reference/LiquidCrystal
#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming

//pin assignments
constexpr uint8_t
    LCD_RS {10},            // 16x2 LCD display (your pins may vary)
    LCD_EN {11},
    LCD_D4 {A3},
    LCD_D5 {A2},
    LCD_D6 {A1},
    LCD_D7 {A0},
    gateButton {4},
    gateLED {LED_BUILTIN};

uint16_t nSample;
bool gateLedState;
uint8_t gateTime {1};
char strFreq[16];
Button btnGate(gateButton);
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup()
{
    pinMode(gateLED, OUTPUT);
    lcd.begin(16, 2);
    btnGate.begin();
    Serial.begin(115200);
    Serial << F( "\n" __FILE__ "\n" __DATE__ " " __TIME__ "\n" );

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
            digitalWrite(gateLED, gateLedState = !gateLedState);
        }
        ++nSample;
        gpsFreq.start(gateTime);
    }
}
