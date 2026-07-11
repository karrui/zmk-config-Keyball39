// Copyright 2024 Manna Harbour
// https://github.com/manna-harbour/miryoku

#if !defined (MIRYOKU_LAYOUTMAPPING_KEYBALL39)

#define XXX &none

// Parameter list is Miryoku's fixed canonical order (40 slots, Nxx = absent).
// Body is the Keyball39 physical order: exactly 39 bindings matching the
// shield matrix-transform (positions 0..38), no padding.
//
// The Keyball39 bottom row is asymmetric: the left half has 3 spare keys
// (positions 30-32) plus 3 thumbs (33-35); the trackball leaves the right
// half only 2 thumbs (36-37) plus an outer pinky-bottom key (38). The spare
// left keys map to &none, and position 38 doubles as Miryoku's third right
// thumb (K37) so all six thumb keys remain available.
#define MIRYOKU_LAYOUTMAPPING_KEYBALL39( \
K00, K01, K02, K03, K04,      K05, K06, K07, K08, K09, \
K10, K11, K12, K13, K14,      K15, K16, K17, K18, K19, \
K20, K21, K22, K23, K24,      K25, K26, K27, K28, K29, \
N30, N31, K32, K33, K34,      K35, K36, K37, N38, N39 \
) \
K00  K01  K02  K03  K04       K05  K06  K07  K08  K09 \
K10  K11  K12  K13  K14       K15  K16  K17  K18  K19 \
K20  K21  K22  K23  K24       K25  K26  K27  K28  K29 \
XXX  XXX  XXX  K32  K33  K34   K35  K36  K37

#endif

#define MIRYOKU_MAPPING MIRYOKU_LAYOUTMAPPING_KEYBALL39
