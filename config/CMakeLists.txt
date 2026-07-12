zephyr_library()
zephyr_library_sources(keyball_stubs.c)

# Prospector's battery_bar widget reads ZMK_SPLIT_BLE_PERIPHERAL_COUNT, a macro
# that zmk/ble.h only defines when CONFIG_ZMK_BLE=y. Under the ESB split
# transport BLE is disabled, so define it here from the peripheral count the
# ESB module still provides (CONFIG_ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS).
if(NOT CONFIG_ZMK_BLE AND CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  zephyr_compile_definitions(ZMK_SPLIT_BLE_PERIPHERAL_COUNT=${CONFIG_ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS})
endif()
