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

#ifndef TARGETKBD_h
#define TARGETKBD_h

#include <Arduino.h>

#include "config.h"
#include "mt8808.h"

// masks
static const uint8_t MASK_ADDRESS_COL = MASK_AX;
static const uint8_t MASK_ADDRESS_ROW = MASK_AY;

//
class TargetKbd {

private:
    MT8808 mt8808;
    // This bit matrix represents the current state of the target keyboard.
    // A key is pressed when its corresponding bit is 0.
    uint8_t kbdMatrix[8];

    void clearKeyboardMatrix();
    bool isCombo(uint8_t key);
    bool isValidKeyAddress(uint8_t key);
    bool isValidRowCol(uint8_t row, uint8_t col);
    void setKeyState(uint8_t row, uint8_t col, bool on);
    bool getKeyState(uint8_t row, uint8_t col);
    void handleCombo(uint8_t combo[], KeyAction a);

public:
    TargetKbd();
    void reset();
    void typeKey(uint8_t key);
    void flipKey(uint8_t key);
    void pressKey(uint8_t key);
    void releaseKey(uint8_t key);
    void handleKey(uint8_t k, KeyAction a);
};

#endif
