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

#ifndef KEYMAPS_h
#define KEYMAPS_h

#include <Arduino.h>

#include "config.h"

//
class KeyMap {

private:
    uint8_t *map;
    uint8_t length;

    bool isValidIndex(uint8_t ix);

public:
    KeyMap(uint8_t *map, uint8_t length);
    bool isAssigned(uint8_t code);
    uint8_t translate(uint8_t code);
};

#endif
