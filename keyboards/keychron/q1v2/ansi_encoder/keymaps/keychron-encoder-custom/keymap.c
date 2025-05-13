/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// for debugging / print statements / etc.
#include "print.h"
// include shared USERSPACE configuration
#include "users/markaacosta/markaacosta.h"

#include QMK_KEYBOARD_H

// clang-format off

enum custom_keycodes {
    CTL_DEL = SAFE_RANGE,
    BACKSPC_DEL,
    NUM_F1,
    NUM_F2,
    NUM_F3,
    NUM_F4,
    NUM_F5,
    NUM_F6,
    NUM_F7,
    NUM_F8,
    NUM_F9,
    NUM_F10,
};

// Tap Dance declarations
enum {
    TD_ALT_CAPS,
    TD_SUPER_NUMPAD_LAYER,
    TD_SHIFT_HOME,
    TD_SHIFT_END,
};

// Tap Dance functions

void td_fn_shift_home(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // SEND_STRING("Safety dance!");
        // reset_tap_dance(state);
        tap_code(KC_HOME);
    }
    else if (state->count == 2) {
        // SEND_STRING("Safety dance!");
        // reset_tap_dance(state);
        register_code(KC_LSFT);
        tap_code(KC_HOME);
        unregister_code(KC_LSFT);
    }
}

void td_fn_shift_end(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // SEND_STRING("Safety dance!");
        // reset_tap_dance(state);
        tap_code(KC_END);
    }
    else if (state->count == 2) {
        // SEND_STRING("Safety dance!");
        // reset_tap_dance(state);
        register_code(KC_LSFT);
        tap_code(KC_END);
        unregister_code(KC_LSFT);
    }
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // taps: 1 -> alt key, 2 -> caps lock
    [TD_ALT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_RALT, KC_CAPS),
    // taps: 1 -> super key, 2 -> numpad layer
    [TD_SUPER_NUMPAD_LAYER] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_LCMD, _numpad_layer),
    // taps: 1 -> press "home", 2 -> press "shift + home"
    [TD_SHIFT_HOME] = ACTION_TAP_DANCE_FN(td_fn_shift_home),
    // taps: 1 -> press "end", 2 -> press "shift + end"
    [TD_SHIFT_END] = ACTION_TAP_DANCE_FN(td_fn_shift_end),
};

const uint16_t PROGMEM vol_down_combo[] = {TD(TD_ALT_CAPS), KC_LEFT_SHIFT, KC_COMMA, COMBO_END};
const uint16_t PROGMEM vol_up_combo[] = {TD(TD_ALT_CAPS), KC_LEFT_SHIFT, KC_DOT, COMBO_END};
combo_t key_combos[] = {
    // COMBO(vol_down_combo, KC_AUDIO_VOL_DOWN),
    // COMBO(vol_up_combo, KC_AUDIO_VOL_UP),
};

// the 'encoder mod' (a modifier for extra behavior when this modifier is held down with encoder movement)
// note - for this to work in sophisticated cases, we have to set it with explicit 'row' and 'col' matrix positions
// right now, it is set to correspond to `KC_A` - the 'a' key. should you wish to change it in the future, you
// must update the explicit row and column below

static bool encoder_mod_held = false;

