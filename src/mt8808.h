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

#ifndef MT8808_h
#define MT8808_h

#include <Arduino.h>

#include "config.h"

// masks
static const uint8_t MASK_RESET  = B00100000;
static const uint8_t MASK_DATA   = B01000000;
static const uint8_t MASK_STROBE = B10000000;
static const uint8_t MASK_AX     = B00000111;
static const uint8_t MASK_AY     = B00111000;

//
class MT8808 {

private:
    void setAddress(uint8_t a);
    void setData(bool on);
    void strobe();

public:
    MT8808();
    void reset();
    void setSwitch(uint8_t address, bool state);
};

#endif
