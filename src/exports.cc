#include "exports.h"

// -----------------------------------------------------------------------------
#include "examples.h"

extern "C" SEXP export_cctz_test() {
  return cctz_test();
}
