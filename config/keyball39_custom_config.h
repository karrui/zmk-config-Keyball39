// Keyball39 Miryoku customization
// https://github.com/manna-harbour/miryoku_zmk
#include <dt-bindings/zmk/keyball39.h>
//
// keymap-drawer expands this local file but does not follow module-owned
// angle-bracket includes. Keep guarded fallbacks here so its parser sees the
// same numeric layer parameters. The firmware gets these from the shared header.
#ifndef U_BASE
#define U_BASE   0
#define U_EXTRA  1
#define U_TAP    2
#define U_BUTTON 3
#define U_NAV    4
#define U_MOUSE  5
#define U_MEDIA  6
#define U_NUM    7
#define U_SYM    8
#define U_FUN    9
#define U_TILE   10
#endif
//
// Base = Colemak-DH with Miryoku's GACS home-row mods (identical to the user's
// Charybdis Nano). NAV/NUM/SYM layer contents are transcribed from the
// Charybdis QMK keymap (charybdis_nano.layout.json). MEDIA adds explicit
// recovery, power, and Bluetooth controls; FUN keeps the Miryoku layout with a
// mirrored F-key cluster. BUTTON adds the trackball scroll chord.

#define MIRYOKU_ALPHAS_COLEMAKDH

// TILE: an 11th layer for Mudeer (KDE Plasma window tiling). Miryoku's layer
// list is overridable -- miryoku_babel/miryoku_layer_list.h is guarded by
// `#if !defined (MIRYOKU_LAYER_LIST)` -- so this replaces the stock list
// wholesale instead of stealing EXTRA or TAP. TILE is appended LAST: the
// dongle input listener uses these indices for scroll and snipe, so no existing
// index may shift. The shared constants live in dt-bindings/zmk/keyball39.h.
#define MIRYOKU_LAYER_LIST \
MIRYOKU_X(BASE,   "Base") \
MIRYOKU_X(EXTRA,  "Extra") \
MIRYOKU_X(TAP,    "Tap") \
MIRYOKU_X(BUTTON, "Button") \
MIRYOKU_X(NAV,    "Nav") \
MIRYOKU_X(MOUSE,  "Mouse") \
MIRYOKU_X(MEDIA,  "Media") \
MIRYOKU_X(NUM,    "Num") \
MIRYOKU_X(SYM,    "Sym") \
MIRYOKU_X(FUN,    "Fun") \
MIRYOKU_X(TILE,   "Tile")

#define MIRYOKU_LAYERMAPPING_TILE MIRYOKU_MAPPING

// Clipboard: use Ctrl-based binds (Ctrl+C/V/X/Z/Y) instead of the Miryoku
// COMMON default (Ctrl+Insert / Shift+Insert), which macOS ignores. Drives the
// BUTTON-layer U_CUT/U_CPY/U_PST/U_UND/U_RDO keys. Cross-OS given the user's
// Ctrl<->Cmd swap on macOS (physical Ctrl -> Cmd there).
#define MIRYOKU_CLIPBOARD_WIN

// Hold-tap tuning (Charybdis-style). Behaviors defined in keyball39.keymap:
//   u_mt_l / u_mt_r : per-hand home-row mods (balanced + 250ms + chordal hold)
//   u_lt_z          : Z pointer key, balanced @175ms
//   u_lt_sl         : / pointer key, balanced @175ms, unrestricted
#define U_MT_L(MOD, TAP)   &u_mt_l MOD TAP
#define U_MT_R(MOD, TAP)   &u_mt_r MOD TAP
#define U_LT_Z(LAYER, TAP) &u_lt_z LAYER TAP
#define U_LT_SL(LAYER, TAP) &u_lt_sl LAYER TAP

