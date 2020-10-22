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

#include "joystick.h"

//
Joystick::Joystick() {}

//
void Joystick::reset() {
    DPRINTLN("[ JOY] resetting");
    setMap(DEFAULT_MAP);
    state = JOYSTICK_ALL;
}

//
void Joystick::process(uint8_t data, TargetKbd *kbd) {

    data = data & JOYSTICK_ALL;
    uint8_t diff = data ^ state;

    if (diff == 0) {
        return;
    }

    DPRINTLN("[ JOY] port data: " + String(data));

    uint8_t mask = 1;

    for (uint8_t ix = 0; ix < JOYSTICK_ACTIONS; ix++) {
        if ((diff & mask) != 0) {
            kbd->handleKey(
                map[ix], (data & mask) == 0 ? PRESS_KEY : RELEASE_KEY);
        }
        mask <<= 1;
    }

    state = data;
}

//
void Joystick::setMap(uint8_t m[JOYSTICK_ACTIONS]) {
    for (uint8_t ix = 0; ix < JOYSTICK_ACTIONS; ix++) {
        DPRINTLN("[ JOY] mapping action " + String(ix) +
            " to key " + String(m[ix]));
        map[ix] = m[ix];
    }
}
