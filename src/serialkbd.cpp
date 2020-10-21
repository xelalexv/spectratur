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
void SerialKbd::process(uint8_t readBuf[2], TargetKbd *kbd) {

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

    kbd->handleKey(key, a);
}
