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
    MAC_BASE,
    MAC_FN,
    // original name: WIN_BASE
    _main_layer,
    // original name: WIN_FN
    _function_layer,
    _extras_layer,
    _mouse_layer,
    _numpad_layer,
};

#endif // !USERSPACE
