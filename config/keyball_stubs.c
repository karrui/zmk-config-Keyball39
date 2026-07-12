#include <zephyr/kernel.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT) && !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

// The nice!view peripheral widget calls this BLE-split-only function to show
// the "connected to central" state. Under the ESB split transport BLE is off
// and the symbol doesn't exist, so stub it. An ESB peripheral has no standing
// connection to report; it is reachable whenever powered, so report connected.
#if !IS_ENABLED(CONFIG_ZMK_SPLIT_BLE)
bool zmk_split_bt_peripheral_is_connected(void) {
    return true;
}
#endif

// The peripheral's display widgets (e.g. the layer widget) reference keymap
// state that only lives on the central. Stub these so the widgets link.
// Signatures track ZMK 0.4 (zmk/keymap.h, zmk/behavior_queue.h).
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
