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

#ifndef TARGET_SPECTRUM_h
#define TARGET_SPECTRUM_h

/*
    This header file contains all definitions needed for the Sinclair ZX Spectrum
    target. If you want to define your own target, you can start with below
    definitions and adapt as needed.
*/

/* --- key addresses in target keyboard matrix --------------------------------

    The constants below define the 6 bit addresses of the target keys in the
    MT88xx switch matrix. The lower 4 bits of an address are the `AX` bits,
    the upper 3 bits the `AY` bits.

        bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
            | x |    AY     |      AX       |

    When using Spectratur with a different target system, you will need to
    adjust this. The addresses depend on the keyboard matrix of the target,
    and on how you wired its rows and columns lines to the MT88xx. You may
    also have to add and remove address constants depending on what particular
    keys are present on your target's keyboard.

    This table shows the assignment of the Sinclair ZX Spectrum keyboard lines
    to an MT8808 switch matrix, and the resulting key assignments:

     MT8808 --- X0    X1    X2    X3    X4    X5    X6    X7
          |   --------------------------------------------------
         Y0   |  1     Q     A     0     P  CAPS  ENTER SPACE  | 1 --- KB1 pins
         Y1   |  2     W     S     9     O     Z     L  SYMBOL | 2
         Y2   |  3     E     D     8     I     X     K     M   | 3
         Y3   |  4     R     F     7     U     C     J     N   | 4
         Y4   |  5     T     G     6     Y     V     H     B   | 5
              -------------------------------------------------
                 1     2     3     4     5     6     7     8 --------- KB2 pins
 */
static const uint8_t K_1 = B0000000;
static const uint8_t K_2 = B0010000;
static const uint8_t K_3 = B0100000;
static const uint8_t K_4 = B0110000;
static const uint8_t K_5 = B1000000;
static const uint8_t K_6 = B1000011;
static const uint8_t K_7 = B0110011;
static const uint8_t K_8 = B0100011;
static const uint8_t K_9 = B0010011;
static const uint8_t K_0 = B0000011;
static const uint8_t K_A = B0000010;
static const uint8_t K_B = B1000111;
static const uint8_t K_C = B0110101;
static const uint8_t K_D = B0100010;
static const uint8_t K_E = B0100001;
static const uint8_t K_F = B0110010;
static const uint8_t K_G = B1000010;
static const uint8_t K_H = B1000110;
static const uint8_t K_I = B0100100;
static const uint8_t K_J = B0110110;
static const uint8_t K_K = B0100110;
static const uint8_t K_L = B0010110;
static const uint8_t K_M = B0100111;
static const uint8_t K_N = B0110111;
static const uint8_t K_O = B0010100;
static const uint8_t K_P = B0000100;
static const uint8_t K_Q = B0000001;
static const uint8_t K_R = B0110001;
static const uint8_t K_S = B0010010;
static const uint8_t K_T = B1000001;
static const uint8_t K_U = B0110100;
static const uint8_t K_V = B1000101;
static const uint8_t K_W = B0010001;
static const uint8_t K_X = B0100101;
static const uint8_t K_Y = B1000100;
static const uint8_t K_Z = B0010101;

static const uint8_t K_CAPS   = B0000101;
static const uint8_t K_ENTER  = B0000110;
static const uint8_t K_SPACE  = B0000111;
static const uint8_t K_SYMBOL = B0010111;

/* --- specials ---------------------------------------------------------------

    This enumeration provides the index numbers for combos & macros.
 */
enum SPECIALS {
    COMBO_PERIOD = 0,       // combos
    COMBO_COMMA,
    COMBO_SEMICOLON,
    COMBO_SLASH,
    COMBO_ASTERISK,
    COMBO_PLUS,
    COMBO_MINUS,
    COMBO_QUOTE,
    COMBO_DOUBLE_QUOTE,
    COMBO_EQUAL,
    COMBO_UNDERSCORE,
    COMBO_DELETE,
    COMBO_UP,
    COMBO_DOWN,
    COMBO_LEFT,
    COMBO_RIGHT,
    COMBO_EXTENDED,
    COMBO_CAPS_LOCK,
    END_OF_COMBOS,          // combo/macro divider
    MACRO_FORMAT_SERIAL,    // macros
    MACRO_LOAD_SERIAL,
    END_OF_SPECIALS         // This must be lower than `TOGGLE`!
};

/* --- combo definitions ------------------------------------------------------

    Each combo defines the keys that should be pressed/released on the target
    when the combo is used. When pressing, keys are pressed in order left to
    right, when releasing right to left.

    Note that it is required to terminate each combo with `NA`! Failure to do
    so will result in crashes.
 */
