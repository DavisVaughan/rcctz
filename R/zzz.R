.onLoad <- function(libname, pkgname) {
  set_tzdir()
}

set_tzdir <- function() {
  tzdir <- Sys.getenv("TZDIR", unset = "")

  # User already set TZDIR
  if (!identical(tzdir, "")) {
    return()
  }

  # Use R's zoneinfo on Windows
  if (.Platform$OS.type == "windows") {
    tzdir <- file.path(R.home(), "share", "zoneinfo")
    Sys.setenv(TZDIR = tzdir)
    return()
  }

  # On Unix, if this folder exists,
  # then CCTZ's `tzfile.h` will use it by default
  if (file.exists("/usr/share/zoneinfo")) {
    return()
  }

  # In the rare scenario that zoneinfo isn't in the default location,
  # try a number of other locations that lubridate also tries and
  # choose the first one we find. Notably, we try R's zoneinfo too.
  tzdirs <- c(
    file.path(R.home(), "share", "zoneinfo"),
    "/usr/share/lib/zoneinfo",
    "/usr/lib/zoneinfo",
    "/usr/local/etc/zoneinfo",
    "/etc/zoneinfo",
    "/usr/etc/zoneinfo"
  )

  for (tzdir in tzdirs) {
    if (file.exists(tzdir)) {
      Sys.setenv(TZDIR = tzdir)
      return()
    }
  }

  stop(
    "Cannot load 'rcctz'. Time zone info folder, 'zoneinfo', not found.",
    call. = FALSE
  )
}
