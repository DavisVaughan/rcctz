#ifndef RCCTZ_CALLABLES_H
#define RCCTZ_CALLABLES_H

#include <cctz/civil_time.h>
#include <cctz/time_zone.h>

// -----------------------------------------------------------------------------
// Civil

cctz::time_zone::civil_lookup lookup_civil(const cctz::civil_second& cs,
                                           const cctz::time_zone tz);

// -----------------------------------------------------------------------------
// Time zone loading

bool tz_load(const std::string& name, cctz::time_zone* tz);

#endif
