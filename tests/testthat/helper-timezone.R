skip_if_bad_sys_timezone <- function() {
  timezone <- Sys.timezone()

  # Guard against possible bad system time zones
  skip_if(
    is.na(timezone) || identical(timezone, ""),
    "Invalid Sys.timezone()"
  )
}
