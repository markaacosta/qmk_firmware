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

#define TAPPING_TERM 200

#include QMK_KEYBOARD_H

// clang-format off

enum layers{
    MAC_BASE,
    MAC_FN,
    _main_layer,
    _function_layer,
    _extras_layer,
    _numpad_layer,
};

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
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ALT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT_ALT, KC_CAPS),
    [TD_SUPER_NUMPAD_LAYER] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_LCMD, _numpad_layer),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CTL_DEL:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_DEL);
                unregister_code(KC_LCTL);
            }
            return false; // Skip normal processing
        case BACKSPC_DEL:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_BACKSPACE);
                unregister_code(KC_LCTL);
            }
            return false; // Skip normal processing
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
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_DEL,             KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        LT(_extras_layer, KC_ESC),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        KC_LCTL,  TD(TD_SUPER_NUMPAD_LAYER),  TD(TD_ALT_CAPS),                                KC_SPC,                                 KC_RALT,  MO(_function_layer), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [_function_layer] = LAYOUT_ansi_82(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,            RM_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,            _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
    [_extras_layer] = LAYOUT_ansi_82(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_HOME,    KC_END,  BACKSPC_DEL,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_UP,  _______,  KC_PRINT_SCREEN,  KC_PGUP,    KC_PGDN,  CTL_DEL,            _______,
        _______,  _______,  _______,  KC_DELETE,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_RIGHT,  _______,  _______,              _______,            _______,
        _______,            _______,  _______,  QK_MAKE,  _______,  QK_BOOT,  _______,  _______,  _______,  _______,  KC_PAUSE,              _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______),
    [_numpad_layer] = LAYOUT_ansi_82(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        KC_KP_SLASH,  KC_KP_ASTERISK,  KC_KP_MINUS,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        KC_KP_PLUS,  KC_KP_7,  KC_KP_8,  KC_KP_9,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  KC_KP_4,  KC_KP_5,  KC_KP_6,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        KC_KP_ENTER,            KC_KP_1,  KC_KP_2,  KC_KP_3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  KC_KP_DOT,                                KC_KP_0,                                _______,  _______,    _______,  _______,  _______,  _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [_main_layer] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_function_layer]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [_extras_layer]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [_numpad_layer]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU)}
};
#endif // ENCODER_MAP_ENABLE
