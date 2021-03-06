#include "callables.h" // Must be included before R headers

#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <stdbool.h> // for bool
#include <R_ext/Rdynload.h>

extern "C" SEXP rcctz_lookup_civil(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern "C" SEXP rcctz_convert_civil(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern "C" SEXP rcctz_lookup_seconds_point(SEXP, SEXP);
extern "C" SEXP rcctz_convert_seconds_point(SEXP, SEXP);
extern "C" SEXP rcctz_force_tz(SEXP, SEXP, SEXP, SEXP);
extern "C" SEXP rcctz_tz_local();
extern "C" SEXP rcctz_tz_system();

// .Call entries
static const R_CallMethodDef CallEntries[] = {
  {"rcctz_lookup_civil",          (DL_FUNC) &rcctz_lookup_civil, 7},
  {"rcctz_convert_civil",         (DL_FUNC) &rcctz_convert_civil, 7},
  {"rcctz_lookup_seconds_point",  (DL_FUNC) &rcctz_lookup_seconds_point, 2},
  {"rcctz_convert_seconds_point", (DL_FUNC) &rcctz_convert_seconds_point, 2},
  {"rcctz_force_tz",              (DL_FUNC) &rcctz_force_tz, 4},
  {"rcctz_tz_local",              (DL_FUNC) &rcctz_tz_local, 0},
  {"rcctz_tz_system",             (DL_FUNC) &rcctz_tz_system, 0},
  {NULL, NULL, 0}
};

extern "C" void R_init_rcctz(DllInfo *dll) {
  R_RegisterCCallable("rcctz", "lookup_civil",          (DL_FUNC) &lookup_civil);
  R_RegisterCCallable("rcctz", "convert_civil",         (DL_FUNC) &convert_civil);
  R_RegisterCCallable("rcctz", "lookup_seconds_point",  (DL_FUNC) &lookup_seconds_point);
  R_RegisterCCallable("rcctz", "convert_seconds_point", (DL_FUNC) &convert_seconds_point);
  R_RegisterCCallable("rcctz", "tz_load",               (DL_FUNC) &tz_load);
  R_RegisterCCallable("rcctz", "tz_from_tzone",         (DL_FUNC) &tz_from_tzone);

  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
