// Arduino gpsFreq Library
// https://github.com/JChristensen/gpsFreq
// Copyright (C) 2018 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Example sketch: One megahertz frequency reference.
// Outputs a 1MHz square wave on the MOSI (D11) pin.
// Tested with Arduino 1.8.19 and an Arduino Uno.
// Use this sketch in conjunction with the gpsFreqCounter
// sketch to measure the accuracy of the system clock.
//
// This sketch will work on MCUs with 16MHz or 8MHz system clock.
//
// Timer 2 is configured to output a 1MHz square wave on the
// MOSI pin (D11 on Arduino Uno). Since the frequency counter is
// limited to ~6MHz (with 16MHz system clock), we divide the system
// clock down to 1MHz, which makes it easy to estimate PPM accuracy
// directly from the frequency measurement.
//
// Jack Christensen 24Jan2013

constexpr bool blink {false};               // set to true to blink an LED
constexpr uint8_t blinkLED {LED_BUILTIN};   // the pin number of the LED to blink
constexpr uint32_t blinkInterval {1000};    // blink interval, milliseconds

void setup()
{
    if (blink) pinMode(blinkLED, OUTPUT);
    pinMode(MOSI, OUTPUT);                  // D11, MOSI, PB3, DIP 17

    // Set timer 2 to output a 1MHz square wave on MOSI (D11 on Arduino Uno)
    TCCR2B = 0;                             // stop timer 2
    TCCR2A = _BV(WGM21) | _BV(COM2A0);      // CTC mode, toggle OC2A on compare match
    // calculate OCR2A value for Timer 2. For 16MHz, divide by 8,
    // else assume 8MHz and divide by 4.
    constexpr uint32_t cpuFreq {F_CPU};
    OCR2A = (cpuFreq == 16000000) ? 7 : 3;
    TCCR2B = _BV(CS20);                     // start timer 2, no prescaling
}

void loop()
{
    if (blink) {
        static uint32_t ms, msLast;
        static bool ledState;

        ms = millis();
        if (ms - msLast >= blinkInterval) {
            msLast = ms;
            digitalWrite(blinkLED, ledState = !ledState);
        }
    }
}
