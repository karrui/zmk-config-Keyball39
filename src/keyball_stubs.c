#include <zephyr/kernel.h>
#include <zephyr/toolchain.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT) && !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) &&               \
    IS_ENABLED(CONFIG_SHIELD_NICE_VIEW_GEM)

// The peripheral half's display widgets (e.g. gem's layer widget) reference
// keymap state that only lives on the central. Stub these so the peripheral
// build links. Signatures track ZMK 0.4 (zmk/keymap.h, zmk/behavior_queue.h).
// These are weak, Gem-only link shims: a future ZMK implementation wins without
// a duplicate-symbol failure. Do not use them for peripheral behavior logic.
// They intentionally report the Base layer because layer state is owned by the
// dongle and is not synchronized to the peripherals.
__weak int zmk_keymap_layer_activate(zmk_keymap_layer_id_t layer, bool locking) {
    return 0;
}

__weak int zmk_keymap_layer_deactivate(zmk_keymap_layer_id_t layer, bool locking) {
    return 0;
}

__weak zmk_keymap_layer_index_t zmk_keymap_highest_layer_active(void) {
    return 0;
}

__weak int zmk_behavior_queue_add(const struct zmk_behavior_binding_event *event,
                                  const struct zmk_behavior_binding behavior, bool press,
                                  uint32_t wait) {
    return 0;
}

#endif
