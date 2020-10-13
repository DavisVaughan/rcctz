#ifndef RCCTZ_EXPORTS_H
#define RCCTZ_EXPORTS_H

#include "r.h"

// -----------------------------------------------------------------------------
// #include "testing.h"

extern "C" SEXP export_cctz_test_civil_day();

extern "C" SEXP export_cctz_test_lookup_civil(SEXP year,
                                              SEXP month,
                                              SEXP day,
                                              SEXP hour,
                                              SEXP minute,
                                              SEXP second,
                                              SEXP tzone);

// -----------------------------------------------------------------------------
// #include "callables.h"

extern "C" SEXP export_tz_local();

// -----------------------------------------------------------------------------
#endif
