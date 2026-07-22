#include <zephyr/kernel.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT) && !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

// The peripheral half's display widgets (e.g. gem's layer widget) reference
// keymap state that only lives on the central. Stub these so the peripheral
// build links. Signatures track ZMK 0.4 (zmk/keymap.h, zmk/behavior_queue.h).
// If a future ZMK compiles the keymap on peripherals too, these will collide
// (multiple definition) — delete the file + its config/CMakeLists.txt entry then.
int zmk_keymap_layer_activate(zmk_keymap_layer_id_t layer, bool locking) {
    return 0;
}

int zmk_keymap_layer_deactivate(zmk_keymap_layer_id_t layer, bool locking) {
    return 0;
}

zmk_keymap_layer_index_t zmk_keymap_highest_layer_active(void) {
    return 0;
}

int zmk_behavior_queue_add(const struct zmk_behavior_binding_event *event,
                           const struct zmk_behavior_binding behavior, bool press,
                           uint32_t wait) {
    return 0;
}

#endif
