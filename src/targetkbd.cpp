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
    mt8808.reset();
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

    if (isCombo(k)) {
        DPRINTLN("[TRGT] combo " + String(k));
        handleCombo(COMBOS[k & B00111111], a);
        return;
    }

    if (!isValidKeyAddress(k)) {
        DPRINTLN("[TRGT] invalid key address: " + String(k));
        return;
    }

    uint8_t col = k & MASK_ADDRESS_COL;
    uint8_t row = k >> 3;

    if (row >= 5) {
        DPRINTLN("[TRGT] row out of bounds: " + String(row));
        return;
    }

    bool data = false;

    switch (a) {
        case PRESS_KEY:
            data = true;
            break;
        case RELEASE_KEY:
            data = false;
            break;
        case FLIP_KEY:
            data = !getKeyState(row, col);
            DPRINTLN("[TRGT] flipping to: " + String(data));
            break;
    }

    DPRINTLN("[TRGT] address: " + String(k) + ", col: " + String(col) +
        ", row: " + String(row) + ", data: " + String(data));

    mt8808.setSwitch(k, data);

    // track key's on/off state
    setKeyState(row, col, data);

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
    return ((key & B11000000) == K_COMBO) && ((key & B00111111) < END_OF_COMBOS);
}

//
bool TargetKbd::isValidKeyAddress(uint8_t key) {
    return 0 <= key && key < B01000000;
}


//
bool TargetKbd::isValidRowCol(uint8_t row, uint8_t col) {
    if (row >= array_len(kbdMatrix)) {
        DPRINTLN("[TRGT] matrix row out of bounds: " + String(row));
        return false;
    }
    if (col >= 8) {
        DPRINTLN("[TRGT] matrix column out of bounds: " + String(col));
        return false;
    }
    return true;
}

//
void TargetKbd::setKeyState(uint8_t row, uint8_t col, bool on) {
    if (isValidRowCol(row, col)) {
        if (on) {
            kbdMatrix[row] = kbdMatrix[row] | (1 << col);
        } else {
            kbdMatrix[row] = kbdMatrix[row] & (~(1 << col));
        }
    }
}

//
bool TargetKbd::getKeyState(uint8_t row, uint8_t col) {
    if (isValidRowCol(row, col)) {
        return (kbdMatrix[row] & (1 << col)) != 0;
    }
    return false;
}
