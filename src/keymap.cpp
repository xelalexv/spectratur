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

#include "keymap.h"

//
KeyMap::KeyMap(uint8_t *m, uint8_t l) {
    map = m;
    length = l;
}

//
bool KeyMap::isValidIndex(uint8_t ix) {
    return ix < length;
}

//
bool KeyMap::isAssigned(uint8_t code) {
    return translate(code) != NA;
}

//
uint8_t KeyMap::translate(uint8_t code) {
    if (isValidIndex(code)) {
        return map[code];
    }
    return NA;
}
