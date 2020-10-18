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

#ifndef SPECTRATUR_h
#define SPECTRATUR_h

// Set whether to activate debug mode. In debug mode, diagnostic messages are
// written to the serial port.
//
#define DEBUG false


// Set whether to use an external keyboard (PS/2 or PS/2 capable USB keyboard).
//
#define EXTERNAL_KBD true


// The timeout in milliseconds for resetting the external keyboard. If there's
// no reply from the keyboard before the timeout, the keyboard is considered not
// being attached
//
#define EXTERNAL_KBD_RESET_TIMEOUT 3000


// Set whether to use a joystick port.
//
#define JOYSTICK true


// macro for key combinations (combos)
//
#define COMBO( c ) K_COMBO | c

static const uint8_t NA = 0xff; // shorthand for "not assigned"
static const uint8_t K_COMBO = B01000000; // base for a key combination


// Include the header file with all the necessary definitions for your target
// system here.
//
#include "target_spectrum.h"
//#include "target_zx80.h"


// --- debug helpers ---------------------------------------------------------

#if DEBUG == true

#include <Arduino.h>

#define DPRINT(...)    Serial.print(__VA_ARGS__)
#define DPRINTLN(...)  Serial.println(__VA_ARGS__)

#else

#define DPRINT(...)
#define DPRINTLN(...)

#endif


// --- other helpers ---------------------------------------------------------

#define array_len( x )  ( sizeof( x ) / sizeof( *x ) )


// ---------------------------------------------------------------------------

// key action - maintained here since enums can't reside in main file
enum KeyAction {
    RELEASE_KEY,
    PRESS_KEY,
    FLIP_KEY
};

#endif
