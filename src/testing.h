#ifndef RCCTZ_TESTING_H
#define RCCTZ_TESTING_H

#include "r.h"

extern "C" SEXP rcctz_test_civil_day();

extern "C" SEXP rcctz_lookup_civil(SEXP year,
                                   SEXP month,
                                   SEXP day,
                                   SEXP hour,
                                   SEXP minute,
                                   SEXP second,
                                   SEXP tzone);

extern "C" SEXP rcctz_tz_local();

#endif
