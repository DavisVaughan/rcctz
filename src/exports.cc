#include "exports.h"

// -----------------------------------------------------------------------------
#include "examples.h"

extern "C" SEXP export_cctz_test_civil_day() {
  return cctz_test_civil_day();
}
