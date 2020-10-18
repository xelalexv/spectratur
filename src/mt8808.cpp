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

#include "mt8808.h"

// TODO: pass port reference?
MT8808::MT8808() {}

//
void MT8808::reset() {
    DPRINTLN("[8808] resetting");
    PORTD |= MASK_RESET;
    delayMicroseconds(3);
    PORTD &= ~MASK_RESET;
}

//
void MT8808::setSwitch(uint8_t address, bool state) {
    setAddress(address);
    setData(state);
    strobe();
}

//
void MT8808::strobe() {
    PORTD |= MASK_STROBE; // strobe HIGH
    // the data sheet specifies a minimum of 20ns, the minimal reliable value
    // for delayMicroseconds is 3, which is OK for our application
    delayMicroseconds(3);
    PORTD &= ~MASK_STROBE; // strobe LOW
}

//
void MT8808::setAddress(uint8_t a) {
    // set address, but make sure not to touch upper two bits
    PORTB |= (MASK_AX | MASK_AY);
    PORTB &= (a | (~(MASK_AX | MASK_AY)));
}

//
void MT8808::setData(bool on) {
    if (on) {
        PORTD |= MASK_DATA;
    } else {
        PORTD &= ~MASK_DATA;
    }
}
