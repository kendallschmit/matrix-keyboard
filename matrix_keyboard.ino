// Outputting on COLUMNS (1-8 + special), checking on ROWS (9-16 + special)
//PinKeyMap[BoardOUT-1][BoardCHECK-9];
//   Board CHECK:  9  10  11  12  13  14  15  16  23  24
//   Board OUT:  1{{Marrel, Tab Set, Tab Clr, Lmar, Rmar, 0 ,0 ,0 ,0 ,0},
//               2 {Space, Return, Tab, BackSp, Express, half, Del, Reloc, 0, 0},
//               3 {-, 1/4, '"', +, 0, 0, 0, 0, 0, 0},
//               4 {9, O, L, ., 0, P, :, ?, 0, 0},
//               5 {7, U, J, M, 8, I, K, ",", 0, 0},
//               6 {5, T, G, B, 6, Y, H, N, 0, 0},
//               7 {3, E, D, C, 4, R, F, V, 0, 0},
//               8 {1, Q, A, Z, 2, W, S, X, 0, 0}}

// capslock is: board 21
// shift is: board 22
// repeat is: board 17
// top /bottom switch: 18/ 19
// led out: lhs board 23, rhs board 24

const int MATRIX_OUT[] = { 2, 3, 4, 5, 6, 7, 8, 9, };
// maps to board pins:     1  2  3  4  5  6  7  8
#define MATRIX_OUT_COUNT 8

const int MATRIX_IN[] = { 12, 14, 15, 16, 18, 19, 20, 21, 10, 11 };
// maps to board pins:    9   10  11  12  13  14  15  16  23  24
#define MATRIX_IN_COUNT 10

const int MATRIX_KEY[MATRIX_IN_COUNT][MATRIX_OUT_COUNT] {
    { KEY_ESC, MODIFIERKEY_CTRL, MODIFIERKEY_ALT, MODIFIERKEY_RIGHT_GUI, MODIFIERKEY_RIGHT_CTRL, 0, 0, 0, 0, 0 },
    { KEY_SPACE, KEY_ENTER, KEY_TAB, KEY_BACKSPACE, MODIFIERKEY_RIGHT_ALT, KEY_RIGHT_BRACE, KEY_DELETE, KEY_BACKSLASH, 0, 0 },
    { KEY_MINUS, KEY_LEFT_BRACE, KEY_QUOTE, KEY_EQUAL, 0, 0, 0, 0, 0, 0 },
    { KEY_9, KEY_O, KEY_L, KEY_PERIOD, KEY_0, KEY_P, KEY_SEMICOLON, KEY_SLASH, 0, 0 },
    { KEY_7, KEY_U, KEY_J, KEY_M, KEY_8, KEY_I, KEY_K, KEY_COMMA, 0, 0 },
    { KEY_5, KEY_T, KEY_G, KEY_B, KEY_6, KEY_Y, KEY_H, KEY_N, 0, 0 },
    { KEY_3, KEY_E, KEY_D, KEY_C, KEY_4, KEY_R, KEY_F, KEY_V, 0, 0 },
    { KEY_1, KEY_Q, KEY_A, KEY_Z, KEY_2, KEY_W, KEY_S, KEY_X, 0, 0 }
};

#define DEBOUNCE_MS 20
int matrix_debounce[MATRIX_IN_COUNT][MATRIX_OUT_COUNT] = {};
bool matrix_state[MATRIX_IN_COUNT][MATRIX_OUT_COUNT] = {};

void setup() {
    for (int i = 0; i < MATRIX_OUT_COUNT; i++) {
        pinMode(MATRIX_OUT[i], OUTPUT);
        digitalWrite(MATRIX_OUT[i], HIGH);
    }
    for (int i = 0; i < MATRIX_IN_COUNT; i++) {
        pinMode(MATRIX_IN[i], INPUT_PULLUP);
    }
}

void loop() {
    for (int out = 0; out < MATRIX_OUT_COUNT; out++) {
        int out_pin = MATRIX_OUT[out];
        digitalWrite(out_pin, LOW);
        for (int in = 0; in < MATRIX_IN_COUNT; in++) {
            if (matrix_debounce[out][in] != 0) {
                if (millis() - matrix_debounce[out][in] >= DEBOUNCE_MS) {
                    matrix_debounce[out][in] = 0;
                }
            }
            else {
                int in_pin = MATRIX_IN[in];
                bool prev_state = matrix_state[out][in];
                bool cur_state = digitalRead(in_pin);
                if (prev_state != cur_state) {
                    int key = MATRIX_KEY[out][in];
                    if (cur_state == LOW) {
                        Keyboard.press(key);
                    }
                    else {
                        Keyboard.release(key);
                    }
                    matrix_debounce[out][in] = millis();
                    matrix_state[out][in] = cur_state;
                }
            }
        }
        digitalWrite(out_pin, HIGH);
    }
}
