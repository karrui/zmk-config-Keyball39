#include <zephyr/kernel.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>
#include <zmk/event_manager.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/events/position_state_changed.h>
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ESB)
#include <zmk_split_esb.h>
#endif

#if IS_ENABLED(CONFIG_ZMK_SPLIT) && !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

// The nice!view peripheral widget calls this BLE-split-only function to show
// the "connected to central" state. Under the ESB split transport BLE is off
// and the symbol doesn't exist, so provide it. With damex's module we back it
// with the real link status: its status API `searching` flag means the half is
// sweeping to re-find the central (i.e. disconnected). Otherwise report true.
#if !IS_ENABLED(CONFIG_ZMK_SPLIT_BLE)
bool zmk_split_bt_peripheral_is_connected(void) {
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ESB)
    struct zmk_split_esb_status status;
    zmk_split_esb_get_status(&status);
    return !status.searching;
#else
    return true;
#endif
}

// damex reports ALL_CONNECTED at the ZMK transport layer and never raises
// zmk_split_peripheral_status_changed, so the nice_view connection widget won't
// refresh on its own. Re-check the real link status on each key event and raise
// the event when it flips, so gem updates the icon. Event-driven on keypress
// (not a periodic timer) so there's no extra wakeup draining the battery — the
// check only runs when the half is already awake handling input.
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ESB)
static int esb_last_connected = -1; /* tri-state: unknown until the first check */

static int esb_link_status_listener(const zmk_event_t *eh) {
    ARG_UNUSED(eh);
    struct zmk_split_esb_status status;
    zmk_split_esb_get_status(&status);
    int connected = status.searching ? 0 : 1;
    if (connected != esb_last_connected) {
        esb_last_connected = connected;
        raise_zmk_split_peripheral_status_changed(
            (struct zmk_split_peripheral_status_changed){.connected = (bool)connected});
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(esb_link_status, esb_link_status_listener);
ZMK_SUBSCRIPTION(esb_link_status, zmk_position_state_changed);
#endif /* CONFIG_ZMK_SPLIT_ESB */
#endif /* !CONFIG_ZMK_SPLIT_BLE */

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

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) && !IS_ENABLED(CONFIG_ZMK_BLE)

// Prospector's output widget shows BLE HID profile status. This is a USB-only
// ESB dongle with BLE disabled, so those functions don't exist. Stub them: no
// BLE profile is ever connected or open (the widget falls back to USB state).
bool zmk_ble_profile_is_connected(uint8_t index) {
    return false;
}

bool zmk_ble_profile_is_open(uint8_t index) {
    return false;
}

#endif
