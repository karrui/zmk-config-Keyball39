// Keyball39 Miryoku customization
// https://github.com/manna-harbour/miryoku_zmk
//
// Base = Colemak-DH with Miryoku's GACS home-row mods (identical to the user's
// Charybdis Nano). NAV/NUM/SYM layer contents are transcribed from the
// Charybdis QMK keymap (charybdis_nano.layout.json). MEDIA and FUN keep Miryoku
// defaults (MEDIA adds Bluetooth control the wireless board needs; its RGB keys
// are inert no-ops on this board). BUTTON adds the trackball scroll chord.

#define MIRYOKU_ALPHAS_COLEMAKDH

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

// BUTTON (hold Z or /) = mouse buttons + clipboard. X and . are remapped to
// &mo U_MOUSE so that Z+X (or /+.) activates the scroll layer; the dongle
// input-listener runs its `scroll` child before `snipe`, so scroll wins while
// Z (BUTTON = snipe) is still held.
#define MIRYOKU_LAYER_BUTTON \
U_UND,              U_CUT,              U_CPY,              U_PST,              U_RDO,              U_RDO,              U_PST,              U_CPY,              U_CUT,              U_UND,             \
&kp LGUI,           &kp LALT,           &kp LCTRL,          &kp LSHFT,          U_NU,               U_NU,               &kp LSHFT,          &kp LCTRL,          &kp LALT,           &kp LGUI,          \
U_UND,              &mo U_MOUSE,        U_CPY,              U_PST,              U_RDO,              U_RDO,              U_PST,              U_CPY,              &mo U_MOUSE,        U_UND,             \
U_NP,               U_NP,               U_BTN3,             U_BTN1,             U_BTN2,             U_BTN2,             U_BTN1,             U_BTN3,             U_NP,               U_NP
