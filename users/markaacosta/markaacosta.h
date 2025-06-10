#ifndef USERSPACE
#define USERSPACE

#include "quantum.h"

// Each layer gets a name for readability.
// The underscores don't mean anything - you can
// have a layer called STUFF or any other name.
// Layer names don't all need to be of the same
// length, and you can also skip them entirely
// and just use numbers.
enum my_layers {
    // if these next two layers enabled, breaks parity and config doesn't work for durgod k310
    // MAC_BASE,
    // MAC_FN,
    // original name: WIN_BASE
    _main_layer,
    // original name: WIN_FN
    _function_layer,
    _extras_layer,
    _mouse_layer,
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

// TAP DANCE

enum {
    TD_ALT_CAPS,
    TD_SUPER_NUMPAD_LAYER,
    TD_SHIFT_HOME,
    TD_SHIFT_END,
    TD_CTRL_SHIFT_UP_ARROW,
    TD_CTRL_SHIFT_DOWN_ARROW,
    TD_CTRL_SHIFT_LEFT_ARROW,
    TD_CTRL_SHIFT_RIGHT_ARROW,
};

#define NUM_TAP_DANCE_ACTIONS 8

tap_dance_action_t tap_dance_actions[NUM_TAP_DANCE_ACTIONS];

void td_fn_shift_home(tap_dance_state_t *state, void *user_data);
void td_fn_shift_end(tap_dance_state_t *state, void *user_data);
void td_fn_ctrl_shift_up_arrow(tap_dance_state_t *state, void *user_data);
void td_fn_ctrl_shift_down_arrow(tap_dance_state_t *state, void *user_data);
void td_fn_ctrl_shift_left_arrow(tap_dance_state_t *state, void *user_data);
void td_fn_ctrl_shift_right_arrow(tap_dance_state_t *state, void *user_data);

// KEY COMBO

#define NUM_KEY_COMBOS 0
#define NUM_KEY_COMBO_ARGS 4

const uint16_t PROGMEM vol_down_combo[NUM_KEY_COMBO_ARGS];
const uint16_t PROGMEM vol_up_combo[NUM_KEY_COMBO_ARGS];
combo_t                key_combos[NUM_KEY_COMBOS];

#endif // !USERSPACE
