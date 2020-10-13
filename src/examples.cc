#include "examples.h"
#include <cctz/civil_time.h>

SEXP cctz_test_civil_day() {
  cctz::civil_day d(2016, 2, 1);

  SEXP out = PROTECT(Rf_allocVector(INTSXP, 3));
  int* p_out = INTEGER(out);

  p_out[0] = d.year();
  p_out[1] = d.month();
  p_out[2] = d.day();

  UNPROTECT(1);
  return out;
}
