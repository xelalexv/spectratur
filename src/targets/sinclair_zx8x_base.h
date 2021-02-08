/*
    Copyright 2021 Alexander Vollschwitz <xelalex@gmx.net>

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

#ifndef TARGET_ZX8X_BASE_h
#define TARGET_ZX8X_BASE_h

/*
    This header file contains base definitions common for the Sinclair ZX80 and
    ZX81 targets. For more detailed comments & explanations on defining a target,
    see targets/sinclair_spectrum.h
*/

/* --- key addresses in target keyboard matrix --------------------------------

    The constants below define the 6 bit addresses of the target keys in the
    MT88xx switch matrix. The lower 4 bits of an address are the `AX` bits,
    the upper 3 bits the `AY` bits.

        bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
            | x |    AY     |      AX       |

    This table shows the assignment of the ZX80/81 keyboard lines to an MT8808
    switch matrix, and the resulting key assignments:

        pin     (6)  (23)   (7)  (22)   (8)  (21)   (9)  (20)      ____ ZX80 data bus
     MT8808 --- X0    X1    X2    X3    X4    X5    X6    X7      |     (buffered)
          |   --------------------------------------------------  |
    (18) Y0   | SHIFT  A     Q     1     0     P NEWLINE SPACE | D0   0 ---- ZX81 KBD
    (17) Y1   |  Z     S     W     2     9     O     L     .   | D1   1 connector pin
    (16) Y2   |  X     D     E     3     8     I     K     M   | D2   2
    (15) Y3   |  C     F     R     4     7     U     J     N   | D3   3
    (14) Y4   |  V     G     T     5     6     Y     H     B   | D4   4
              -------------------------------------------------
                 A8    A9   A10   A11   A12   A13   A14   A15 ----------- address bus
                (D3)  (D4) (D5)  (D6)  (D7)  (D8)  (D9)  (D10)    ZX80     via diodes
                (D6)  (D4) (D2)  (D1)  (D3)  (D5)  (D7)  (D8)     ZX81
 */

static const uint8_t K_1 = B0000011;
static const uint8_t K_2 = B0010011;
static const uint8_t K_3 = B0100011;
static const uint8_t K_4 = B0110011;
static const uint8_t K_5 = B1000011;
static const uint8_t K_6 = B1000100;
static const uint8_t K_7 = B0110100;
static const uint8_t K_8 = B0100100;
static const uint8_t K_9 = B0010100;
static const uint8_t K_0 = B0000100;
static const uint8_t K_A = B0000001;
static const uint8_t K_B = B1000111;
static const uint8_t K_C = B0110000;
static const uint8_t K_D = B0100001;
static const uint8_t K_E = B0100010;
static const uint8_t K_F = B0110001;
static const uint8_t K_G = B1000001;
static const uint8_t K_H = B1000110;
static const uint8_t K_I = B0100101;
static const uint8_t K_J = B0110110;
static const uint8_t K_K = B0100110;
static const uint8_t K_L = B0010110;
static const uint8_t K_M = B0100111;
static const uint8_t K_N = B0110111;
static const uint8_t K_O = B0010101;
static const uint8_t K_P = B0000101;
static const uint8_t K_Q = B0000010;
static const uint8_t K_R = B0110010;
static const uint8_t K_S = B0010001;
static const uint8_t K_T = B1000010;
static const uint8_t K_U = B0110101;
static const uint8_t K_V = B1000000;
static const uint8_t K_W = B0010010;
static const uint8_t K_X = B0100000;
static const uint8_t K_Y = B1000101;
static const uint8_t K_Z = B0010000;

static const uint8_t K_SHIFT   = B0000000;
static const uint8_t K_NEWLINE = B0000110;
static const uint8_t K_SPACE   = B0000111;
static const uint8_t K_DOT     = B0010111;

// --- specials ---------------------------------------------------------------

// combo definitions common for ZX80 and ZX81
static const uint8_t combo_left[]         = {K_SHIFT, K_5, NA};
static const uint8_t combo_down[]         = {K_SHIFT, K_6, NA};
static const uint8_t combo_up[]           = {K_SHIFT, K_7, NA};
static const uint8_t combo_right[]        = {K_SHIFT, K_8, NA};
static const uint8_t combo_rubout[]       = {K_SHIFT, K_0, NA};
static const uint8_t combo_dollar[]       = {K_SHIFT, K_U, NA};
static const uint8_t combo_open_paren[]   = {K_SHIFT, K_I, NA};
static const uint8_t combo_close_paren[]  = {K_SHIFT, K_O, NA};
static const uint8_t combo_exp[]          = {K_SHIFT, K_H, NA};
static const uint8_t combo_minus[]        = {K_SHIFT, K_J, NA};
static const uint8_t combo_plus[]         = {K_SHIFT, K_K, NA};
static const uint8_t combo_equal[]        = {K_SHIFT, K_L, NA};
static const uint8_t combo_caps_lock[]    = {TOGGLE, K_SHIFT, NA};
static const uint8_t combo_colon[]        = {K_SHIFT, K_Z, NA};
static const uint8_t combo_semicolon[]    = {K_SHIFT, K_X, NA};
static const uint8_t combo_question[]     = {K_SHIFT, K_C, NA};
static const uint8_t combo_slash[]        = {K_SHIFT, K_V, NA};
static const uint8_t combo_lower[]        = {K_SHIFT, K_N, NA};
static const uint8_t combo_greater[]      = {K_SHIFT, K_M, NA};
static const uint8_t combo_comma[]        = {K_SHIFT, K_DOT, NA};
static const uint8_t combo_pound[]        = {K_SHIFT, K_SPACE, NA};

// macro definitions common for ZX80 and ZX81

#endif
