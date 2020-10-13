#include "callables.h"

// -----------------------------------------------------------------------------
// Civil

void lookup_civil(const cctz::civil_second& cs,
                  const cctz::time_zone tz,
                  cctz::time_zone::civil_lookup* cl) {
  *cl = tz.lookup(cs);
}

// -----------------------------------------------------------------------------
// Time zone loading

bool tz_load(const std::string& name, cctz::time_zone* tz) {
  return cctz::load_time_zone(name, tz);
}
