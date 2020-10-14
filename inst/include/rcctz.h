#ifndef RCCTZ_H
#define RCCTZ_H

#include "cctz/civil_time.h"
#include "cctz/time_zone.h"

#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <stdbool.h>

namespace rcctz {
  using cctz::civil_year;
  using cctz::civil_month;
  using cctz::civil_day;
  using cctz::civil_hour;
  using cctz::civil_minute;
  using cctz::civil_second;

  using cctz::weekday;

  // Implemented in the header, so safe to call
  using cctz::get_weekday;
  using cctz::next_weekday;
  using cctz::prev_weekday;
  using cctz::get_yearday;

  /*
   * Only `seconds_point` is exposed, as we can't exported a templated callable
   * version of `lookup_time_point()`. But that is okay because this is the only
   * one that really matters for R users.
   */
  using seconds_point = cctz::time_point<cctz::seconds>;
  using seconds = cctz::seconds;

  /*
   * Custom rcctz::time_zone that reimplements cctz::time_zone in a way
   * that calls the callables but still presents the same interface
   */
  class time_zone {
  public:
    time_zone() {
      tz_ = cctz::time_zone();
    }

    using absolute_lookup = cctz::time_zone::absolute_lookup;
    using civil_lookup = cctz::time_zone::civil_lookup;

    inline absolute_lookup lookup(const seconds_point& sp) const {
      typedef absolute_lookup fn_ptr(const seconds_point& sp, const cctz::time_zone& tz);
      static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "lookup_seconds_point");
      return fn(sp, tz_);
    }
    inline civil_lookup lookup(const civil_second& cs) const {
      typedef civil_lookup fn_ptr(const civil_second& cs, const cctz::time_zone& tz);
      static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "lookup_civil");
      return fn(cs, tz_);
    }

    inline civil_second convert(const seconds_point& sp) const {
      typedef civil_second fn_ptr(const seconds_point& sp, const cctz::time_zone& tz);
      static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "convert_seconds_point");
      return fn(sp, tz_);
    }
    inline seconds_point convert(const civil_second& cs) const {
      typedef seconds_point fn_ptr(const civil_second& cs, const cctz::time_zone& tz);
      static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "convert_civil");
      return fn(cs, tz_);
    }

    inline bool load(const std::string& name) {
      typedef bool fn_ptr(const std::string& name, cctz::time_zone* tz);
      static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "tz_load");
      return fn(name, &tz_);
    }

    // Relational operators.
    friend bool operator==(time_zone lhs, time_zone rhs) {
      return lhs.tz_ == rhs.tz_;
    }
    friend bool operator!=(time_zone lhs, time_zone rhs) {
      return !(lhs.tz_ == rhs.tz_);
    }

  private:
    cctz::time_zone tz_;
  }; // class time_zone

  inline civil_second convert(const seconds_point& sp, const time_zone& tz) {
    return tz.convert(sp);
  }
  inline seconds_point convert(const civil_second& cs, const time_zone& tz) {
    return tz.convert(cs);
  }

  inline bool tz_load(const std::string& name, time_zone* tz) {
    return tz->load(name);
  }
  inline std::string tz_from_tzone(SEXP tzone) {
    typedef std::string fn_ptr(SEXP tzone);
    static fn_ptr *fn = (fn_ptr*) R_GetCCallable("rcctz", "tz_from_tzone");
    return fn(tzone);
  }
} // namespace rcctz

#endif
