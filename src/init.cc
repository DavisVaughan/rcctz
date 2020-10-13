#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <stdbool.h> // for bool
#include <R_ext/Rdynload.h>

#include "exports.h"

extern "C" {

  // .Call entries
  static const R_CallMethodDef CallEntries[] = {
    {"export_cctz_test_civil_day", (DL_FUNC) &export_cctz_test_civil_day, 0},
    {NULL, NULL, 0}
  };

  void R_init_rcctz(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
  }

} // extern "C"
