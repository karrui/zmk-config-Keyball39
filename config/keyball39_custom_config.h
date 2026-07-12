// Keyball39 Miryoku customization
// https://github.com/manna-harbour/miryoku_zmk
//
// Base = Colemak-DH with Miryoku's GACS home-row mods (identical to the user's
// Charybdis Nano). NAV/NUM/SYM layer contents are transcribed from the
// Charybdis QMK keymap (charybdis_nano.layout.json). MEDIA and FUN keep Miryoku
// defaults (MEDIA adds Bluetooth control the wireless board needs; its RGB keys
// are inert no-ops on this board). BUTTON adds the trackball scroll chord.

#define MIRYOKU_ALPHAS_COLEMAKDH

// Base override = Colemak-DH with two thumb tweaks (Charybdis-style):
//   * rightmost/inner left thumb (was Mouse/Tab) -> FUN, still taps Tab
//     (the Mouse layer is only movement keys, redundant with the trackball)
//   * right-outer thumb (was FUN/Del) -> plain Delete
// Mouse buttons stay on Z and / (U_LT(U_BUTTON, ...)); everything else is the
// stock Colemak-DH base.
#define MIRYOKU_LAYER_BASE \
&kp Q,             &kp W,             &kp F,             &kp P,             &kp B,             &kp J,             &kp L,             &kp U,             &kp Y,             &kp SQT,           \
U_MT(LGUI, A),     U_MT(LALT, R),     U_MT(LCTRL, S),    U_MT(LSHFT, T),    &kp G,             &kp M,             U_MT(LSHFT, N),    U_MT(LCTRL, E),    U_MT(LALT, I),     U_MT(LGUI, O),     \
U_LT(U_BUTTON, Z), U_MT(RALT, X),     &kp C,             &kp D,             &kp V,             &kp K,             &kp H,             &kp COMMA,         U_MT(RALT, DOT),   U_LT(U_BUTTON, SLASH),\
U_NP,              U_NP,              U_LT(U_MEDIA, ESC),U_LT(U_NAV, SPACE),U_LT(U_FUN, TAB),  U_LT(U_SYM, RET),  U_LT(U_NUM, BSPC), &kp DEL,           U_NP,              U_NP

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
U_NP,               U_NP,               &kp DOT,            &kp N0,             &kp MINUS,          U_NA,               U_NA,               U_NA,               U_NP,               U_NP

// SYM (hold Enter) <- Charybdis L6: shifted number-row symbols + right arrows
#define MIRYOKU_LAYER_SYM \
&kp LS(GRAVE)     , &kp LS(SEMI)      , &kp LS(MINUS)     , &kp LS(EQUAL)     , &kp LS(BSLH)      , U_NP              , &kp HOME          , &kp UP            , &kp END           , U_NP, \
&kp LS(N1)        , &kp LS(N2)        , &kp LS(N3)        , &kp LS(N4)        , &kp LS(N5)        , U_NP              , &kp LEFT          , &kp DOWN          , &kp RIGHT         , U_NP, \
&kp LS(N6)        , &kp LS(N7)        , &kp LS(N8)        , &kp LS(N9)        , &kp LS(N0)        , U_NP              , U_NP              , U_NP              , U_NP              , U_NP, \
U_NP,               U_NP,               &kp LPAR,           &kp RPAR,           &kp UNDER,          U_NA,               U_NA,               U_NA,               U_NP,               U_NP

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
U_NP,               U_NP,               U_BTN3,             U_BTN1,             U_BTN2,             U_BTN2,             U_BTN1,             U_BTN3,             U_NP,               U_NP
