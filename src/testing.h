#ifndef RCCTZ_EXAMPLES_H
#define RCCTZ_EXAMPLES_H

#include "r.h"

SEXP cctz_test_civil_day();

SEXP cctz_test_lookup_civil(SEXP year,
                            SEXP month,
                            SEXP day,
                            SEXP hour,
                            SEXP minute,
                            SEXP second,
                            SEXP tzone);

#endif
