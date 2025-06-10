// TOP-LEVEL

#include "markaacosta.h"

// for debugging / print statements / etc.
#include "print.h"

// ENCODERS

bool encoder_mod_held = false;

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

// the 'encoder mod' (a modifier for extra behavior when this modifier is held down with encoder movement)
// note - for this to work in sophisticated cases, we have to set it with explicit 'row' and 'col' matrix positions
// right now, it is set to correspond to `KC_A` - the 'a' key. should you wish to change it in the future, you
// must update the explicit row and column below

#define ENCODER_MOD_ROW 3
#define ENCODER_MOD_COL 1

// keyboard EVENT LOOP, basically

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
            uint8_t         index = 0;
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

// TAP DANCE FUNCTIONS

void td_fn_shift_home(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // SEND_STRING("Safety dance!");
        // reset_tap_dance(state);
        tap_code(KC_HOME);
    } else if (state->count == 2) {
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
    } else if (state->count == 2) {
        // SEND_STRING("Safety dance!");
        // reset_tap_dance(state);
        register_code(KC_LSFT);
        tap_code(KC_END);
        unregister_code(KC_LSFT);
    }
}

void td_fn_ctrl_shift_left_arrow(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_LCTL);
        tap_code(KC_LEFT);
        unregister_code(KC_LCTL);
    } else if (state->count == 2) {
        register_code(KC_LCTL);
        register_code(KC_LSFT);
        tap_code(KC_LEFT);
        unregister_code(KC_LSFT);
        unregister_code(KC_LCTL);
    }
}

void td_fn_ctrl_shift_right_arrow(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_LCTL);
        tap_code(KC_RGHT);
        unregister_code(KC_LCTL);
    } else if (state->count == 2) {
        register_code(KC_LCTL);
        register_code(KC_LSFT);
        tap_code(KC_RGHT);
        unregister_code(KC_LSFT);
        unregister_code(KC_LCTL);
    }
}

void td_fn_ctrl_shift_up_arrow(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_LCTL);
        tap_code(KC_UP);
        unregister_code(KC_LCTL);
    } else if (state->count == 2) {
        register_code(KC_LCTL);
        register_code(KC_LSFT);
        tap_code(KC_UP);
        unregister_code(KC_LSFT);
        unregister_code(KC_LCTL);
    }
}

void td_fn_ctrl_shift_down_arrow(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_LCTL);
        tap_code(KC_DOWN);
        unregister_code(KC_LCTL);
    } else if (state->count == 2) {
        register_code(KC_LCTL);
        register_code(KC_LSFT);
        tap_code(KC_DOWN);
        unregister_code(KC_LSFT);
        unregister_code(KC_LCTL);
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
    // taps: 1 -> press "ctrl + up", 2 -> press "ctrl + shift + up"
    [TD_CTRL_SHIFT_UP_ARROW] = ACTION_TAP_DANCE_FN(td_fn_ctrl_shift_up_arrow),
    // taps: 1 -> press "ctrl + down", 2 -> press "ctrl + shift + down"
    [TD_CTRL_SHIFT_DOWN_ARROW] = ACTION_TAP_DANCE_FN(td_fn_ctrl_shift_down_arrow),
    // taps: 1 -> press "ctrl + left", 2 -> press "ctrl + shift + left"
    [TD_CTRL_SHIFT_LEFT_ARROW] = ACTION_TAP_DANCE_FN(td_fn_ctrl_shift_left_arrow),
    // taps: 1 -> press "ctrl + right", 2 -> press "ctrl + shift + right"
    [TD_CTRL_SHIFT_RIGHT_ARROW] = ACTION_TAP_DANCE_FN(td_fn_ctrl_shift_right_arrow),
};

const uint16_t PROGMEM vol_down_combo[] = {TD(TD_ALT_CAPS), KC_LEFT_SHIFT, KC_COMMA, COMBO_END};
const uint16_t PROGMEM vol_up_combo[]   = {TD(TD_ALT_CAPS), KC_LEFT_SHIFT, KC_DOT, COMBO_END};
combo_t                key_combos[]     = {
    // COMBO(vol_down_combo, KC_AUDIO_VOL_DOWN),
    // COMBO(vol_up_combo, KC_AUDIO_VOL_UP),
};