#define ENCODER_MOD_ROW 3
#define ENCODER_MOD_COL 1

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // // for debugging purposes, if / when needed...
    // uint8_t layer = get_highest_layer(layer_state);
    // uprintf("Current layer: %d\n", layer);

    if (record->event.key.row == ENCODER_MOD_ROW && record->event.key.col == ENCODER_MOD_COL) {
        uprintf("ENCODER_MOD detected: %s\n", record->event.pressed ? "pressed" : "released");
        encoder_mod_held = record->event.pressed;

        return true;
    }
    switch (keycode) {
        case CTL_DEL:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_DEL);
                unregister_code(KC_LCTL);
            }
            return false;
        case BACKSPC_DEL:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_BACKSPACE);
                unregister_code(KC_LCTL);
            }
            return false;
        case NUM_F1 ... NUM_F10:
            uint8_t index = 0;
            static uint16_t num_press_timer[10]; // one timer for each key
            index = keycode - NUM_F1;

            if (record->event.pressed) {
                num_press_timer[index] = timer_read();
            } else {
                uint16_t elapsed = timer_elapsed(num_press_timer[index]);
                if (elapsed < TAPPING_TERM) {
                    tap_code(KC_1 + index); // send number
                } else {
                    tap_code(KC_F1 + index); // send F-key
                }
            }
            return false;
    }
    return true; // Let QMK handle other keycodes normally
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    // get active layer
    uint8_t layer = get_highest_layer(layer_state);
    // uprintf("Layer while encoding moved: %d\n", layer);

    if (layer == _mouse_layer) {
        if (encoder_mod_held) {
            tap_code(clockwise ? KC_MS_UP : KC_MS_DOWN);
        } else {
            tap_code(clockwise ? KC_MS_RIGHT : KC_MS_LEFT);
        }
    }
    // the default layer is 0, so encoder presses are actually recognized at 0 (mac), not 3 (linux)
    else if (layer <= _main_layer) {
        tap_code(clockwise ? KC_VOLU : KC_VOLD);
    }
    return false;
}

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_82(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LOPT,  KC_LCMD,                                KC_SPC,                                 KC_RCMD,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_ansi_82(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   _______,            RM_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),

    [_main_layer] = LAYOUT_ansi_82(
        // 1. esc, 2. f1, 3. f2, 4. f3, 5. f4, 6. f5, 7. f6, 8. f7, 9. f8, 10. f9, 11. f10, 12. f11, 13. f12, 14. home, 15. encoder
        KC_CAPS,   LT(_mouse_layer, KC_F1),    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_HOME,             KC_MPLY,
        // 1. grave, 2. 1, 3. 2, 4. 3, 5. 4, 6. 5, 7. 6, 8. 7, 9. 8, 10. 9, 11. 0, 12. -, 13. =, 14. backspace, 15. end
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            LT(_numpad_layer, KC_END),
        // 1. tab, 2. q, 3. w, 4. e, 5. r, 6. t, 7. y, 8. u, 9. i, 10. o, 11. p, 12. [, 13. ], 14. back-slash, 15. pageup
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGUP,
        // 1. caps-lock, 2. a, 3. s, 4. d, 5. f, 6. g, 7. h, 8. j, 9. k, 10. l, 11. semicolon, 12. quote, 13. enter, 14. pagedown
        LT(_extras_layer, KC_ESC),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_PGDN,
        // 1. left-shift, 2. z, 3. x, 4. c, 5. v, 6. b, 7. n, 8. m, 9. comma, 10. period, 11. forward-slash, 12. right-shift, 13. up
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        // 1. left-control, 2. super, 3. left-alt, 4. SPACE, 5. right-alt, 6. function, 7. right-control, 8. left, 9. down, 10. right
        KC_LCTL,  KC_LCMD,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(_function_layer), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [_function_layer] = LAYOUT_ansi_82(
        // 1. esc, 2. f1, 3. f2, 4. f3, 5. f4, 6. f5, 7. f6, 8. f7, 9. f8, 10. f9, 11. f10, 12. f11, 13. f12, 14. del, 15. encoder
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            RM_TOGG,
        // 1. grave, 2. 1, 3. 2, 4. 3, 5. 4, 6. 5, 7. 6, 8. 7, 9. 8, 10. 9, 11. 0, 12. -, 13. =, 14. backspace, 15. pageup
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. tab, 2. q, 3. w, 4. e, 5. r, 6. t, 7. y, 8. u, 9. i, 10. o, 11. p, 12. [, 13. ], 14. back-slash, 15. pagedown
        RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. caps-lock, 2. a, 3. s, 4. d, 5. f, 6. g, 7. h, 8. j, 9. k, 10. l, 11. semicolon, 12. quote, 13. enter, 14. home
        _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        // 1. left-shift, 2. z, 3. x, 4. c, 5. v, 6. b, 7. n, 8. m, 9. comma, 10. period, 11. forward-slash, 12. right-shift, 13. up
        _______,            KC_MPRV, KC_VOLD, KC_MPLY, KC_VOLU, KC_MNXT,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        // 1. left-control, 2. super, 3. left-alt, 4. SPACE, 5. right-alt, 6. function, 7. right-control, 8. left, 9. down, 10. right
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
    [_extras_layer] = LAYOUT_ansi_82(
        // 1. esc, 2. f1, 3. f2, 4. f3, 5. f4, 6. f5, 7. f6, 8. f7, 9. f8, 10. f9, 11. f10, 12. f11, 13. f12, 14. del, 15. encoder
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. grave, 2. 1, 3. 2, 4. 3, 5. 4, 6. 5, 7. 6, 8. 7, 9. 8, 10. 9, 11. 0, 12. -, 13. =, 14. backspace, 15. pageup
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  TD(TD_SHIFT_HOME),    TD(TD_SHIFT_END),  BACKSPC_DEL,            _______,
        // 1. tab, 2. q, 3. w, 4. e, 5. r, 6. t, 7. y, 8. u, 9. i, 10. o, 11. p, 12. [, 13. ], 14. back-slash, 15. pagedown
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_INS,  _______,  KC_PRINT_SCREEN,  KC_PGUP,    KC_PGDN,  CTL_DEL,            _______,
        // 1. caps-lock, 2. a, 3. s, 4. d, 5. f, 6. g, 7. h, 8. j, 9. k, 10. l, 11. semicolon, 12. quote, 13. enter, 14. home
        _______,  _______,  _______,  KC_DELETE,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,  KC_RGHT,  _______,  _______,              _______,            _______,
        // 1. left-shift, 2. z, 3. x, 4. c, 5. v, 6. b, 7. n, 8. m, 9. comma, 10. period, 11. forward-slash, 12. right-shift, 13. up
        _______,            _______,  _______,  QK_MAKE,  _______,  QK_BOOT,  _______,  _______,  _______,  _______,  KC_PAUSE,              _______,  _______,
        // 1. left-control, 2. super, 3. left-alt, 4. SPACE, 5. right-alt, 6. function, 7. right-control, 8. left, 9. down, 10. right
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
    [_mouse_layer] = LAYOUT_ansi_82(
        // 1. esc, 2. f1, 3. f2, 4. f3, 5. f4, 6. f5, 7. f6, 8. f7, 9. f8, 10. f9, 11. f10, 12. f11, 13. f12, 14. del, 15. encoder
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            MS_BTN1,
        // 1. grave, 2. 1, 3. 2, 4. 3, 5. 4, 6. 5, 7. 6, 8. 7, 9. 8, 10. 9, 11. 0, 12. -, 13. =, 14. backspace, 15. pageup
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. tab, 2. q, 3. w, 4. e, 5. r, 6. t, 7. y, 8. u, 9. i, 10. o, 11. p, 12. [, 13. ], 14. back-slash, 15. pagedown
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  MS_BTN1,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. caps-lock, 2. a, 3. s, 4. d, 5. f, 6. g, 7. h, 8. j, 9. k, 10. l, 11. semicolon, 12. quote, 13. enter, 14. home
        _______,  XXXXXXX,  _______,  _______,  _______,  _______,  MS_LEFT,  MS_DOWN,  MS_UP,  MS_RGHT,  _______,  _______,              _______,            _______,
        // 1. left-shift, 2. z, 3. x, 4. c, 5. v, 6. b, 7. n, 8. m, 9. comma, 10. period, 11. forward-slash, 12. right-shift, 13. up
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,
        // 1. left-control, 2. super, 3. left-alt, 4. SPACE, 5. right-alt, 6. function, 7. right-control, 8. left, 9. down, 10. right
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
    [_numpad_layer] = LAYOUT_ansi_82(
        // 1. esc, 2. f1, 3. f2, 4. f3, 5. f4, 6. f5, 7. f6, 8. f7, 9. f8, 10. f9, 11. f10, 12. f11, 13. f12, 14. del, 15. encoder
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. grave, 2. 1, 3. 2, 4. 3, 5. 4, 6. 5, 7. 6, 8. 7, 9. 8, 10. 9, 11. 0, 12. -, 13. =, 14. backspace, 15. pageup
        KC_KP_SLASH,  KC_KP_ASTERISK,  KC_KP_MINUS,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. tab, 2. q, 3. w, 4. e, 5. r, 6. t, 7. y, 8. u, 9. i, 10. o, 11. p, 12. [, 13. ], 14. back-slash, 15. pagedown
        KC_KP_PLUS,  KC_KP_7,  KC_KP_8,  KC_KP_9,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        // 1. caps-lock, 2. a, 3. s, 4. d, 5. f, 6. g, 7. h, 8. j, 9. k, 10. l, 11. semicolon, 12. quote, 13. enter, 14. home
        _______,  KC_KP_4,  KC_KP_5,  KC_KP_6,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        // 1. left-shift, 2. z, 3. x, 4. c, 5. v, 6. b, 7. n, 8. m, 9. comma, 10. period, 11. forward-slash, 12. right-shift, 13. up
        KC_KP_ENTER,            KC_KP_1,  KC_KP_2,  KC_KP_3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,
        // 1. left-control, 2. super, 3. left-alt, 4. SPACE, 5. right-alt, 6. function, 7. right-control, 8. left, 9. down, 10. right
        _______,  _______,  KC_KP_DOT,                                KC_KP_0,                                _______,  _______,    _______,  _______,  _______,  _______)
};

// #if defined(ENCODER_MAP_ENABLE)
// const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
//     [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
//     [MAC_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
//     [_main_layer] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
//     [_function_layer]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
//     [_extras_layer]   = { ENCODER_CCW_CW(KC_PGUP, KC_PGDN)},
//     [_mouse_layer]   = { ENCODER_CCW_CW(MS_LEFT, MS_RGHT)},
//     [_numpad_layer]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)}
// };
// #endif // ENCODER_MAP_ENABLE
