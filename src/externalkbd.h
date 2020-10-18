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

#ifndef EXTERNALKBD_h
#define EXTERNALKBD_h

#include <Arduino.h>

#include "_PS2KeyAdvanced.h"

#include "config.h"
#include "joystick.h"
#include "keymap.h"
#include "targetkbd.h"
#include "input_keycodes.h"

/*
    translation table for translating codes returned by PS2KeyAdvanced library
    to our input key codes. It's somewhat redundant, since the library already
    does a translation, but it does not seem trivial to modify its translation
    table without hurting functionality. But it's a small table and only a
    single array lookup, so not much of an overhead.
 */
static const uint8_t MAP_PS2_TO_INPUT[] = {
    KEY_RESERVED,
    KEY_NUMLOCK,    // PS2_KEY_NUM         0x01
    KEY_SCROLLLOCK, // PS2_KEY_SCROLL      0x02
    KEY_CAPSLOCK,   // PS2_KEY_CAPS        0x03
    KEY_PRINT,      // PS2_KEY_PRTSCR      0x04
    KEY_PAUSE,      // PS2_KEY_PAUSE       0x05
    KEY_LEFTSHIFT,  // PS2_KEY_L_SHIFT     0x06
    KEY_RIGHTSHIFT, // PS2_KEY_R_SHIFT     0x07
    KEY_LEFTCTRL,   // PS2_KEY_L_CTRL      0X08
    KEY_RIGHTCTRL,  // PS2_KEY_R_CTRL      0X09
    KEY_LEFTALT,    // PS2_KEY_L_ALT       0x0A
    KEY_RIGHTALT,   // PS2_KEY_R_ALT       0x0B
    KEY_LEFTMETA,   // PS2_KEY_L_GUI       0x0C
    KEY_RIGHTMETA,  // PS2_KEY_R_GUI       0x0D
    KEY_MENU,       // PS2_KEY_MENU        0x0E
    KEY_PAUSE,      // PS2_KEY_BREAK       0x0F
    KEY_SYSRQ,      // PS2_KEY_SYSRQ       0x10
    KEY_HOME,       // PS2_KEY_HOME        0x11
    KEY_END,        // PS2_KEY_END         0x12
    KEY_PAGEUP,     // PS2_KEY_PGUP        0x13
    KEY_PAGEDOWN,   // PS2_KEY_PGDN        0x14
    KEY_LEFT,       // PS2_KEY_L_ARROW     0x15
    KEY_RIGHT,      // PS2_KEY_R_ARROW     0x16
    KEY_UP,         // PS2_KEY_UP_ARROW    0x17
    KEY_DOWN,       // PS2_KEY_DN_ARROW    0x18
    KEY_INSERT,     // PS2_KEY_INSERT      0x19
    KEY_DELETE,     // PS2_KEY_DELETE      0x1A
    KEY_ESC,        // PS2_KEY_ESC         0x1B
    KEY_BACKSPACE,  // PS2_KEY_BS          0x1C
    KEY_TAB,        // PS2_KEY_TAB         0x1D
    KEY_ENTER,      // PS2_KEY_ENTER       0x1E
    KEY_SPACE,      // PS2_KEY_SPACE       0x1F
    KEY_KP0,        // PS2_KEY_KP0         0x20
    KEY_KP1,        // PS2_KEY_KP1         0x21
    KEY_KP2,        // PS2_KEY_KP2         0x22
    KEY_KP3,        // PS2_KEY_KP3         0x23
    KEY_KP4,        // PS2_KEY_KP4         0x24
    KEY_KP5,        // PS2_KEY_KP5         0x25
    KEY_KP6,        // PS2_KEY_KP6         0x26
    KEY_KP7,        // PS2_KEY_KP7         0x27
    KEY_KP8,        // PS2_KEY_KP8         0x28
    KEY_KP9,        // PS2_KEY_KP9         0x29
    KEY_KPDOT,      // PS2_KEY_KP_DOT      0x2A
    KEY_KPENTER,    // PS2_KEY_KP_ENTER    0x2B
    KEY_KPPLUS,     // PS2_KEY_KP_PLUS     0x2C
    KEY_KPMINUS,    // PS2_KEY_KP_MINUS    0x2D
    KEY_KPASTERISK, // PS2_KEY_KP_TIMES    0x2E
    KEY_KPSLASH,    // PS2_KEY_KP_DIV      0x2F
    KEY_0,          // PS2_KEY_0           0X30
    KEY_1,          // PS2_KEY_1           0X31
    KEY_2,          // PS2_KEY_2           0X32
    KEY_3,          // PS2_KEY_3           0X33
    KEY_4,          // PS2_KEY_4           0X34
    KEY_5,          // PS2_KEY_5           0X35
    KEY_6,          // PS2_KEY_6           0X36
    KEY_7,          // PS2_KEY_7           0X37
    KEY_8,          // PS2_KEY_8           0X38
    KEY_9,          // PS2_KEY_9           0X39
    KEY_APOSTROPHE, // PS2_KEY_APOS        0X3A
    KEY_COMMA,      // PS2_KEY_COMMA       0X3B
    KEY_MINUS,      // PS2_KEY_MINUS       0X3C
    KEY_DOT,        // PS2_KEY_DOT         0X3D
    KEY_SLASH,      // PS2_KEY_DIV         0X3E
    KEY_KPEQUAL,    // PS2_KEY_KP_EQUAL    0x3F
    KEY_APOSTROPHE, // PS2_KEY_SINGLE      0X40
    KEY_A,          // PS2_KEY_A           0X41
    KEY_B,          // PS2_KEY_B           0X42
    KEY_C,          // PS2_KEY_C           0X43
    KEY_D,          // PS2_KEY_D           0X44
    KEY_E,          // PS2_KEY_E           0X45
    KEY_F,          // PS2_KEY_F           0X46
    KEY_G,          // PS2_KEY_G           0X47
    KEY_H,          // PS2_KEY_H           0X48
    KEY_I,          // PS2_KEY_I           0X49
    KEY_J,          // PS2_KEY_J           0X4A
    KEY_K,          // PS2_KEY_K           0X4B
    KEY_L,          // PS2_KEY_L           0X4C
    KEY_M,          // PS2_KEY_M           0X4D
    KEY_N,          // PS2_KEY_N           0X4E
    KEY_O,          // PS2_KEY_O           0X4F
    KEY_P,          // PS2_KEY_P           0X50
    KEY_Q,          // PS2_KEY_Q           0X51
    KEY_R,          // PS2_KEY_R           0X52
    KEY_S,          // PS2_KEY_S           0X53
    KEY_T,          // PS2_KEY_T           0X54
    KEY_U,          // PS2_KEY_U           0X55
    KEY_V,          // PS2_KEY_V           0X56
    KEY_W,          // PS2_KEY_W           0X57
    KEY_X,          // PS2_KEY_X           0X58
    KEY_Y,          // PS2_KEY_Y           0X59
    KEY_Z,          // PS2_KEY_Z           0X5A
    KEY_SEMICOLON,  // PS2_KEY_SEMI        0X5B
    KEY_BACKSPACE,  // PS2_KEY_BACK        0X5C
    KEY_LEFTBRACE,  // PS2_KEY_OPEN_SQ     0X5D
    KEY_RIGHTBRACE, // PS2_KEY_CLOSE_SQ    0X5E
    KEY_EQUAL,      // PS2_KEY_EQUAL       0X5F
    KEY_KPCOMMA,    // PS2_KEY_KP_COMMA    0x60
    KEY_F1,         // PS2_KEY_F1          0X61
    KEY_F2,         // PS2_KEY_F2          0X62
    KEY_F3,         // PS2_KEY_F3          0X63
    KEY_F4,         // PS2_KEY_F4          0X64
    KEY_F5,         // PS2_KEY_F5          0X65
    KEY_F6,         // PS2_KEY_F6          0X66
    KEY_F7,         // PS2_KEY_F7          0X67
    KEY_F8,         // PS2_KEY_F8          0X68
    KEY_F9,         // PS2_KEY_F9          0X69
    KEY_F10,        // PS2_KEY_F10         0X6A
    KEY_F11,        // PS2_KEY_F11         0X6B
    KEY_F12         // PS2_KEY_F12         0X6C
};

//
class ExternalKbd {

private:
    PS2KeyAdvanced ps2;
    KeyMap map;

    void config();
    uint8_t toInputCode(uint8_t ps2Code);
    void setJoystickMap(Joystick *joy);

public:
    ExternalKbd(uint8_t dataPin, uint8_t irqPin);
    void reset();
    void process(TargetKbd *kbd, Joystick *joy);
};

#endif
