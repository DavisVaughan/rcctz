test_that("civil day test works", {
  expect_identical(test_civil_day(), c(2016L, 2L, 1L))
})

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

test_that("`TZ` is used if set", {
  expect_identical(
    withr::with_envvar(new = c("TZ" = "foo"), tz_local()),
    "foo"
  )
})

test_that("`TZ` warns if set to empty string and falls back to system tz", {
  skip_if_bad_sys_timezone()

  expect_identical(
    withr::with_envvar(new = c("TZ" = ""), tz_local()),
    tz_system()
  )
})

test_that("system time zone can be looked up", {
  skip_if_bad_sys_timezone()

  expect_identical(
    Sys.timezone(),
    tz_system()
  )
})
