#include "callables.h" // Must be included before R headers
#include "exports.h"

#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <stdbool.h> // for bool
#include <R_ext/Rdynload.h>

// .Call entries
static const R_CallMethodDef CallEntries[] = {
  {"export_cctz_test_civil_day", (DL_FUNC) &export_cctz_test_civil_day, 0},
  {"export_tz_local",            (DL_FUNC) &export_tz_local, 0},
  {NULL, NULL, 0}
};

extern "C" void R_init_rcctz(DllInfo *dll) {
  R_RegisterCCallable("rcctz", "lookup_civil", (DL_FUNC) &lookup_civil);
  R_RegisterCCallable("rcctz", "convert_civil", (DL_FUNC) &convert_civil);
  R_RegisterCCallable("rcctz", "lookup_time_point", (DL_FUNC) &lookup_time_point);
  R_RegisterCCallable("rcctz", "convert_time_point", (DL_FUNC) &convert_time_point);
  R_RegisterCCallable("rcctz", "tz_load", (DL_FUNC) &tz_load);
  R_RegisterCCallable("rcctz", "tz_from_tzone", (DL_FUNC) &tz_from_tzone);

  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
