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

#ifndef TARGET_ZX80_h
#define TARGET_ZX80_h

#include "targets/sinclair_zx8x_base.h"

// --- specials ---------------------------------------------------------------
enum SPECIALS {
    COMBO_LEFT = 0,
    COMBO_DOWN,
    COMBO_UP,
    COMBO_RIGHT,
    COMBO_HOME,
    COMBO_RUBOUT,
    COMBO_DOUBLE_QUOTE,
    COMBO_DOLLAR,
    COMBO_OPEN_PAREN,
    COMBO_CLOSE_PAREN,
    COMBO_ASTERISK,
    COMBO_EXP,
    COMBO_MINUS,
    COMBO_PLUS,
    COMBO_EQUAL,
    COMBO_EDIT,
    COMBO_CAPS_LOCK,
    COMBO_COLON,
    COMBO_SEMICOLON,
    COMBO_QUESTION,
    COMBO_SLASH,
    COMBO_LOWER,
    COMBO_GREATER,
    COMBO_COMMA,
    COMBO_POUND,
    END_OF_COMBOS,      // combo/macro divider
    MACRO_LOAD,
    END_OF_SPECIALS
};

// combo definitions
static const uint8_t combo_home[]         = {K_SHIFT, K_9, NA};
static const uint8_t combo_double_quote[] = {K_SHIFT, K_Y, NA};
static const uint8_t combo_asterisk[]     = {K_SHIFT, K_P, NA};
static const uint8_t combo_edit[]         = {K_SHIFT, K_NEWLINE, NA};

// macro definitions
static const uint8_t macro_load[] = { // LOAD ""
    K_W, SK(COMBO_DOUBLE_QUOTE), SK(COMBO_DOUBLE_QUOTE), NA
};

// specials table
static const uint8_t* SPECIALS[END_OF_SPECIALS] = {
    combo_left,
    combo_down,
    combo_up,
    combo_right,
    combo_home,
    combo_rubout,
    combo_double_quote,
    combo_dollar,
    combo_open_paren,
    combo_close_paren,
    combo_asterisk,
    combo_exp,
    combo_minus,
    combo_plus,
    combo_equal,
    combo_edit,
    combo_caps_lock,
    combo_colon,
    combo_semicolon,
    combo_question,
    combo_slash,
    combo_lower,
    combo_greater,
    combo_comma,
    combo_pound,
    0, // combo/macro divider
    macro_load
};

/* --- key map ----------------------------------------------------------------

    map for translating input key codes (see input_keycodes.h) to target key
    addresses
 */
static const uint8_t MAP_INPUT_TO_TARGET[] = {
    NA,                 // KEY_RESERVED
    NA,                 // KEY_ESC
    K_1,                // KEY_1
    K_2,                // KEY_2
    K_3,                // KEY_3
    K_4,                // KEY_4
    K_5,                // KEY_5
    K_6,                // KEY_6
    K_7,                // KEY_7
    K_8,                // KEY_8
    K_9,                // KEY_9
    K_0,                // KEY_0
    SK(COMBO_MINUS),    // KEY_MINUS
    SK(COMBO_EQUAL),    // KEY_EQUAL
    SK(COMBO_RUBOUT),   // KEY_BACKSPACE
    SK(COMBO_EDIT),     // KEY_TAB
    K_Q,                // KEY_Q
    K_W,                // KEY_W
    K_E,                // KEY_E
    K_R,                // KEY_R
    K_T,                // KEY_T
    K_Y,                // KEY_Y
    K_U,                // KEY_U
    K_I,                // KEY_I
    K_O,                // KEY_O
    K_P,                // KEY_P
    SK(COMBO_OPEN_PAREN), // KEY_LEFTBRACE
    SK(COMBO_CLOSE_PAREN),// KEY_RIGHTBRACE
    K_NEWLINE,          // KEY_ENTER
    NA,                 // KEY_LEFTCTRL
    K_A,                // KEY_A
    K_S,                // KEY_S
    K_D,                // KEY_D
    K_F,                // KEY_F
    K_G,                // KEY_G
    K_H,                // KEY_H
    K_J,                // KEY_J
    K_K,                // KEY_K
    K_L,                // KEY_L
    SK(COMBO_SEMICOLON),// KEY_SEMICOLON
    SK(COMBO_DOUBLE_QUOTE),// KEY_APOSTROPHE
    SK(COMBO_EXP),      // KEY_GRAVE
    K_SHIFT,            // KEY_LEFTSHIFT
    SK(COMBO_QUESTION), // KEY_BACKSLASH
    K_Z,                // KEY_Z
    K_X,                // KEY_X
    K_C,                // KEY_C
    K_V,                // KEY_V
    K_B,                // KEY_B
    K_N,                // KEY_N
    K_M,                // KEY_M
    SK(COMBO_COMMA),    // KEY_COMMA
    K_DOT,              // KEY_DOT
    SK(COMBO_SLASH),    // KEY_SLASH
    K_SHIFT,            // KEY_RIGHTSHIFT
    SK(COMBO_ASTERISK), // KEY_KPASTERISK
    NA,                 // KEY_LEFTALT
    K_SPACE,            // KEY_SPACE
    SK(COMBO_CAPS_LOCK),// KEY_CAPSLOCK
    NA,                 // KEY_F1
    NA,                 // KEY_F2
    SK(MACRO_LOAD),     // KEY_F3
    NA,                 // KEY_F4
    NA,                 // KEY_F5
    NA,                 // KEY_F6
    NA,                 // KEY_F7
    NA,                 // KEY_F8
    NA,                 // KEY_F9
    NA,                 // KEY_F10
    NA,                 // KEY_NUMLOCK
    NA,                 // KEY_SCROLLLOCK
    K_7,                // KEY_KP7
    K_8,                // KEY_KP8
    K_9,                // KEY_KP9
    SK(COMBO_MINUS),    // KEY_KPMINUS
    K_4,                // KEY_KP4
    K_5,                // KEY_KP5
    K_6,                // KEY_KP6
    SK(COMBO_PLUS),     // KEY_KPPLUS
    K_1,                // KEY_KP1
    K_2,                // KEY_KP2
    K_3,                // KEY_KP3
    K_0,                // KEY_KP0
    K_DOT,              // KEY_KPDOT
    NA,                 //
    NA,                 // KEY_ZENKAKUHANKAKU
    NA,                 // KEY_102ND
    NA,                 // KEY_F11
    NA,                 // KEY_F12
    NA,                 // KEY_RO
    NA,                 // KEY_KATAKANA
    NA,                 // KEY_HIRAGANA
    NA,                 // KEY_HENKAN
    NA,                 // KEY_KATAKANAHIRAGANA
    NA,                 // KEY_MUHENKAN
    NA,                 // KEY_KPJPCOMMA
    K_NEWLINE,          // KEY_KPENTER
    NA,                 // KEY_RIGHTCTRL
    SK(COMBO_SLASH),    // KEY_KPSLASH
    NA,                 // KEY_SYSRQ
    NA,                 // KEY_RIGHTALT
    NA,                 // KEY_LINEFEED
    SK(COMBO_HOME),     // KEY_HOME
    SK(COMBO_UP),       // KEY_UP
    NA,                 // KEY_PAGEUP
    SK(COMBO_LEFT),     // KEY_LEFT
    SK(COMBO_RIGHT),    // KEY_RIGHT
    NA,                 // KEY_END
    SK(COMBO_DOWN),     // KEY_DOWN
    NA,                 // KEY_PAGEDOWN
    NA,                 // KEY_INSERT
    SK(COMBO_RUBOUT)    // KEY_DELETE
};

#endif
