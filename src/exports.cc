#include "exports.h"

// -----------------------------------------------------------------------------
#include "testing.h"

extern "C" SEXP export_cctz_test_civil_day() {
  return cctz_test_civil_day();
}

extern "C" SEXP export_cctz_test_lookup_civil(SEXP year,
                                              SEXP month,
                                              SEXP day,
                                              SEXP hour,
                                              SEXP minute,
                                              SEXP second,
                                              SEXP tzone) {
  return cctz_test_lookup_civil(year, month, day, hour, minute, second, tzone);
}

// -----------------------------------------------------------------------------
#include "callables.h"

extern "C" SEXP export_tz_local() {
  return Rf_ScalarString(Rf_mkCharCE(tz_local(), CE_UTF8));
}
