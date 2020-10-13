#ifndef RCCTZ_H
#define RCCTZ_H

#include "cctz/civil_time.h"
#include "cctz/time_zone.h"

#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <stdbool.h>


#ifdef HAVE_VISIBILITY_ATTRIBUTE
# define attribute_hidden __attribute__ ((visibility ("hidden")))
#else
# define attribute_hidden
#endif


#ifdef __cplusplus
extern "C"
#endif
inline
void attribute_hidden rcctz_lookup_civil(const cctz::civil_second& cs,
                                         const cctz::time_zone tz,
                                         cctz::time_zone::civil_lookup* cl) {
  typedef void fn_ptr(const cctz::civil_second& cs,
                      const cctz::time_zone tz,
                      cctz::time_zone::civil_lookup* cl);

  static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "lookup_civil");

  return fn(cs, tz, cl);
}

#ifdef __cplusplus
extern "C"
#endif
inline
bool attribute_hidden rcctz_tz_load(const std::string& name, cctz::time_zone* tz) {
  typedef bool fn_ptr(const std::string& name, cctz::time_zone* tz);

  static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "tz_load");

  return fn(name, tz);
}

#ifdef __cplusplus

namespace rcctz {
  inline void lookup_civil(const cctz::civil_second& cs,
                           const cctz::time_zone tz,
                           cctz::time_zone::civil_lookup* cl) {
    return rcctz_lookup_civil(cs, tz, cl);
  }

  inline bool tz_load(const std::string& name, cctz::time_zone* tz) {
    return rcctz_tz_load(name, tz);
  }
}

#endif // __cplusplus

#endif
