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

#ifndef SERIALKBD_h
#define SERIALKBD_h

#include <Arduino.h>

#include "config.h"
#include "joystick.h"
#include "keymap.h"
#include "targetkbd.h"

//
class SerialKbd {

private:
    KeyMap *map;
    uint8_t joystickMap[JOYSTICK_ACTIONS];
    int8_t joystickMapIx = -1;

public:
    SerialKbd();
    void reset();
    void process(uint8_t readBuf[2], TargetKbd *kbd, Joystick *joy);
};

#endif
