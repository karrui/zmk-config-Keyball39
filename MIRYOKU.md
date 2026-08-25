# Miryoku on Keyball39

This configuration runs a customized
[Miryoku ZMK](https://github.com/manna-harbour/miryoku_zmk) layout on a
three-part Keyball39: two BLE peripheral halves and a USB dongle central.
External modules are fetched by west and pinned to known-good commits in
`config/west.yml`.

## Repository structure

- `boards/shields/keyball_nano/` — Keyball39 hardware, dongle, and per-part config
- `config/keyball39.keymap` — custom hold-tap behavior and soft-off guard
- `config/keyball39_custom_config.h` — layer contents and Miryoku overrides
- `config/mapping/39/keyball39.h` — 39-key physical mapping
- `include/dt-bindings/zmk/keyball39.h` — layer indices shared by keymap and pointer listener
- `build.yaml` — dongle, left, right, and settings-reset build matrix

The active keymap is `config/keyball39.keymap`; there is no alternate keymap to
rename and Miryoku is not a Git submodule.

## Physical mapping

Positions `0–29` are the 3×5 alpha blocks. The bottom row is asymmetric:

- `30` — fixed TILE hold
- `31` — fixed Alt
- `32` — fixed GUI
- `33–35` — three left Miryoku thumbs
- `36–38` — three right Miryoku thumbs; `38` also acts as the right TILE hold

## Layers

| Index | Layer | Activation | Purpose |
|---:|---|---|---|
| 0 | BASE | Default | Colemak-DH with GACS home-row mods |
| 1 | EXTRA | MEDIA selector | Miryoku Extra layer |
| 2 | TAP | Miryoku selector | Tap-only alphas |
| 3 | BUTTON | Hold Z or `/` | Mouse buttons, clipboard, and precision pointer mode |
| 4 | NAV | Hold Space | Navigation and editing |
| 5 | MOUSE | Z+X, `/`+`.` or BUTTON+comma toggle | Transparent layer used only for scroll mode |
| 6 | MEDIA | Hold Esc | Media, Bluetooth, power, and recovery |
| 7 | NUM | Hold Backspace | Numbers and symbols |
| 8 | SYM | Hold Enter | Shifted symbols and arrows |
| 9 | FUN | Hold Tab | Function keys and Delete |
| 10 | TILE | Hold either pinky-bottom key | Mudeer/KWin window tiling |

Named layer numbers live in `include/dt-bindings/zmk/keyball39.h` and are shared
by the keymap and dongle input listener. Their order must mirror
`MIRYOKU_LAYER_LIST`; append new layers after TILE or update both declarations
deliberately.

## Trackball behavior

The PMW3610 on the right half emits raw pointer events across the split. The
dongle is the only place that applies layer-dependent processing:

- BASE and most layers — normal cursor movement
- BUTTON (`3`) — snipe at 1:2 scaling
- MOUSE (`5`) — XY-to-wheel conversion at 1:3 scaling
- BUTTON + MOUSE — scroll wins because its listener child is evaluated first

Automouse is intentionally disabled. Moving or brushing the ball never changes
layers. Scroll is explicit: hold Z+X or `/`+`.`, or toggle it with comma while
BUTTON is held.

## MEDIA system controls

Hold Esc for MEDIA. The top row provides mirrored recovery controls so each
physical half can be targeted:

| Left top row | Right top row |
|---|---|
| Bootloader · Reset · Extra · Base · Soft-off | Soft-off · Clear all BT · Clear current BT · Reset · Bootloader |

Soft-off requires a two-second hold. It is configured on every split part, and
a hardware reset is the only wake source afterward. Bluetooth profiles 0–3 are
on the right lower row; profile 4 is on the left inner lower key. The normal
endpoint toggle, external-power toggle, and media controls remain available.

## TILE layer

[Mudeer](https://github.com/darkstego/Mudeer) is a KWin script. Hold either
pinky-bottom key and drive the layer with the other hand. Both halves emit the
same translated keycodes:

```text
Q ¼ far-left   W ¼ mid-left   F ¼ mid-right   P ¼ far-right   B whole
A ⅔ left       R ⅓ left       S ⅓ center      T ⅓ right       G ⅔ right
Z ¾ left       X ½ left       C ½ center      D ½ right       V ¾ right
```

The base chord is Meta+Alt. Add Shift for the top variant or Ctrl for the bottom
variant. Clear KDE's existing Meta+Alt+P `cycle-panels` shortcut. For finer
vertical splits, enable sequential tiling in Mudeer's KWin settings.

## Building and flashing

GitHub Actions uses `build.yaml` to produce:

- `keyball39_dongle`
- `keyball39_left`
- `keyball39_right`
- `nice_nano_settings_reset`
- `xiao_ble_settings_reset`

Firmware changes in this repository can affect central behavior, peripheral
power management, and the sensor pipeline, so flash the dongle and both halves
together. Settings-reset images are recovery tools, not part of a normal update.
