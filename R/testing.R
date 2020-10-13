lookup_civil <- function(year = 1970L,
                         month = 1L,
                         day = 1L,
                         hour = 0L,
                         minute = 0L,
                         second = 0L,
                         tzone = "America/New_York") {
  out <- .Call(rcctz_lookup_civil, year, month, day, hour, minute, second, tzone)
  new_posixct(out, tzone)
}

convert_civil <- function(year = 1970L,
                          month = 1L,
                          day = 1L,
                          hour = 0L,
                          minute = 0L,
                          second = 0L,
                          tzone = "America/New_York") {
  out <- .Call(rcctz_convert_civil, year, month, day, hour, minute, second, tzone)
  new_posixct(out, tzone)
}

tz_local <- function() {
  .Call(rcctz_tz_local)
}

tz_system <- function() {
  .Call(rcctz_tz_system)
}

new_posixct <- function(x, tzone) {
  structure(x, class = c("POSIXct", "POSIXt"), tzone = tzone)
}
