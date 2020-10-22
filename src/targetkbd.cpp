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

#include "targetkbd.h"

//
TargetKbd::TargetKbd() {}

//
void TargetKbd::reset() {
    clearKeyboardMatrix();
    mt88xx.reset();
}

//
void TargetKbd::clearKeyboardMatrix() {
    for (uint8_t ix = 0; ix < array_len(kbdMatrix); ix++) {
        kbdMatrix[ix] = 0;
    }
}

//
void TargetKbd::typeKey(uint8_t k) {
    pressKey(k);
    delay(100);
    releaseKey(k);
}

//
void TargetKbd::flipKey(uint8_t k) {
    handleKey(k, FLIP_KEY);
}

//
void TargetKbd::pressKey(uint8_t k) {
    handleKey(k, PRESS_KEY);
}

//
void TargetKbd::releaseKey(uint8_t k) {
    handleKey(k, RELEASE_KEY);
}

//
void TargetKbd::handleKey(uint8_t k, KeyAction a) {

    if (k == NA) {
        DPRINTLN("[TRGT] unassigned key");
        return;
    }

    if (isCombo(k)) {
        DPRINTLN("[TRGT] combo " + String(k));
        handleCombo(COMBOS[k & ~K_COMBO], a);
        return;
    }

    if (!isValidKeyAddress(k)) {
        DPRINTLN("[TRGT] invalid key address: " + String(k));
        return;
    }

    uint8_t ax = k & K_MASK_AX;
    uint8_t ay = (k & K_MASK_AY) >> 4; // shift out 4 AX bits

    bool data = false;

    switch (a) {
        case PRESS_KEY:
            data = true;
            break;
        case RELEASE_KEY:
            data = false;
            break;
        case FLIP_KEY:
            data = !getKeyState(ax, ay);
            DPRINTLN("[TRGT] flipping to: " + String(data));
            break;
    }

    DPRINTLN("[TRGT] key: " + String(k) + ", address: " + String(k) + ", ax: "
        + String(ax) + ", ay: " + String(ay) + ", data: " + String(data));

    mt88xx.setSwitch(k, data);

    // track key's on/off state
    setKeyState(ax, ay, data);

    //delay(100);
}

//
void TargetKbd::handleCombo(uint8_t combo[], KeyAction a) {
    if (a == PRESS_KEY) {
        for (int ix = 0; ix < array_len(combo); ix++) {
            handleKey(combo[ix], a);
        }
    } else if (a == RELEASE_KEY) {
        for (int ix = array_len(combo) - 1; ix >= 0 ; ix--) {
            handleKey(combo[ix], a);
        }
    }
}

//
bool TargetKbd::isCombo(uint8_t key) {
    return ((key & K_COMBO) == K_COMBO) && ((key & ~K_COMBO) < END_OF_COMBOS);
}

//
bool TargetKbd::isValidKeyAddress(uint8_t key) {
    return key < K_COMBO;
}


//
bool TargetKbd::isValidAxAy(uint8_t ax, uint8_t ay) {
    if (ax >= array_len(kbdMatrix)) {
        DPRINTLN("[TRGT] matrix AX out of bounds: " + String(ax));
        return false;
    }
    if (ay > 7) {
        DPRINTLN("[TRGT] matrix AY out of bounds: " + String(ay));
        return false;
    }
    return true;
}

//
void TargetKbd::setKeyState(uint8_t ax, uint8_t ay, bool on) {
    if (isValidAxAy(ax, ay)) {
        if (on) {
            kbdMatrix[ax] = kbdMatrix[ax] | (1 << ay);
        } else {
            kbdMatrix[ax] = kbdMatrix[ax] & (~(1 << ay));
        }
    }
}

//
bool TargetKbd::getKeyState(uint8_t ax, uint8_t ay) {
    if (isValidAxAy(ax, ay)) {
        return (kbdMatrix[ax] & (1 << ay)) != 0;
    }
    return false;
}
