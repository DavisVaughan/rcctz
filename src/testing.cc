#include "r.h"
#include "callables.h"
#include <cctz/civil_time.h>

extern "C"
SEXP rcctz_test_civil_day() {
  cctz::civil_day d(2016, 2, 1);

  SEXP out = PROTECT(Rf_allocVector(INTSXP, 3));
  int* p_out = INTEGER(out);

  p_out[0] = d.year();
  p_out[1] = d.month();
  p_out[2] = d.day();

  UNPROTECT(1);
  return out;
}

extern "C"
SEXP rcctz_lookup_civil(SEXP year,
                        SEXP month,
                        SEXP day,
                        SEXP hour,
                        SEXP minute,
                        SEXP second,
                        SEXP tzone) {
  cctz::civil_second cs = cctz::civil_second(
    INTEGER(year)[0],
    INTEGER(month)[0],
    INTEGER(day)[0],
    INTEGER(hour)[0],
    INTEGER(minute)[0],
    INTEGER(second)[0]
  );

  cctz::time_zone tz;
  std::string cpp_tz = tz_from_tzone(tzone);

  if (!tz_load(cpp_tz, &tz)) {
    Rf_errorcall(R_NilValue, "Failed to load time zone.");
  }

  cctz::time_zone::civil_lookup cl = lookup_civil(cs, tz);

  SEXP out = PROTECT(Rf_allocVector(REALSXP, 3));

  REAL(out)[0] = cl.trans.time_since_epoch().count();
  REAL(out)[1] = cl.pre.time_since_epoch().count();
  REAL(out)[2] = cl.post.time_since_epoch().count();

  UNPROTECT(1);
  return out;
}

extern "C"
SEXP rcctz_tz_local() {
  return Rf_ScalarString(Rf_mkCharCE(tz_local(), CE_UTF8));
}

extern "C"
SEXP rcctz_tz_system() {
  return Rf_ScalarString(Rf_mkCharCE(tz_system(), CE_UTF8));
}
