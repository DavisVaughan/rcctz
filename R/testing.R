cctz_test_civil_day <- function() {
  .Call(export_cctz_test_civil_day)
}

cctz_test_lookup_civil <- function(year = 1970L,
                                   month = 1L,
                                   day = 1L,
                                   hour = 0L,
                                   minute = 0L,
                                   second = 0L,
                                   tzone = "America/New_York") {
  out <- .Call(export_cctz_test_lookup_civil, year, month, day, hour, minute, second, tzone)
  structure(out, class = c("POSIXct", "POSIXt"), tzone = tzone)
}