// Mudeer tile shortcut: Meta+Alt + key. Deliberately only TWO modifiers -- that
// leaves Shift and Ctrl free as the vertical dimension (see the TILE layer
// below). Base picked by auditing ~/.config/kglobalshortcutsrc across all 45
// combos: Meta+Alt collides once (plasmashell cycle-panels on Meta+Alt+P),
// against 3 for Meta+Ctrl (kwin Activate Window Demanding Attention on +A,
// plasmashell clipboard_action on +X, the Vicinae launcher on +V) and 5 for
// Mudeer's stock Meta. Meta+Shift is avoided throughout.
#define U_TIL(K) &kp LG(LA(K))

// Base override = Colemak-DH with two thumb tweaks (Charybdis-style):
//   * rightmost/inner left thumb (was Mouse/Tab) -> FUN, still taps Tab
//     (the Mouse layer is only movement keys, redundant with the trackball)
//   * right-outer thumb (was FUN/Del) -> TILE hold (see below). Delete moved to
//     the FUN layer's Bspc thumb; NAV still has it on this same key.
// Mouse buttons stay on Z and / (U_LT(U_BUTTON, ...)); everything else is the
// stock Colemak-DH base.
#define MIRYOKU_LAYER_BASE \
&kp Q,             &kp W,             &kp F,             &kp P,             &kp B,             &kp J,             &kp L,             &kp U,             &kp Y,             &kp SQT,           \
U_MT_L(LGUI, A),   U_MT_L(LALT, R),   U_MT_L(LCTRL, S),  U_MT_L(LSHFT, T),  &kp G,             &kp M,             U_MT_R(LSHFT, N),  U_MT_R(LCTRL, E),  U_MT_R(LALT, I),   U_MT_R(LGUI, O),   \
U_LT_Z(U_BUTTON, Z),U_MT(RALT, X),    &kp C,             &kp D,             &kp V,             &kp K,             &kp H,             &kp COMMA,         U_MT(RALT, DOT),   U_LT_SL(U_BUTTON, SLASH),\
U_NP,              U_NP,              U_LT(U_MEDIA, ESC),U_LT(U_NAV, SPACE),U_LT(U_FUN, TAB),  U_LT(U_SYM, RET),  U_LT(U_NUM, BSPC), &mo U_TILE,        U_NP,              U_NP

// NAV (hold Space) <- Charybdis L2: arrows / paging + left-hand mods
#define MIRYOKU_LAYER_NAV \
&kp LC(LS(BSLH))  , &kp LC(LS(LA(SLASH))), U_NP              , U_NP              , U_NP              , &kp PG_UP         , &kp HOME          , &kp UP            , &kp END           , &kp PG_DN, \
&kp LGUI          , &kp LALT          , &kp LCTRL         , &kp LSHFT         , U_NP              , &kp CAPS          , &kp LEFT          , &kp DOWN          , &kp RIGHT         , U_NP, \
U_NP              , U_NP              , U_NP              , U_NP              , U_NP              , &kp INS           , &kp HOME          , &kp PG_DN         , &kp PG_UP         , &kp END, \
U_NP,               U_NP,               U_NA,               U_NA,               U_NA,               &kp RET,            &kp BSPC,           &kp DEL,            U_NP,               U_NP

// NUM (hold Bspc) <- Charybdis L5: numbers + symbols, right-hand mods
#define MIRYOKU_LAYER_NUM \
&kp GRAVE         , &kp SEMI          , &kp MINUS         , &kp EQUAL         , &kp BSLH          , U_NP              , U_NP              , U_NP              , U_NP              , U_NP, \
&kp N1            , &kp N2            , &kp N3            , &kp N4            , &kp N5            , U_NP              , &kp LSHFT         , &kp LCTRL         , &kp LALT          , &kp LGUI, \
&kp N6            , &kp N7            , &kp N8            , &kp N9            , &kp N0            , U_NP              , U_NP              , U_NP              , U_NP              , U_NP, \
U_NP,               U_NP,               &kp LBKT,           &kp RBKT,           &kp DOT,            U_NA,               U_NA,               U_NA,               U_NP,               U_NP

