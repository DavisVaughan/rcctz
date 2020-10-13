#include "callables.h"

// -----------------------------------------------------------------------------
// Civil

cctz::time_zone::civil_lookup lookup_civil(const cctz::civil_second& cs,
                                           const cctz::time_zone& tz) {
  return tz.lookup(cs);
}

cctz::time_point<cctz::seconds> convert_civil(const cctz::civil_second& cs,
                                              const cctz::time_zone& tz) {
  return cctz::convert(cs, tz);
}

// -----------------------------------------------------------------------------
// Time point

cctz::time_zone::absolute_lookup lookup_time_point(const cctz::time_point<cctz::seconds>& tp,
                                                   const cctz::time_zone& tz) {
  return tz.lookup(tp);
}

cctz::civil_second convert_time_point(const cctz::time_point<cctz::seconds>& tp,
                                      const cctz::time_zone& tz) {
  return cctz::convert(tp, tz);
}

// -----------------------------------------------------------------------------
// Time zone loading

bool tz_load(const std::string& name, cctz::time_zone* tz) {
  return cctz::load_time_zone(name, tz);
}
