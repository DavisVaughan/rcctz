#include "callables.h"
#include <string.h>

// -----------------------------------------------------------------------------
// Civil

cctz::time_zone::civil_lookup lookup_civil(const cctz::civil_second& cs,
                                           const cctz::time_zone& tz) {
  return tz.lookup(cs);
}

seconds_point convert_civil(const cctz::civil_second& cs,
                            const cctz::time_zone& tz) {
  return cctz::convert(cs, tz);
}

// -----------------------------------------------------------------------------
// Time point

cctz::time_zone::absolute_lookup lookup_seconds_point(const seconds_point& sp,
                                                      const cctz::time_zone& tz) {
  return tz.lookup(sp);
}

cctz::civil_second convert_seconds_point(const seconds_point& sp,
                                         const cctz::time_zone& tz) {
  return cctz::convert(sp, tz);
}

// -----------------------------------------------------------------------------
// Time zone loading

/*
 * Brought over from lubridate/timechange
 * https://github.com/vspinu/timechange/blob/9c1f769a6c85899665af8b86e43115d392c09989/src/tzone.cpp#L4
 */

/*
 * Extract a string time zone from an R `tzone` attribute
 *
 * This is slightly different from the lubridate version. For POSIXlt objects,
 * the time zone attribute might be a character vector of length 3.
 * If the first element is `""` (which happens on a Mac with
 * `as.POSIXlt(Sys.time())`), then lubridate will look to the second element
 * and will use that as the time zone. I think this is incorrect, because those
 * are always time zone abbreviations, and will fail to load because they
 * aren't true time zone names. I think that is the reason Vitalie opened
 * this issue, and the reason for the time zone map in lubridate. This function
 * works more like `lubridate:::tz.POSIXt()` which just takes the first element
 * of the tzone attribute.
 * https://github.com/google/cctz/issues/46
 * https://github.com/tidyverse/lubridate/blob/b9025e6d5152f9da3857d7ef18f2571d3d861bae/src/update.cpp#L49
 */
std::string tz_from_tzone(SEXP tzone) {
  if (tzone == R_NilValue) {
    return "";
  }

  if (TYPEOF(tzone) != STRSXP) {
    Rf_errorcall(R_NilValue, "`tzone` must be a character vector or `NULL`.");
  }

  R_xlen_t n = Rf_xlength(tzone);

  // Assume `character()` tzone is also local time
  if (n == 0) {
    return "";
  }

  const char* out = CHAR(STRING_ELT(tzone, 0));

  return out;
}

const char* tz_local();

bool tz_load(const std::string& name, cctz::time_zone* tz) {
  if (name.size() == 0) {
    // CCTZ local time zone function doesn't work on windows,
    // so we look up the local time zone using R's `Sys.timezone()`
    // or the `TZ` envvar when an empty string is the input.
    // https://github.com/google/cctz/issues/53
    return cctz::load_time_zone(tz_local(), tz);
  }

  return cctz::load_time_zone(name, tz);
}

const char* tz_system();

const char* tz_local() {
  const char* tz_env = std::getenv("TZ");

  if (tz_env == NULL) {
    // Unset, use system tz
    return tz_system();
  }

  if (strlen(tz_env) == 0) {
    // If set but empty, R behaves in a system specific way and there is no way
    // to infer local time zone.
    Rf_warning("Environment variable `TZ` is set to \"\". Using system time zone.");
    return tz_system();
  }

  return tz_env;
}

static const char* tz_system_get();

const char* tz_system() {
  // Once per session
  static const char* TZ_SYS = NULL;

  if (TZ_SYS == NULL) {
    TZ_SYS = strdup(tz_system_get());
  }

  return TZ_SYS;
}

static const char* tz_system_get() {
  SEXP fn = Rf_findFun(Rf_install("Sys.timezone"), R_BaseEnv);
  SEXP call = PROTECT(Rf_lang1(fn));
  SEXP timezone = PROTECT(Rf_eval(call, R_GlobalEnv));

  if (TYPEOF(timezone) != STRSXP || Rf_length(timezone) != 1) {
    Rf_warning("Unexpected result from `Sys.timezone()`, returning 'UTC'.");
    UNPROTECT(2);
    return "UTC";
  }

  SEXP tz = STRING_ELT(timezone, 0);

  if (tz == NA_STRING || strlen(CHAR(tz)) == 0) {
    Rf_warning(
      "System timezone name is unknown. "
      "Please set the environment variable `TZ`. "
      "Defaulting to 'UTC'."
    );

    UNPROTECT(2);
    return "UTC";
  }

  UNPROTECT(2);
  return CHAR(tz);
}


