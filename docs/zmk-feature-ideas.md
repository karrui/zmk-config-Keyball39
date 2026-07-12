# ZMK Feature Ideas for Keyball39 Miryoku

A researched catalogue of ZMK behaviors and input processors that could extend
this config, with a per-feature verdict tailored to what is **already** set up
here (see `MIRYOKU.md` and `config/keyball39.keymap`).

## Version basis (read first)

This repo pins **ZMK `v0.3`** (`config/west.yml`, `revision: v0.3`) plus three
modules: `zmk-pmw3610-driver` (trackball), `prospector-zmk-module` (dongle
display), `miryoku_zmk` (layout engine).

Every behavior and input processor below was verified to exist **in the `v0.3`
tag**, not just mainline, by checking the tag's source tree:

- Behaviors present at `v0.3`: tap-dance, caps-word, mod-morph, sticky-key,
  key-repeat, macro (+ one/two-param), bluetooth, outputs, toggle-layer,
  to-layer, momentary-layer, key-toggle. Verified in
  [`app/dts/bindings/behaviors` @ v0.3](https://github.com/zmkfirmware/zmk/tree/v0.3/app/dts/bindings/behaviors).
- Input processors present at `v0.3`: scaler, transform, temp-layer,
  code-mapper, behaviors. Verified in
  [`app/dts/bindings/input_processors` @ v0.3](https://github.com/zmkfirmware/zmk/tree/v0.3/app/dts/bindings/input_processors)
  and [`app/dts/input/processors` @ v0.3](https://github.com/zmkfirmware/zmk/tree/v0.3/app/dts/input/processors).
- **Conditional layers** (tri-layer) is confirmed present at `v0.3`
  ([`zmk,conditional-layers.yaml` @ v0.3](https://github.com/zmkfirmware/zmk/blob/v0.3/app/dts/bindings/zmk%2Cconditional-layers.yaml)).
- **Auto-shift** is **not** a mainline behavior in any ZMK version — it must be
  hand-rolled from hold-tap or parameterised macros, or pulled from a community
  module. See its section for the caveat.

The input-processor subsystem is fully present in `v0.3` — this repo already
uses `zip_xy_to_scroll_mapper`, `zip_scroll_scaler`, and `zip_xy_scaler` in
`config/boards/shields/keyball_nano/keyball39_dongle.overlay`, and has
`zip_temp_layer` present-but-commented. Nothing in the "input processors"
section below requires a newer ZMK than what is pinned.

---

## Summary — highest-value picks

| Feature | ZMK name | Fits a trackball 36%? | Verdict for THIS config |
|---|---|---|---|
| Combos | `zmk,combos` | Frees keys on a 36-key board (esc/tab/parens/caps) | **Add** — best single win; gate with `require-prior-idle-ms` so home-row-mod rolls don't misfire |
| Caps-word | `&caps_word` | Replaces held-shift on a keyboard with mod-taps | **Add** — bind via a combo; complements the home-row mods |
| Mod-morph | `&mod_morph` | Packs 2 symbols per key; shift+bspc→del | **Add selectively** — comma/dot → `;`/`:`, `!`/`?`; a bspc/del morph |
| Conditional layers | `zmk,conditional-layers` | Auto ADJUST for BT/reset/RGB | **Optional** — Miryoku already thumb-holds every layer; use only for a combined ADJUST |
| Scroll direction transform | `&zip_scroll_transform` | Natural-vs-reverse scroll on the ball | **Try** — one-line fix if ball scroll feels backwards |
| Auto-mouse-layer | `&zip_temp_layer` | Motion auto-activates a layer | **Leave off (as you have it)** — but `require-prior-idle-ms` + `excluded-positions` can rescue it if you ever want it back |
| Sticky keys | `&sk` / `&sl` | One-shot mods/layers | **Skip mostly** — home-row mods already cover this; a sticky-shift combo is the only real gap and caps-word beats it |
| Tap-dance | `&tap_dance` | More actions per key | **Use sparingly** — adds tap latency, fights your `quick-tap-ms=200` repeat feel |
| Key-repeat | `&key_repeat` | Cheap "repeat last key" | **Optional** — one key on NUM/SYM |
| Macros | `&macro` | Trackball drag / select-all | **Niche** — a click-drag macro is the interesting one |
| Auto-shift | (not mainline) | — | **Skip** — conflicts with home-row mods |
| Bluetooth mgmt | `&bt` / `&out` | Profile switching | **Already covered** on MEDIA layer; sanity-check `BT_CLR`/`BT_SEL` are present |

---

# Input processors (highest-value area)

Input processors sit on an `zmk,input-listener` and transform pointer events
before they reach the host. On this build the relevant listener is
`trackball_listener` on the **dongle** (central), with per-child `layers` and
`input-processors` lists tried in order.
Source: [Input Processors overview](https://zmk.dev/docs/keymaps/input-processors),
[Input Processor usage](https://zmk.dev/docs/keymaps/input-processors/usage).

Predefined instances available via `#include <input/processors.dtsi>`:
`&zip_xy_scaler`, `&zip_x_scaler`, `&zip_y_scaler`, `&zip_scroll_scaler`,
`&zip_xy_transform`, `&zip_scroll_transform`, `&zip_xy_to_scroll_mapper`,
`&zip_xy_swap_mapper`, `&zip_temp_layer`, `&zip_button_behaviors`.
Source: [Input Processors overview](https://zmk.dev/docs/keymaps/input-processors).

## Auto-mouse-layer (`zip_temp_layer`)

**What it is.** An input processor that *enables a layer while pointer events
arrive and auto-disables it after a timeout of no motion* — the ZMK equivalent
of QMK "auto mouse layer." Two required params: `<layer> <timeout-ms>`. Optional
`require-prior-idle-ms` (don't activate if a key was pressed within N ms) and
`excluded-positions` (key positions that do not keep the layer alive).
Source: [Temporary Layer Input Processor](https://zmk.dev/docs/keymaps/input-processors/temp-layer).

```dts
// on the listener child:
input-processors = <&zip_temp_layer 4 700>;   // layer 4, 700ms idle timeout
// or a custom instance that ignores stray brushes while typing:
zip_temp_layer_gated: zip_temp_layer_gated {
    compatible = "zmk,input-processor-temp-layer";
    #input-processor-cells = <2>;
    require-prior-idle-ms = <150>;
    excluded-positions = <...home-row positions...>;
};
```

**Verdict: leave OFF (you already disabled it), but keep this in your back
pocket.** Your overlay comment says you turned it off because brushing the ball
mid-type flipped to NAV. That exact problem is what `require-prior-idle-ms` and
`excluded-positions` were added to solve. If you ever want motion→NAV back, a
gated instance (idle ~120-150ms, home-row keys excluded) is the sanctioned fix
rather than the raw `<&zip_temp_layer 4 700>` you commented out.
Source: [temp-layer](https://zmk.dev/docs/keymaps/input-processors/temp-layer).

## Input scaling / "acceleration" (`zip_xy_scaler`, `zip_scroll_scaler`)

**What it is.** `type=<INPUT_EV_REL>` scalers multiply then divide event values:
`&zip_xy_scaler <mult> <div>`. Params are positive ints, max 16. You already run
`&zip_xy_scaler 1 2` for snipe and `&zip_scroll_scaler 1 3` for slow scroll.
Source: [Scaler Input Processor](https://zmk.dev/docs/keymaps/input-processors/scaler).

**Caveat — no true acceleration in core.** The scaler is *linear*; ZMK core has
no velocity-curve pointer acceleration behavior. Non-linear accel would need a
custom/module processor. For base cursor speed, prefer the driver's CPI
(`CONFIG_PMW3610_CPI`, `CONFIG_PMW3610_SNIPE_CPI` — you already set these) over
stacking scalers.

**Verdict: no change needed.** You already scale both cursor (snipe) and scroll,
and tune CPI at the driver. A base-layer `&zip_xy_scaler` is only worth it if the
raw ball feels too fast even at your default CPI — tune CPI first.

## Axis transform / invert / swap (`zip_xy_transform`)

**What it is.** `&zip_xy_transform <flags>` inverts or swaps axes. Flags:
`INPUT_TRANSFORM_X_INVERT`, `INPUT_TRANSFORM_Y_INVERT` (multiply that axis by
-1), `INPUT_TRANSFORM_XY_SWAP` (retype X↔Y). Combine with `|`.
Source: [Transformer Input Processor](https://zmk.dev/docs/keymaps/input-processors/transformer).

```dts
input-processors = <&zip_xy_transform (INPUT_TRANSFORM_X_INVERT | INPUT_TRANSFORM_Y_INVERT)>;
```

**Verdict: keep in back pocket only.** Your ball orientation is already correct
(cursor tracks right). Only reach for this if a physical remount or firmware
change makes an axis feel reversed/rotated.

## Scroll direction transform (`zip_scroll_transform`)

**What it is.** Same transform family, applied to wheel events — the clean way to
flip scroll direction (natural vs. reverse) without touching the OS.
Source: [Transformer Input Processor](https://zmk.dev/docs/keymaps/input-processors/transformer),
[Input Processors overview](https://zmk.dev/docs/keymaps/input-processors).

```dts
// in the `scroll { ... }` child, after the scroll mapper:
input-processors =
    <&zip_xy_to_scroll_mapper>,
    <&zip_scroll_transform (INPUT_TRANSFORM_Y_INVERT)>,  // flip vertical scroll
    <&zip_scroll_scaler 1 3>;
```

**Verdict: try it if ball-scroll direction annoys you.** Cheap, one line, keeps
the OS on default scroll settings across machines — nice for a wireless board you
move between hosts. Purely a preference toggle.

## Drag-scroll: input-processor vs. your layer approach

**What it is.** "Drag scroll" = ball motion becomes wheel scroll. Two idioms:
(a) a layer whose listener child maps XY→scroll (what you do — the `scroll { }`
child on `layers = <5>`, entered via the Z+X / `&tog U_MOUSE` chord), or
(b) a `zip_temp_layer` that auto-enters a scroll layer on motion.
Source: [Input Processors overview](https://zmk.dev/docs/keymaps/input-processors),
[temp-layer](https://zmk.dev/docs/keymaps/input-processors/temp-layer).

**Verdict: your layer approach is the right one — keep it.** The momentary Z+X
chord plus `&tog` latch gives explicit, misfire-proof control that the
motion-triggered variant can't match on a board where you rest a thumb near the
ball. No change recommended.

## Pointer-button → behavior (`zip_button_behaviors`)

**What it is.** Maps physical trackball button *clicks* (left/middle/right event
codes) directly to ZMK behaviors, independent of any layer. `bindings` length
must match `codes`.
Source: [Behaviors Input Processor](https://zmk.dev/docs/keymaps/input-processors/behaviors).

**Verdict: niche/skip.** Only relevant if you had a physical switch under the
ball you wanted to remap globally. Your mouse buttons live on the BUTTON layer
via `&mkp`, which is more flexible. Noted for completeness.

---

# Keymap behaviors

## Combos (`zmk,combos`) — top recommendation

**What it is.** Press N key positions within `timeout-ms` to fire one binding.
Properties: `key-positions` (required), `bindings` (required), `timeout-ms`
(required), `layers` (restrict to layers; omit = global), `require-prior-idle-ms`
(suppress if a non-modifier was pressed within N ms — the misfire guard),
`slow-release` (release on last key up instead of first).
Source: [Combos](https://zmk.dev/docs/keymaps/combos).

```dts
/ {
    combos {
        compatible = "zmk,combos";
        combo_esc {           // Colemak-DH W+F -> Esc  (positions 1 2)
            key-positions = <1 2>;
            bindings = <&kp ESC>;
            timeout-ms = <40>;
            require-prior-idle-ms = <100>;
        };
        combo_tab {           // F+P -> Tab (positions 2 3)
            key-positions = <2 3>;
            bindings = <&kp TAB>;
            timeout-ms = <40>;
            require-prior-idle-ms = <100>;
        };
        combo_caps { key-positions = <3 6>; bindings = <&caps_word>; timeout-ms = <50>; };
    };
};
```

**Why it suits this board.** 36 keys means every symbol/nav key competes for a
thumb hold. Combos add keycodes (esc, tab, `(`/`)`, caps-word, del) without new
keys and without stealing a base-layer slot — the classic 36% move.

**Verdict: ADD — the highest-value change here.** One caveat specific to your
setup: you run balanced home-row mods with same-hand chordal-hold, so
fast rolls across adjacent home-row keys are common. Put `require-prior-idle-ms`
(~50-100ms) on every combo, keep `key-positions` on non-adjacent or upper-row
keys where you can, and keep `timeout-ms` tight (~30-45ms). Position numbers use
the matrix-transform order noted in your keymap header
(left `0-4 10-14 20-24 30-35`, right `5-9 15-19 25-29 36-38`).

## Caps-word (`&caps_word`)

**What it is.** Context-aware caps lock: shifts alphas until a "break" key is
pressed. `continue-list` sets keys that keep it alive (default: alphanumerics,
`_`, backspace, delete); `mods` sets which modifier(s) to apply (default
`MOD_LSFT`). No timeout property.
Source: [Caps Word](https://zmk.dev/docs/keymaps/behaviors/caps-word).

```dts
&caps_word { continue-list = <UNDERSCORE MINUS BACKSPACE DELETE>; };
// bind on a combo or a spare layer key:  &caps_word
```

**Why it suits this board.** On a mod-tap layout, holding shift for `MY_CONST`
means fighting the home-row shift timing. Caps-word taps once and types the whole
identifier — great for `SCREAMING_SNAKE_CASE`.

**Verdict: ADD (via a combo).** It directly complements your home-row mods and
costs no key if bound to a combo. Miryoku's base doesn't give you one by default.

## Mod-morph (`&mod_morph`)

**What it is.** Tap = first binding; tap while a listed modifier is held = second
binding. `bindings` = two behaviors; `mods` = triggering modifier mask;
`keep-mods` = which held mods still pass through to the morphed keycode.
Source: [Mod-Morph](https://zmk.dev/docs/keymaps/behaviors/mod-morph).

```dts
bspc_del: bspc_del {
    compatible = "zmk,behavior-mod-morph";
    #binding-cells = <0>;
    bindings = <&kp BACKSPACE>, <&kp DELETE>;
    mods = <(MOD_LSFT|MOD_RSFT)>;
};
comma_semi: comma_semi {          // , -> ;   shift+, -> :
    compatible = "zmk,behavior-mod-morph";
    #binding-cells = <0>;
    bindings = <&kp COMMA>, <&kp SEMI>;
    mods = <(MOD_LSFT|MOD_RSFT)>;
};
```

**Why it suits this board.** Doubles the payload of punctuation keys. You
currently spend NUM/SYM slots on `;`, `:`, `-`, `_`; morphs on the base
comma/dot keys can surface the common ones without a layer hop.
**Caveat:** only *one* of the listed mods needs to be held to morph (you can't
require a specific 2-mod combo); nest morphs for more states.

**Verdict: ADD selectively.** A `,`→`;`(`:`) and `.`→(`?`) morph is a clean win.
Skip a shift+bspc→del morph *only if* it clashes with your existing separate DEL
thumb — you already have both BSPC (NUM hold) and a dedicated DEL, so the del
morph is redundant here; the punctuation morphs are the value.

## Conditional layers / tri-layer (`zmk,conditional-layers`)

**What it is.** When *all* layers in `if-layers` are active, `then-layer`
activates — generalised tri-layer (lower+raise→adjust).
Source: [Conditional Layers](https://zmk.dev/docs/keymaps/conditional-layers).

```dts
/ {
    conditional_layers {
        compatible = "zmk,conditional-layers";
        tri {
            if-layers = <4 7>;    // NAV + NUM held together
            then-layer = <10>;    // an ADJUST layer (BT/reset/RGB/bootloader)
        };
    };
};
```

**Why it suits this board.** A single ADJUST reachable by holding two thumbs is a
tidy home for `&bt BT_SEL`, `&sys_reset`, `&bootloader`, output toggles — keys
you rarely want but shouldn't have to dig for.

**Verdict: OPTIONAL.** Miryoku's whole premise is that *every* layer already has
its own thumb-hold, so you don't have the lower/raise→adjust gap tri-layer was
invented for. Worth it only if you want a dedicated ADJUST/system layer without
spending another thumb chord. Note the doc's gotcha: a `then-layer` is *fully*
owned by the conditional system — momentary-activating it directly won't stick.

## Sticky keys (`&sk`) / sticky layer (`&sl`)

**What it is.** `&sk <mod>` holds a modifier until the next key
(`release-after-ms` default 1000ms). `quick-release` deactivates on next key
*press* (stops double-shift). `lazy` sends the mod just before the next key
(avoids stray `LGUI` menu pops). `ignore-modifiers` (default on) lets you stack
sticky mods. `&sl <layer>` is the one-shot-layer analogue.
Source: [Sticky Key](https://zmk.dev/docs/keymaps/behaviors/sticky-key).

**Verdict: MOSTLY SKIP.** Your per-hand home-row mods already give one-handed
GACS access, so sticky mods are largely redundant. The one genuine gap —
one-shot shift for a single capital — is better served by caps-word (above) or a
`&sk LSHFT` on a *combo* if you want it. `&sl` is niche given every Miryoku layer
is a thumb-hold already.

## Tap-dance (`&tap_dance`)

**What it is.** Different behavior per tap-count on one key; `tapping-term-ms`
window, `bindings` list indexed by count. Confirmed a **mainline** behavior at
`v0.3` (`zmk,behavior-tap-dance.yaml` is in the tag), *not* a module.
Source: [Tap-Dance](https://zmk.dev/docs/keymaps/behaviors/tap-dance),
[binding @ v0.3](https://github.com/zmkfirmware/zmk/blob/v0.3/app/dts/bindings/behaviors/zmk%2Cbehavior-tap-dance.yaml).

**Verdict: USE SPARINGLY.** Tap-dance forces the firmware to wait out
`tapping-term-ms` to disambiguate tap-count, which fights the snappy,
repeat-on-double-tap feel you engineered with `quick-tap-ms=200` on every
hold-tap. A hold-tap or a combo usually gets you the same second action without
the added latency. Fine for a rarely-typed key (e.g. a media key), not for
letters or thumbs.

## Key-repeat (`&key_repeat`)

**What it is.** Re-sends the last keycode. Tracks only the HID Key usage page by
default; extend `usage-pages` to include Consumer to repeat media keys.
Source: [Key Repeat](https://zmk.dev/docs/keymaps/behaviors/key-repeat).

**Verdict: OPTIONAL.** Cheap convenience on a spare NUM/SYM slot; not a priority.

## Macros (`&macro`)

**What it is.** Runs a behavior sequence on press/release. `bindings` list,
`wait-ms`/`tap-ms` timing, control tokens `&macro_tap`/`&macro_press`/
`&macro_release`/`&macro_pause_for_release`/`&macro_wait_time`. Parameterised via
`zmk,behavior-macro-one-param|two-param` + `&macro_param_1to1` etc.
Source: [Macros](https://zmk.dev/docs/keymaps/behaviors/macros).

```dts
// Trackball click-drag: press LCLK, hold it across ball motion, drop on next tap
drag: drag {
    compatible = "zmk,behavior-macro";
    #binding-cells = <0>;
    bindings = <&macro_press &mkp LCLK>, <&macro_pause_for_release>, <&macro_release &mkp LCLK>;
};
```

**Why it suits this board.** The trackball-relevant use is a **click-drag** hold
(select text / drag windows without pinning a physical button) and one-shot
combos like select-all-then-copy.

**Verdict: NICHE.** The click-drag macro is the one genuinely useful trackball
macro; add it to the BUTTON layer if you do a lot of dragging. Otherwise low
priority.

## Auto-shift

**What it is.** Tap = lowercase, hold = shifted — automatically for all keys.
**Not a mainline ZMK behavior in any version** (confirmed: no auto-shift binding
in the tree). It's hand-built from a hold-tap (`&kp X` tap / `&kp LS(X)` hold) or
parameterised macros, per community write-ups.
Source: [ZMK issue #2276 (no built-in auto-shift)](https://github.com/zmkfirmware/zmk/issues/2276),
[community hold-tap approach](https://github.com/nickcoutsos/keymap-editor/wiki/Autoshift-using-ZMK-behaviors).

**Verdict: SKIP.** A hold-tap-per-key auto-shift would collide head-on with your
home-row mods (both want the hold action) and blow up your tapping-term tuning.
Not worth it on this layout.

## Bluetooth / endpoints (`&bt`, `&out`)

**What it is.** `&bt BT_SEL <n>`, `BT_NXT`, `BT_PRV`, `BT_CLR` (clear current
bond), `BT_CLR_ALL`, `BT_DISC <n>`. `&out OUT_USB`/`OUT_BLE`/`OUT_TOG` picks the
endpoint.
Source: [Bluetooth behavior](https://zmk.dev/docs/keymaps/behaviors/bluetooth).

**Verdict: ALREADY COVERED — just verify.** Miryoku's MEDIA layer carries the BT
keys and your `MIRYOKU.md` notes MEDIA "adds Bluetooth control the wireless board
needs." **Dongle caveat:** in a dongle/central-listener setup, profile pairing
lives on the **dongle**, so the `&bt` keys must be reachable from the dongle's
host connection — confirm `BT_SEL 0..n` and `BT_CLR` are actually present on
MEDIA. `&out` is largely moot here (the dongle owns the USB/BLE endpoint).

---

# Display widgets (brief)

This build already loads `prospector-zmk-module` on the dongle
(`prospector_adapter`) and `nice_view` on the halves. The Prospector module
provides a large dongle-side status display (layers, battery, connection); nice!
view gives the per-half OLED. There is no *trackball-specific* widget in core ZMK
worth wiring up here — pointer state (scroll/snipe layer) is already legible via
the active-layer indicator since your trackball modes are layers.
Source: [prospector-zmk-module](https://github.com/carrefinho/prospector-zmk-module),
[ZMK displays](https://zmk.dev/docs/features/displays).

**Verdict: no action** — your display stack already surfaces the layer state that
doubles as trackball-mode state.

---

## Suggested adoption order

1. **Combos** (esc, tab, caps-word, parens) with `require-prior-idle-ms` — biggest win.
2. **Caps-word** bound to a combo.
3. **Mod-morph** on `,`/`.` for `;`/`:` and `?`.
4. **`zip_scroll_transform`** if ball-scroll direction bugs you.
5. Everything else: as-needed, per the verdicts above.
