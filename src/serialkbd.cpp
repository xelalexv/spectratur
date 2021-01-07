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

#include "serialkbd.h"

//
SerialKbd::SerialKbd() {
    map = new KeyMap(MAP_INPUT_TO_TARGET, array_len(MAP_INPUT_TO_TARGET));
}

//
void SerialKbd::reset() {
    DPRINTLN("[ SER] resetting");
    joystickMapIx = -1;
}

//
void SerialKbd::process(uint8_t readBuf[2], TargetKbd *kbd, Joystick *joy) {

    uint8_t makeBreak = readBuf[0];
    uint8_t code = readBuf[1];
    KeyAction a;

    switch (makeBreak) {
        case 0: // break
            a = RELEASE_KEY;
            break;
        case 1: // make
            a = PRESS_KEY;
            break;
        default:
            DPRINTLN("[ SER] illegal make/break code: " + String(makeBreak));
            return;
    }

    uint8_t key = map->translate(code);
    DPRINTLN("[ SER] action: " + String(a) + ", code: " + String(code) +
        ", key: " + String(key));

    if (code == 59) { // start joystick map setup (F1); TODO: make configurable
        if (a == RELEASE_KEY && joy != NULL) {
            DPRINTLN("[ SER] setting joystick");
            joystickMapIx = 0;
        }

    } else if (joystickMapIx < 0) { // regular key handling
        kbd->handleKey(key, a);

    } else if (a == RELEASE_KEY) { // collecting joystick map
        DPRINTLN("[ SER] joystick setup " + String(key));
        joystickMap[joystickMapIx++] = key;
        if (joystickMapIx == array_len(joystickMap)) {
            joystickMapIx = -1;
            joy->setMap(joystickMap);
        }
    }
}
