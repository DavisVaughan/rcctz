#ifndef RCCTZ_H
#define RCCTZ_H

#include "cctz/civil_time.h"
#include "cctz/time_zone.h"

#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <stdbool.h>

namespace rcctz {
  inline cctz::time_zone::civil_lookup lookup_civil(const cctz::civil_second& cs,
                                                    const cctz::time_zone tz) {
    typedef cctz::time_zone::civil_lookup fn_ptr(const cctz::civil_second& cs,
                                                 const cctz::time_zone tz);

    static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "lookup_civil");

    return fn(cs, tz);
  }

  inline bool tz_load(const std::string& name, cctz::time_zone* tz) {
    typedef bool fn_ptr(const std::string& name, cctz::time_zone* tz);

    static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "tz_load");

    return fn(name, tz);
  }
}

#endif
