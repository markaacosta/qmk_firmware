#define TAPPING_TERM 200

#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

/* THIS FILE WAS GENERATED!
 *
 * This file was generated by qmk json2c. You may or may not want to
 * edit it directly.
 */

// Each layer gets a name for readability.
// The underscores don't mean anything - you can
// have a layer called STUFF or any other name.
// Layer names don't all need to be of the same
// length, and you can also skip them entirely
// and just use numbers.
enum layer_names {
    _main_layer,
    _function_layer,
    _extras_layer,
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
    NUM_F10
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_main_layer]  = LAYOUT_all(
        // fn keys - 16
        MT(MOD_LCTL, KC_ESC), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PSCR, KC_SCRL, KC_PAUS,
        // num keys - 21
        KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_INS, KC_HOME, KC_PGUP, KC_NUM, KC_PSLS, KC_PAST, KC_PMNS,
        // qwerty - 21
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, KC_END, KC_PGDN, KC_P7, KC_P8, KC_P9, KC_PPLS,
        // asdfgh - 17
        LT(_extras_layer, KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, KC_P4, KC_P5, KC_P6,
        // zxcvbn - 18
        KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_P1, KC_P2, KC_P3, KC_PENT,
        // modifiers, space, etc. - 13
        KC_LCTL, KC_LGUI, MT(MOD_LALT, KC_CAPS_LOCK), KC_SPC, KC_RALT, MO(_function_layer), KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_P0, KC_PDOT),
    [_function_layer]  = LAYOUT_all(
        // fn keys - 16
        _______, KC_MPLY, KC_MSTP, KC_MPRV, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, _______, _______, _______, _______, _______,
        // num keys - 21
        _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, _______, _______, _______, _______, _______, _______, _______,
        // qwerty - 21
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        // asdfgh - 17
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        // zxcvbn - 18
        _______, _______, _______, _______, _______, _______, KC_MEDIA_PLAY_PAUSE, _______, _______, KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK, _______, _______, _______, _______, _______, _______, _______,
        // _______, _______, etc. - 13
        _______, _______, _______, _______, _______, GU_TOGG, _______, _______, _______, _______, _______, _______, _______),
    [_extras_layer]  = LAYOUT_all(
        // fn keys - 16
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        // num keys - 21
        _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_HOME, KC_END, BACKSPC_DEL, _______, _______, _______, _______, _______, _______, _______,
        // qwerty - 21
        _______, _______, _______, _______, _______, _______, _______, _______, KC_UP, _______, KC_PRINT_SCREEN, KC_PGUP, KC_PGDN, CTL_DEL, _______, _______, _______, _______, _______, _______,  _______,
        // asdfgh - 17
        _______, _______, _______, KC_DELETE, _______, _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______, _______, _______, _______, _______, _______,
        // zxcvbn - 18
        _______, _______, _______, _______, QK_MAKE, _______, QK_BOOT, _______, _______, _______, _______, KC_PAUSE, _______, _______, _______, _______, _______, _______,
        // _______, _______, etc. - 13
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______)};

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

#ifdef OTHER_KEYMAP_C
#    include OTHER_KEYMAP_C
#endif // OTHER_KEYMAP_C