// SYM (hold Enter) <- Charybdis L6: shifted number-row symbols + right arrows
#define MIRYOKU_LAYER_SYM \
&kp LS(GRAVE)     , &kp LS(SEMI)      , &kp LS(MINUS)     , &kp LS(EQUAL)     , &kp LS(BSLH)      , U_NP              , &kp HOME          , &kp UP            , &kp END           , U_NP, \
&kp LS(N1)        , &kp LS(N2)        , &kp LS(N3)        , &kp LS(N4)        , &kp LS(N5)        , U_NP              , &kp LEFT          , &kp DOWN          , &kp RIGHT         , U_NP, \
&kp LS(N6)        , &kp LS(N7)        , &kp LS(N8)        , &kp LS(N9)        , &kp LS(N0)        , U_NP              , U_NP              , U_NP              , U_NP              , U_NP, \
U_NP,               U_NP,               &kp LBRC,           &kp RBRC,           &kp GT,             U_NA,               U_NA,               U_NA,               U_NP,               U_NP

// FUN (hold Tab) = stock Miryoku FUN mirrored left<->right, so the F-keys sit
// on the RIGHT hand (FUN is held by the left thumb, freeing the right to press
// them). Just the default MIRYOKU_ALTERNATIVES_FUN flipped column-for-column,
// except the Bspc thumb, which is Delete now that the base layer's Del key
// became the TILE hold.
#define MIRYOKU_LAYER_FUN \
U_BOOT,            &u_to_U_TAP,       &u_to_U_EXTRA,     &u_to_U_BASE,      U_NA,              &kp PSCRN,         &kp F9,            &kp F8,            &kp F7,            &kp F12,           \
&kp LGUI,          &kp LALT,          &kp LCTRL,         &kp LSHFT,         U_NA,              &kp SLCK,          &kp F6,            &kp F5,            &kp F4,            &kp F11,           \
U_NA,              &kp RALT,          &u_to_U_MEDIA,     &u_to_U_FUN,       U_NA,              &kp PAUSE_BREAK,   &kp F3,            &kp F2,            &kp F1,            &kp F10,           \
U_NP,              U_NP,              U_NA,              U_NA,              U_NA,              &kp TAB,           &kp DEL,           &kp K_APP,         U_NP,              U_NP

// MEDIA (hold Esc) keeps the useful media/endpoint controls and replaces the
// board's inert RGB slots with guarded system controls. Bootloader and reset are
// mirrored because those behaviors act on the physical half that emitted them.
// Soft-off is a two-second hold (configured in keyball39.keymap) and available
// from either hand. Profile 4 fills the fifth ZMK Bluetooth profile.
#define MIRYOKU_LAYER_MEDIA \
&bootloader,        &sys_reset,        &u_to_U_EXTRA,     &u_to_U_BASE,      &soft_off,         &soft_off,         &bt BT_CLR_ALL,    &bt BT_CLR,        &sys_reset,        &bootloader,       \
&kp LGUI,           &kp LALT,          &kp LCTRL,         &kp LSHFT,         U_NA,              U_EP_TOG,          &kp C_PREV,        &kp C_VOL_DN,      &kp C_VOL_UP,      &kp C_NEXT,        \
U_NA,               &kp RALT,          &u_to_U_FUN,       &u_to_U_MEDIA,     &bt BT_SEL 4,      &u_out_tog,        &u_bt_sel_0,       &u_bt_sel_1,       &u_bt_sel_2,       &u_bt_sel_3,       \
U_NP,               U_NP,              U_NA,              U_NA,              U_NA,              &kp C_STOP,        &kp C_PP,          &kp C_MUTE,        U_NP,              U_NP

// MOUSE layer is transparent: it exists only as the trackball's scroll layer
// (its movement keys are useless with a trackball). Comma on BUTTON latches it
// via &tog U_MOUSE, so scrolling toggles on/off while typing still passes
// through; X/. still activate it momentarily for the Z+X / /+. scroll chord.
#define MIRYOKU_LAYER_MOUSE \
&trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,            \
&trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,            \
&trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,            \
&trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans,             &trans

