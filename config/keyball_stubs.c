#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>
#include <zmk/event_manager.h>
#include <zmk/events/split_peripheral_status_changed.h>
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
// zmk_split_peripheral_status_changed, so the nice_view connection widget would
// never refresh on its own. Poll the real link status (a lock-free snapshot
// read, cheap) and raise the event when it flips, so the icon updates.
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ESB)
static bool esb_last_connected = true;

static void esb_link_poll_work_cb(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(esb_link_poll_work, esb_link_poll_work_cb);

static void esb_link_poll_work_cb(struct k_work *work) {
    struct zmk_split_esb_status status;
    zmk_split_esb_get_status(&status);
    bool connected = !status.searching;
    if (connected != esb_last_connected) {
        esb_last_connected = connected;
        raise_zmk_split_peripheral_status_changed(
            (struct zmk_split_peripheral_status_changed){.connected = connected});
    }
    k_work_reschedule(&esb_link_poll_work, K_MSEC(1000));
}

static int esb_link_poll_init(void) {
    k_work_schedule(&esb_link_poll_work, K_SECONDS(2));
    return 0;
}

SYS_INIT(esb_link_poll_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
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
