/*
    Copyright 2020 Alexander Vollschwitz <xelalex@gmx.net>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <Arduino.h>

#include "config.h"
#include "externalkbd.h"
#include "serialkbd.h"
#include "joystick.h"
#include "targetkbd.h"


static const uint8_t PS2_DATAPIN = 4;
static const uint8_t PS2_IRQPIN  = 3;


// --- key sources ------------------------------------------------------------
ExternalKbd *externalKbd = NULL;
SerialKbd *serialKbd = NULL;
Joystick *joystick = NULL;

// --- key sink ---------------------------------------------------------------
TargetKbd *targetKbd = NULL;

// ------------------------------------------------------------------ SETUP ---

void setup() {

    /* port D (digital pins 0 to 7)
        bit 0: (serial port TX, don't use or touch)
            1: (serial port RX, don't use or touch)
            2: input pull-up (reserve; interrupt capable)
            3: PS/2 library: KBD clock; interrupt capable
            4: PS/2 library: KBD data
            5: output, MT88xx RESET
            6: output, MT88xx DATA
            7: output, MT88xx STROBE */
    DDRD  |= B11100000;
    PORTD &= B00011111;

    /* port B (digital pins 8 to 13)
        bit 0: output, MT88xx AX0
            1: output, MT88xx AX1
            2: output, MT88xx AX2
            3: output, MT88xx AY0
            4: output, MT88xx AY1
            5: output, MT88xx AY2 (also LED)
            6: input pull-up (not accessible)
            7: input pull-up (not accessible) */
    DDRB  = B00111111;
    PORTB = B11000000;

    /* port C (analog pins 0 to 5)
        bit 0: input pull-up, joystick UP
            1: input pull-up, joystick DOWN
            2: input pull-up, joystick LEFT
            3: input pull-up, joystick RIGHT
            4: input pull-up, joystick TRIGGER
            5: output, MT8812/16 AX3
            6: input pull-up (not accessible)
            7: input pull-up (not accessible) */
    DDRC  = B00100000;
    PORTC = B11011111;

    targetKbd = new TargetKbd();
    serialKbd = new SerialKbd();

    if (EXTERNAL_KBD) {
        externalKbd = new ExternalKbd(PS2_DATAPIN, PS2_IRQPIN);
    }

    if (JOYSTICK) {
        joystick = new Joystick();
    }

    Serial.begin(115200);
    reset();
}

// ------------------------------------------------------------------- LOOP ---

void loop() {

    if (Serial.available() > 1) {
        uint8_t buf[2] = {0, 0};
        Serial.readBytes(buf, 2);
        if (!handleSerial(buf) && (serialKbd != NULL)) {
            serialKbd->process(buf, targetKbd, joystick);
        }
    }

    if (externalKbd != NULL) {
        externalKbd->process(targetKbd, joystick);
    }

    if (joystick != NULL) {
        joystick->process(PINC, targetKbd);
    }
}

// ----------------------------------------------------------------------------

//
bool handleSerial(uint8_t buf[2]) {

    DPRINTLN("[MAIN] serial: {"
        + String(buf[0]) + ", " + String(buf[1]) + "}");

    switch ((char)buf[0]) {
        case '?':
            hello();
            break;
        case '!':
            reset();
            break;
        default:
            return false;
    }

    return true;
}

//
void hello() {
    Serial.println("spectratur");
}

//
void reset() {
    DPRINTLN("[MAIN] resetting");
    serialKbd->reset();
    targetKbd->reset();
    if (externalKbd != NULL) {
        externalKbd->reset();
    }
    if (joystick != NULL) {
        joystick->reset();
    }
    hello();
}
