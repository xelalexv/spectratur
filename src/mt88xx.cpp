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

#include "mt88xx.h"

// TODO: pass port references?
MT88xx::MT88xx() {}

//
void MT88xx::reset() {
    DPRINTLN("[88xx] resetting");
    PORTD |= MASK_RESET;
    delayMicroseconds(3);
    PORTD &= ~MASK_RESET;
}

//
void MT88xx::setSwitch(uint8_t address, bool state) {
    setAddress(address);
    setData(state);
    strobe();
}

//
void MT88xx::strobe() {
    PORTD |= MASK_STROBE; // strobe HIGH
    // the data sheet specifies a minimum of 20ns, the minimal reliable value
    // for delayMicroseconds is 3, which is OK for our application
    delayMicroseconds(3);
    PORTD &= ~MASK_STROBE; // strobe LOW
}

//
void MT88xx::setAddress(uint8_t a) {
    // set address bits AX0-2 and AY0-2 in PORTB, but squeeze out AX3 and
    // don't touch upper two bits
    PORTB |= (MASK_AX | MASK_AY);
    PORTB &= (~(MASK_AX | MASK_AY) | (a & MASK_AX) | ((a >> 1) & MASK_AY));
    // set AX3, for MT8812/16
    PORTC |= MASK_AX3;
    PORTC &= (((a << 2) & MASK_AX3) | ~MASK_AX3);
}

//
void MT88xx::setData(bool on) {
    if (on) {
        PORTD |= MASK_DATA;
    } else {
        PORTD &= ~MASK_DATA;
    }
}
