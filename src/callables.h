#ifndef RCCTZ_CALLABLES_H
#define RCCTZ_CALLABLES_H

#include <cctz/civil_time.h>
#include <cctz/time_zone.h>
#include "r.h"

// Same as the `seconds_point` exposed in `rcctz.h`
using seconds_point = cctz::time_point<cctz::seconds>;

// -----------------------------------------------------------------------------
// Civil

cctz::time_zone::civil_lookup lookup_civil(const cctz::civil_second& cs,
                                           const cctz::time_zone& tz);

seconds_point convert_civil(const cctz::civil_second& cs,
                            const cctz::time_zone& tz);

// -----------------------------------------------------------------------------
// Time point

cctz::time_zone::absolute_lookup lookup_seconds_point(const seconds_point& sp,
                                                      const cctz::time_zone& tz);

cctz::civil_second convert_seconds_point(const seconds_point& sp,
                                         const cctz::time_zone& tz);

// -----------------------------------------------------------------------------
// Time zone loading

bool tz_load(const std::string& name, cctz::time_zone* tz);

std::string tz_from_tzone(SEXP tzone);

// Just for testing
const char* tz_local();
const char* tz_system();

#endif
