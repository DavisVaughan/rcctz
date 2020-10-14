test_that("civil second lookup works", {
  expect_identical(
    lookup_civil(tzone = "America/New_York"),
    as.POSIXct("1970-01-01", "America/New_York") + rep(0, 3)
  )

  gap1 <- lookup_civil(month = 4L, day = 26L, hour = 2L, minute = 1L, tzone = "America/New_York")
  gap2 <- lookup_civil(month = 4L, day = 26L, hour = 2L, minute = 1L, tzone = "America/Los_Angeles")

  x <- c("1970-04-26 03:00:00", "1970-04-26 03:01:00", "1970-04-26 01:01:00")

  expect1 <- as.POSIXct(x, tz = "America/New_York")
  expect2 <- as.POSIXct(x, tz = "America/Los_Angeles")

  expect_identical(gap1, expect1)
  expect_identical(gap2, expect2)
})

test_that("civil second conversion works", {
  expect_identical(
    convert_civil(tzone = "America/New_York"),
    as.POSIXct("1970-01-01", "America/New_York")
  )

  # For gaps, conversion always chooses nearest after the gap
  gap1 <- convert_civil(month = 4L, day = 26L, hour = 2L, minute = 1L, tzone = "America/New_York")
  gap2 <- convert_civil(month = 4L, day = 26L, hour = 2L, minute = 1L, tzone = "America/Los_Angeles")

  # For ambiguous due to fallback, conversion always chooses the first date prior to fallback
  fall1 <- convert_civil(month = 10L, day = 25L, hour = 1L, minute = 1L, tzone = "America/New_York")
  fall2 <- convert_civil(month = 10L, day = 25L, hour = 1L, minute = 1L, tzone = "America/Los_Angeles")

  expect1 <- as.POSIXct("1970-04-26 03:00:00", tz = "America/New_York")
  expect2 <- as.POSIXct("1970-04-26 03:00:00", tz = "America/Los_Angeles")

  expect3 <- new_posixct(25678860, "America/New_York")
  expect4 <- new_posixct(25689660, "America/Los_Angeles")

  expect_identical(gap1, expect1)
  expect_identical(gap2, expect2)

  expect_identical(fall1, expect3)
  expect_identical(fall2, expect4)
})

test_that("lookup time point works", {
  x <- lookup_seconds_point(0L, "UTC")
  y <- lookup_seconds_point(1L, "UTC")

  expect_identical(x[[1]], c(1970L, 1L, 1L, 0L, 0L, 0L))
  expect_identical(y[[1]], c(1970L, 1L, 1L, 0L, 0L, 1L))

  test1 <- as.POSIXct("1970-04-26 01:59:59", tz = "America/New_York")
  test2 <- test1 + 1L

  a <- lookup_seconds_point(as.integer(test1), "America/New_York")
  b <- lookup_seconds_point(as.integer(test2), "America/New_York")

  expect_false(a[[2]])
  expect_true(b[[2]])

  expect_identical(a[[3]], -18000L)
  expect_identical(b[[3]], -14400L)
})

test_that("can convert time point", {
  expect_identical(convert_seconds_point(), c(1970L, 1L, 1L, 0L, 0L, 0L))
  expect_identical(convert_seconds_point(-1L), c(1969L, 12L, 31L, 23L, 59L, 59L))
  expect_identical(convert_seconds_point(tzone = "America/New_York"), c(1969L, 12L, 31L, 19L, 0L, 0L))
})

test_that("can do a forced time zone change", {
  expect_identical(
    force_tz(0L, "UTC", "America/New_York", "boundary"),
    as.POSIXct("1970-01-01", tz = "America/New_York")
  )

  test <- as.integer(as.POSIXct("1970-04-26 02:00:01", tz = "UTC"))

  expect_identical(
    force_tz(test, "UTC", "America/New_York", "boundary"),
    as.POSIXct("1970-04-26 03:00:00", tz = "America/New_York")
  )
  expect_identical(
    force_tz(test, "UTC", "America/New_York", "original"),
    as.POSIXct("1970-04-26 03:00:01", tz = "America/New_York")
  )
  expect_identical(
    force_tz(test, "UTC", "America/New_York", "new"),
    as.POSIXct("1970-04-26 01:00:01", tz = "America/New_York")
  )
  expect_identical(
    force_tz(test, "UTC", "America/New_York", "missing"),
    as.POSIXct(NA_character_, tz = "America/New_York")
  )
})

test_that("`TZ` is used if set", {
  expect_identical(
    withr::with_envvar(new = c("TZ" = "foo"), tz_local()),
    "foo"
  )
})

test_that("`TZ` warns if set to empty string and falls back to system tz", {
  # Windows `Sys.setenv(FOO = "")` unsets the envvar on Windows,
  # see `?Sys.setenv` Details
  skip_on_os("windows")

  skip_if_bad_sys_timezone()

  expect_identical(
    expect_warning(
      withr::with_envvar(new = c("TZ" = ""), tz_local()),
      "Using system time zone"
    ),
    tz_system()
  )
})

test_that("system time zone can be looked up", {
  # There is some strange bug on Linux with R 3.4 where Sys.timezone() returns
  # UTC here but then tz_system() returns Etc/UTC. It seems to work on other
  # versions of R with Linux
  skip_on_os("linux")

  skip_if_bad_sys_timezone()

  expect_identical(
    Sys.timezone(),
    tz_system()
  )
})
