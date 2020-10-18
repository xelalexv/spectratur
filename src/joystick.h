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

#ifndef JOYSTICK_h
#define JOYSTICK_h

#include <Arduino.h>

#include "config.h"
#include "targetkbd.h"

// masks
static const uint8_t JOYSTICK_UP       = B00000001;
static const uint8_t JOYSTICK_DOWN     = B00000010;
static const uint8_t JOYSTICK_LEFT     = B00000100;
static const uint8_t JOYSTICK_RIGHT    = B00001000;
static const uint8_t JOYSTICK_TRIGGER  = B00010000;
static const uint8_t JOYSTICK_ALL      = B00011111;

static const uint8_t JOYSTICK_ACTIONS = 5;

static const uint8_t DEFAULT_MAP[JOYSTICK_ACTIONS] = {K_Q, K_A, K_N, K_M, K_Z};

//
class Joystick {

private:
    uint8_t map[JOYSTICK_ACTIONS];
    uint8_t state;

public:
    Joystick();
    void reset();
    void setMap(uint8_t m[JOYSTICK_ACTIONS]);
    void process(uint8_t port, TargetKbd *kbd);
};

#endif
