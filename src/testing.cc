#include "r.h"
#include "callables.h"
#include <chrono>

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
SEXP rcctz_convert_civil(SEXP year,
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

  cctz::time_point<cctz::seconds> tp = convert_civil(cs, tz);

  SEXP out = PROTECT(Rf_allocVector(REALSXP, 1));

  REAL(out)[0] = tp.time_since_epoch().count();

  UNPROTECT(1);
  return out;
}

extern "C"
SEXP rcctz_lookup_time_point(SEXP point, SEXP tzone) {
  cctz::seconds sec(INTEGER(point)[0]);
  cctz::time_point<cctz::seconds> tp(sec);

  cctz::time_zone tz;
  std::string cpp_tz = tz_from_tzone(tzone);

  if (!tz_load(cpp_tz, &tz)) {
    Rf_errorcall(R_NilValue, "Failed to load time zone.");
  }

  cctz::time_zone::absolute_lookup lookup = lookup_time_point(tp, tz);

  SEXP out = PROTECT(Rf_allocVector(VECSXP, 3));

  SEXP cs = PROTECT(Rf_allocVector(INTSXP, 6));
  INTEGER(cs)[0] = lookup.cs.year();
  INTEGER(cs)[1] = lookup.cs.month();
  INTEGER(cs)[2] = lookup.cs.day();
  INTEGER(cs)[3] = lookup.cs.hour();
  INTEGER(cs)[4] = lookup.cs.minute();
  INTEGER(cs)[5] = lookup.cs.second();

  SET_VECTOR_ELT(out, 0, cs);
  SET_VECTOR_ELT(out, 1, Rf_ScalarLogical(lookup.is_dst));
  SET_VECTOR_ELT(out, 2, Rf_ScalarInteger(lookup.offset));

  UNPROTECT(2);
  return out;
}

extern "C"
SEXP rcctz_convert_time_point(SEXP point, SEXP tzone) {
  cctz::seconds sec(INTEGER(point)[0]);
  cctz::time_point<cctz::seconds> tp(sec);

  cctz::time_zone tz;
  std::string cpp_tz = tz_from_tzone(tzone);

  if (!tz_load(cpp_tz, &tz)) {
    Rf_errorcall(R_NilValue, "Failed to load time zone.");
  }

  cctz::civil_second cs = convert_time_point(tp, tz);

  SEXP out = PROTECT(Rf_allocVector(INTSXP, 6));
  INTEGER(out)[0] = cs.year();
  INTEGER(out)[1] = cs.month();
  INTEGER(out)[2] = cs.day();
  INTEGER(out)[3] = cs.hour();
  INTEGER(out)[4] = cs.minute();
  INTEGER(out)[5] = cs.second();

  UNPROTECT(1);
  return out;
}

extern "C"
SEXP rcctz_force_tz(SEXP x, SEXP tzone_from, SEXP tzone_to, SEXP dst) {
  const char* c_dst = CHAR(STRING_ELT(dst, 0));
  bool dst_original = !strcmp(c_dst, "original");
  bool dst_new = !strcmp(c_dst, "new");
  bool dst_boundary = !strcmp(c_dst, "boundary");
  bool dst_missing = !strcmp(c_dst, "missing");

  cctz::seconds sec(INTEGER(x)[0]);
  cctz::time_point<cctz::seconds> tp(sec);

  cctz::time_zone tz_from;
  std::string cpp_tz_from = tz_from_tzone(tzone_from);

  cctz::time_zone tz_to;
  std::string cpp_tz_to = tz_from_tzone(tzone_to);

  if (!tz_load(cpp_tz_from, &tz_from)) {
    Rf_errorcall(R_NilValue, "Failed to load time zone.");
  }
  if (!tz_load(cpp_tz_to, &tz_to)) {
    Rf_errorcall(R_NilValue, "Failed to load time zone.");
  }

  cctz::civil_second cs = convert_time_point(tp, tz_from);
  cctz::time_zone::civil_lookup cl = lookup_civil(cs, tz_to);

  SEXP out = PROTECT(Rf_allocVector(REALSXP, 1));
  double* p_out = REAL(out);

  if (cl.kind == cctz::time_zone::civil_lookup::UNIQUE) {
    p_out[0] = cl.trans.time_since_epoch().count();
    UNPROTECT(1);
    return out;
  }

  if (cl.kind == cctz::time_zone::civil_lookup::SKIPPED ||
      cl.kind == cctz::time_zone::civil_lookup::REPEATED) {
    if (dst_original) {
      p_out[0] = cl.pre.time_since_epoch().count();
    } else if (dst_new) {
      p_out[0] = cl.post.time_since_epoch().count();
    } else if (dst_boundary) {
      p_out[0] = cl.trans.time_since_epoch().count();
    } else if (dst_missing) {
      p_out[0] = NA_REAL;
    } else {
      Rf_errorcall(R_NilValue, "Unknown dst");
    }

    UNPROTECT(1);
    return out;
  }

  Rf_errorcall(R_NilValue, "Should never get here");
}

extern "C"
SEXP rcctz_tz_local() {
  return Rf_ScalarString(Rf_mkCharCE(tz_local(), CE_UTF8));
}

extern "C"
SEXP rcctz_tz_system() {
  return Rf_ScalarString(Rf_mkCharCE(tz_system(), CE_UTF8));
}