// BUTTON (hold Z or /) = mouse buttons + clipboard. X and . -> &mo U_MOUSE for
// the Z+X (or /+.) momentary scroll chord; comma -> &tog U_MOUSE latches scroll
// on/off. The dongle listener runs its `scroll` child before `snipe`, so scroll
// wins while Z (BUTTON = snipe) is still held.
#define MIRYOKU_LAYER_BUTTON \
U_UND,              U_CUT,              U_CPY,              U_PST,              U_RDO,              U_RDO,              U_PST,              U_CPY,              U_CUT,              U_UND,             \
&kp LGUI,           &kp LALT,           &kp LCTRL,          &kp LSHFT,          U_NU,               U_NU,               &kp LSHFT,          &kp LCTRL,          &kp LALT,           &kp LGUI,          \
U_UND,              &mo U_MOUSE,        U_CPY,              U_PST,              U_RDO,              U_RDO,              U_PST,              &tog U_MOUSE,       &mo U_MOUSE,        U_UND,             \
U_NP,               U_NP,               U_BTN2,             U_BTN1,             U_BTN3,             U_BTN3,             U_BTN1,             U_BTN2,             U_NP,               U_NP

// TILE (hold either pinky-bottom key: left = position 30, right = position 38)
// = Mudeer window tiling. Held by one pinky, driven by the other hand, so the
// three alpha rows are TRANSLATED not mirrored -- the leftmost key of either
// half is the leftmost tile on screen, on both hands:
//
//   Q ¼far-L   W ¼mid-L   F ¼mid-R   P ¼far-R   B whole
//   A ⅔left    R ⅓left    S ⅓center  T ⅓right   G ⅔right
//   Z ¾left    X ½left    C ½center  D ½right   V ¾right
//
// Rows go finest (quarters) to widest (halves + three-quarters); each row is
// symmetric about its middle, and the outer key of a row is the "grown"
// version of the one beside it -- the spatial form of Mudeer's Shift-to-grow.
//
// Both halves emit the SAME keycode for a given tile (the left half's Colemak
// legends), so one KDE shortcut serves both hands and none of them land on
// punctuation. Vertical position rides on the two modifiers U_TIL deliberately
// left free, on the thumbs of both hands:
//   plain      -> full height   (Meta+Alt+key)
//   + Shift    -> top half      (Meta+Alt+Shift+key)
//   + Ctrl     -> bottom half   (Meta+Alt+Ctrl+key)
// The fixed Alt on position 31 is simply part of the prefix here, so pressing
// it alongside a tile is harmless.
// Finer vertical splits come from Mudeer's sequential tiling (enable it in the
// Kwin script options): a second tile press within 1s splits the first one
// vertically by the second tile's horizontal slot, so R then X = left third,
// top half; R then S = left third, middle vertical third.
#define MIRYOKU_LAYER_TILE \
U_TIL(Q),           U_TIL(W),           U_TIL(F),           U_TIL(P),           U_TIL(B),           U_TIL(Q),           U_TIL(W),           U_TIL(F),           U_TIL(P),           U_TIL(B),          \
U_TIL(A),           U_TIL(R),           U_TIL(S),           U_TIL(T),           U_TIL(G),           U_TIL(A),           U_TIL(R),           U_TIL(S),           U_TIL(T),           U_TIL(G),          \
U_TIL(Z),           U_TIL(X),           U_TIL(C),           U_TIL(D),           U_TIL(V),           U_TIL(Z),           U_TIL(X),           U_TIL(C),           U_TIL(D),           U_TIL(V),          \
U_NP,               U_NP,               U_NU,               &kp LSHFT,          &kp LCTRL,          &kp LSHFT,          &kp LCTRL,          U_NU,               U_NP,               U_NP
