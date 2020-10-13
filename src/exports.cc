#include "exports.h"

// -----------------------------------------------------------------------------
#include "testing.h"

extern "C" SEXP export_cctz_test_civil_day() {
  return cctz_test_civil_day();
}

// -----------------------------------------------------------------------------
#include "callables.h"

extern "C" SEXP export_tz_local() {
  return Rf_ScalarString(Rf_mkCharCE(tz_local(), CE_UTF8));
}
