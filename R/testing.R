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

lookup_seconds_point <- function(point = 0L, tzone = "UTC") {
  .Call(rcctz_lookup_seconds_point, point, tzone)
}

convert_seconds_point <- function(point = 0L, tzone = "UTC") {
  .Call(rcctz_convert_seconds_point, point, tzone)
}

force_tz <- function(x, tzone_from, tzone_to, dst) {
  out <- .Call(rcctz_force_tz, x, tzone_from, tzone_to, dst)
  new_posixct(out, tzone_to)
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
