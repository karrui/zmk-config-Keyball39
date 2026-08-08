# Miryoku on Keyball39

This repo includes [Miryoku ZMK](https://github.com/manna-harbour/miryoku_zmk) as a submodule for use with the Keyball39.

## Structure

- `miryoku/` — Miryoku ZMK submodule (the layout engine)
- `config/mapping/39/keyball39.h` — Keyball39 physical layout mapping
- `config/keyball39_miryoku.keymap` — Miryoku keymap (3-line include file)
- `config/keyball39.keymap` — Original custom keymap (kept as backup)

## Key Mapping

The Keyball39 has 39 keys: 30 alphas (3×5 per hand) + 9 bottom-row keys.

The mapping assigns:
- **K00–K29**: Alpha block (3×5 per hand)
- **K32/K34/K33**: Left thumb keys (tertiary, primary, secondary)
  - K33 = left trackball button
- **K36/K35**: Right thumb keys (secondary, primary)
- **K37**: Right trackball button (tertiary right)
- **K38**: Right edge key
- **RC(3,1)**: Unused (`&none`)

## Trackball Layers

The pmw3610 trackball module (right hand) has hardcoded layer activation:

| Setting | Layer | Miryoku Layer | Behavior |
|---------|-------|---------------|----------|
| `automouse-layer` | 4 | **NAV** | Trackball moves cursor (temp layer, 700ms timeout) |
| `scroll-layers` | 5 | **MOUSE** | Trackball scrolls (XY → scroll mapper) |
| `snipe-layers` | 6 | **MEDIA** | Trackball moves cursor with 1:3 scaling (precision sniping) |

The dongle's `trackball_listener` processes the incoming trackball data and applies the appropriate transformation based on which layer is active.

### Sniping

Sniping is active on layer 6 (**MEDIA**). Activate it by holding the MEDIA layer — typically via `U_LT(U_MEDIA, ESC)` on the leftmost thumb key (tap = ESC, hold = MEDIA/snipe). While snipe mode is active, the trackball moves the cursor at a 1:3 scale ratio for precision targeting.

## Building

### Option A: Use Miryoku as the default keymap

Rename the keymap files:
```bash
cd config
mv keyball39.keymap keyball39_original.keymap
mv keyball39_miryoku.keymap keyball39.keymap
```

Then build normally:
```bash
west build -b nice_nano_v2 -s config -- -DSHIELD=keyball39_left nice_view_adapter nice_view
west build -b nice_nano_v2 -s config -- -DSHIELD=keyball39_right nice_view_adapter nice_view
west build -b seeeduino_xiao_ble -s config -- -DSHIELD=keyball39_dongle prospector_adapter
```

Or via the GitHub Actions workflow (build.yaml).

### Option B: Keep both keymaps

Build the Miryoku keymap by specifying it explicitly:
```bash
west build -b nice_nano_v2 -s config \
  -- -DSHIELD=keyball39_left_nice_view_adapter_nice_view \
  -DCONF_FILE=$(pwd)/config/keyball39_miryoku.keymap \
  -DDTS_FILE=$(pwd)/config/keyball39_miryoku.keymap
```

### Option C: Use Miryoku's GitHub workflow

Fork [miryoku_zmk](https://github.com/manna-harbour/miryoku_zmk) and use their Build Inputs workflow. You'll need to:
1. Specify board = `nice_nano_v2` and shield = `keyball39_left,keyball39_right`
2. Point to this repo as an out-of-tree shield source
3. This requires creating an [outboard file](https://github.com/manna-harbour/miryoku_zmk/tree/main/.github/workflows/outboards)

## Customization

Miryoku supports many layout options via `custom_config.h`. Edit `miryoku/miryoku/custom_config.h` or use defines:

```c
#define MIRYOKU_ALPHAS_QWERTY       // QWERTY alpha layer
#define MIRYOKU_TAP_QWERTY          // QWERTY tap layer
#define MIRYOKU_NAV_INVERTEDT       // Inverted T nav cluster
#define MIRYOKU_CLIPBOARD_WIN       // Windows clipboard (Ctrl-C/V/X)
#define MIRYOKU_LAYERS_FLIP         // Swap left/right layers
```

See [Miryoku docs](https://github.com/manna-harbour/miryoku/tree/master/docs/reference) for all options.

## Layers

| Layer | Index | Activated By | Contents |
|-------|-------|-------------|----------|
| BASE | 0 | Default | Colemak-DH alphas |
| EXTRA | 1 | Toggle from other layers | QWERTY alphas |
| TAP | 2 | Tap layer of mod-morph keys | Colemak-DH alphas (tap-only) |
| BUTTON | 3 | Z / / (bottom row, Colemak-DH) | Mouse buttons, clipboard |
| NAV | 4 | SPACE thumb hold | Nav arrows, trackball automouse |
| MOUSE | 5 | Not thumb-bound here (BUTTON chords) | Transparent — exists as the trackball scroll layer |
| MEDIA | 6 | ESC thumb hold (tap=ESC) | Media controls, RGB, BT — **trackball snipe** |
| NUM | 7 | BSPC thumb hold (tap=BSPC) | Numbers |
| SYM | 8 | RET thumb hold (tap=RET) | Symbols |
| FUN | 9 | TAB thumb hold (tap=TAB) | Function keys, Delete on the Bspc thumb |
| TILE | 10 | Either pinky-bottom key (pos 30 / pos 38) | Mudeer window tiling — see below |

Layer 10 is an addition to Miryoku's stock list, appended last so the trackball's hardcoded layer numbers (4/5/6) don't move. `MIRYOKU_LAYER_LIST` and the `U_*` layer indices are overridden in `config/keyball39_custom_config.h`.

## TILE layer — Mudeer window tiling

[Mudeer](https://github.com/darkstego/Mudeer) is a KWin script; every tile it
offers is an independently registered KDE shortcut, so all of these are rebound
under **System Settings → Shortcuts → search "mudeer"**.

Hold either pinky-bottom key (left position 30, right position 38) and drive the
tiling with the other hand. The three alpha rows are *translated*, not mirrored —
the leftmost key of either half is the leftmost tile on screen:

```
 Q ¼ far-left   W ¼ mid-left   F ¼ mid-right   P ¼ far-right   B whole
 A ⅔ left       R ⅓ left       S ⅓ center      T ⅓ right       G ⅔ right
 Z ¾ left       X ½ left       C ½ center      D ½ right       V ¾ right
```

Rows run finest (quarters) to widest (halves and three-quarters); each row is
symmetric about its middle, and a row's outer key is the "grown" version of its
neighbour. Both halves send the same keycode for a given tile, so one KDE
shortcut serves both hands.

Modifiers carry the vertical dimension (`U_TIL` uses only Meta+Ctrl so Shift and
Alt stay free). Shift and Alt sit on the thumbs of both hands on this layer, and
the fixed Alt on position 31 works as the left hand's bottom-half key:

| Held | Emits | Mudeer action |
|------|-------|---------------|
| — | `Meta+Ctrl+<key>` | full height |
| Shift | `Meta+Ctrl+Shift+<key>` | *Top* variant |
| Alt | `Meta+Ctrl+Alt+<key>` | *Bottom* variant |

Meta+Ctrl was chosen to stay clear of the Meta+Shift combos already used by
Spectacle and the Claude Desktop quick-invoke.

For splits finer than a half, enable **sequential tiling** in the Kwin script
options (System Settings → Kwin Scripts → Mudeer). A second tile press within 1s
splits the first tile vertically using the second tile's horizontal slot, so
`R` then `X` gives the left third, top half; `R` then `S` gives the left third,
middle vertical third.
