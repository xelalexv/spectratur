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

#include "externalkbd.h"

//
ExternalKbd::ExternalKbd(uint8_t dataPin, uint8_t irqPin) :
    map{MAP_INPUT_TO_TARGET, array_len(MAP_INPUT_TO_TARGET)} {
    ps2.begin(dataPin, irqPin);
}

//
void ExternalKbd::reset() {

    DPRINT("[PS/2] resetting ... ");

    unsigned long start = millis();
    ps2.resetKey();

    while ((millis() - start) < EXTERNAL_KBD_RESET_TIMEOUT) {
        if (ps2.available()) {
            switch (ps2.read()) {
                case 0xaa:
                    DPRINTLN("OK");
                    ps2.setLock(PS2_LOCK_NUM);
                    ps2.setNoRepeat(1);
                    return;
                case 0xfc:
                    DPRINTLN("NG");
                    return;
            }
        }
        delay(250);
    }

    DPRINTLN("not attached");
}

//
void ExternalKbd::process(TargetKbd *kbd, Joystick *joy) {

    if (!ps2.available()) {
        return;
    }

    uint16_t c = ps2.read();

    if (c == 0) {
        return;
    }

    uint8_t code = c & 0xff;

    // special cases
    // TODO make configurable
    if ((c & PS2_BREAK) != 0) {
        switch (code) {
            case 27: // reset
                reset();
                if (kbd != NULL) {
                    kbd->reset();
                }
                if (joy != NULL) {
                    joy->reset();
                }
                return;
            case 97: // joystick setup
                setJoystickMap(joy);
                return;
        }
    }

    code = toInputCode(code);
    uint8_t key = map.translate(code);
    KeyAction a;

    if ((c & PS2_BREAK) != 0) {
        a = RELEASE_KEY;
    } else {
        a = PRESS_KEY;
    }

    DPRINTLN("[PS/2] control: " + String(c >> 8) + ", action: " + String(a) +
        ", code: " + String(code) + ", key: " + String(key));

    kbd->handleKey(key, a);
}

//
uint8_t ExternalKbd::toInputCode(uint8_t ps2Code) {
    if (ps2Code < array_len(MAP_PS2_TO_INPUT)) {
        return MAP_PS2_TO_INPUT[ps2Code];
    }
    return KEY_RESERVED;
}

//
void ExternalKbd::setJoystickMap(Joystick *j) {

    if (j == NULL) {
        return;
    }

    DPRINTLN("[PS/2] setting joystick map");
    uint8_t m[JOYSTICK_ACTIONS];

    for (int ix = 0; ix < JOYSTICK_ACTIONS; ) {

        if (ps2.available()) {

            uint16_t c = ps2.read();

            if ((c & PS2_BREAK) != 0) {
                uint8_t code = toInputCode(c & 0xff);
                uint8_t key = map.translate(code);
                DPRINTLN("[PS/2] joystick setup " + String(key));
                m[ix] = key;
                ix++;
            }

        } else {
            delay(100);
        }
    }

    j->setMap(m);
}