static const uint8_t combo_period[]       = {K_SYMBOL, K_M, NA};
static const uint8_t combo_comma[]        = {K_SYMBOL, K_N, NA};
static const uint8_t combo_semicolon[]    = {K_SYMBOL, K_O, NA};
static const uint8_t combo_slash[]        = {K_SYMBOL, K_V, NA};
static const uint8_t combo_asterisk[]     = {K_SYMBOL, K_B, NA};
static const uint8_t combo_plus[]         = {K_SYMBOL, K_K, NA};
static const uint8_t combo_minus[]        = {K_SYMBOL, K_J, NA};
static const uint8_t combo_quote[]        = {K_SYMBOL, K_7, NA};
static const uint8_t combo_double_quote[] = {K_SYMBOL, K_P, NA};
static const uint8_t combo_equal[]        = {K_SYMBOL, K_L, NA};
static const uint8_t combo_underscore[]   = {K_SYMBOL, K_0, NA};
static const uint8_t combo_delete[]       = {K_CAPS, K_0, NA};
static const uint8_t combo_up[]           = {K_CAPS, K_7, NA};
static const uint8_t combo_down[]         = {K_CAPS, K_6, NA};
static const uint8_t combo_left[]         = {K_CAPS, K_5, NA};
static const uint8_t combo_right[]        = {K_CAPS, K_8, NA};
static const uint8_t combo_extended[]     = {K_SYMBOL, K_CAPS, NA};
// when the first element is `TOGGLE`, the combo is handled as a toggle key
static const uint8_t combo_caps_lock[]    = {TOGGLE, K_CAPS, NA};

/* --- macro definitions ------------------------------------------------------

    Each macro defines a sequence of keys to be "typed" when it is used. That
    is, the keys listed in a macro will be pressed and released again one by
    one, from left to right. Combos can be used in a macro. Use the `SK`
    preprocessor macro to reference them.

    Note that it is required to terminate each macro with `NA`! Failure to do
    so will result in crashes.
 */
static const uint8_t macro_format_serial[] = {
    SK(COMBO_EXTENDED), SK(COMBO_UNDERSCORE),   // FORMAT
    SK(COMBO_DOUBLE_QUOTE),                     // "
    K_B,                                        // b
    SK(COMBO_DOUBLE_QUOTE),                     // "
    SK(COMBO_SEMICOLON),                        // ;
    K_1, K_9, K_2, K_0, K_0,                    // 19200
    NA
};

static const uint8_t macro_load_serial[] = {
    K_J,                                        // LOAD
    SK(COMBO_ASTERISK),                         // *
    SK(COMBO_DOUBLE_QUOTE),                     // "
    K_B,                                        // b
    SK(COMBO_DOUBLE_QUOTE),                     // "
    NA
};

/* --- specials table ---------------------------------------------------------

    This table aggregates all combos & macros that should be used. The order
    needs to exactly follow the `SPECIALS` enumeration above.
 */
static const uint8_t* SPECIALS[END_OF_SPECIALS] = {
    combo_period,
    combo_comma,
    combo_semicolon,
    combo_slash,
    combo_asterisk,
    combo_plus,
    combo_minus,
    combo_quote,
    combo_double_quote,
    combo_equal,
    combo_underscore,
    combo_delete,
    combo_up,
    combo_down,
    combo_left,
    combo_right,
    combo_extended,
    combo_caps_lock,
    0, // combo/macro divider
    macro_format_serial,
    macro_load_serial
};

/* --- key map ----------------------------------------------------------------

    This map translates input key codes (see input_keycodes.h) to target key
    addresses. For this, the input key code is used as an index into this table.
    Combos & macros can be referenced via the `SK` preprocessor macro.

    Note that since bit 7 is used to distinguish between plain keys (0) and
    special keys (1), i.e. combos & macros, the size of this table must not
    exceed 128.
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
    SK(COMBO_DELETE),   // KEY_BACKSPACE
    NA,                 // KEY_TAB
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
    NA,                 // KEY_LEFTBRACE
    NA,                 // KEY_RIGHTBRACE
    K_ENTER,            // KEY_ENTER
    K_SYMBOL,           // KEY_LEFTCTRL
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
    SK(COMBO_QUOTE),    // KEY_APOSTROPHE
    NA,                 // KEY_GRAVE
    K_CAPS,             // KEY_LEFTSHIFT
    SK(COMBO_DOUBLE_QUOTE), // KEY_BACKSLASH
    K_Z,                // KEY_Z
    K_X,                // KEY_X
    K_C,                // KEY_C
    K_V,                // KEY_V
    K_B,                // KEY_B
    K_N,                // KEY_N
    K_M,                // KEY_M
    SK(COMBO_COMMA),    // KEY_COMMA
    SK(COMBO_PERIOD),   // KEY_DOT
    NA,                 // KEY_SLASH
    K_CAPS,             // KEY_RIGHTSHIFT
    SK(COMBO_ASTERISK), // KEY_KPASTERISK
    K_SYMBOL,           // KEY_LEFTALT
    K_SPACE,            // KEY_SPACE
    SK(COMBO_CAPS_LOCK),// KEY_CAPSLOCK
    NA,                 // KEY_F1
    SK(MACRO_FORMAT_SERIAL),// KEY_F2
    SK(MACRO_LOAD_SERIAL),  // KEY_F3
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
    SK(COMBO_PERIOD),   // KEY_KPDOT
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
    K_ENTER,            // KEY_KPENTER
    K_SYMBOL,           // KEY_RIGHTCTRL
    SK(COMBO_SLASH),    // KEY_KPSLASH
    NA,                 // KEY_SYSRQ
    K_SYMBOL,           // KEY_RIGHTALT
    NA,                 // KEY_LINEFEED
    NA,                 // KEY_HOME
    SK(COMBO_UP),       // KEY_UP
    NA,                 // KEY_PAGEUP
    SK(COMBO_LEFT),     // KEY_LEFT
    SK(COMBO_RIGHT),    // KEY_RIGHT
    NA,                 // KEY_END
    SK(COMBO_DOWN),     // KEY_DOWN
};

#endif
