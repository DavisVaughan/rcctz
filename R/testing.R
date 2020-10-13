test_civil_day <- function() {
  .Call(rcctz_test_civil_day)
}

lookup_civil <- function(year = 1970L,
                         month = 1L,
                         day = 1L,
                         hour = 0L,
                         minute = 0L,
                         second = 0L,
                         tzone = "America/New_York") {
  out <- .Call(rcctz_lookup_civil, year, month, day, hour, minute, second, tzone)
  structure(out, class = c("POSIXct", "POSIXt"), tzone = tzone)
}

tz_local <- function() {
  .Call(rcctz_tz_local)
}
