library(here)
library(fs)
library(glue)

# ------------------------------------------------------------------------------

# Updating:
# 1) Run the lines in this file to update the headers and source code
# 2) Re-update `time_zone.h` to comment out `parse()` and `format()`

# ------------------------------------------------------------------------------

# Files in cctz/src that we don't need
remove_src <- c(
  "cctz_benchmark.cc",
  "civil_time_test.cc",
  "time_tool.cc",
  "time_zone_format_test.cc",
  "time_zone_lookup_test.cc"
)

# TODO: Re-include `time_zone_format.cc` for access to `parse()` and `format()`.
#
# We remove it for now because Windows doesn't have a `strptime()`, so CCTZ
# tries to mock one using `std::get_time()`. This is a C++11 function that was
# added to the g++ compiler in version 5. Sadly, before Rtools40, only g++4.9
# was used on Windows so on Windows + R < 4.0.0 this won't compile.
#
# Technically `strptime()` seems to be only required for `parse()`, meaning
# that `format()` probably works out of the box if we comment out the `parse()`
# code in `time_zone_format.cc`.
#
# There is a backport of `get_time()` that could be used. This is what
# RcppCCTZ does, but the library is very useful even without this code.
# https://github.com/eddelbuettel/rcppcctz/pull/11
remove_src <- c(remove_src, "time_zone_format.cc")

# ------------------------------------------------------------------------------
# cd into temp dir
# git clone the repo

dir_temp <- tempdir()
dir_dest <- dir_create(path(dir_temp, "dest/"))

url_repo <- "https://github.com/google/cctz"

cmd_cd <- glue(shQuote("cd"), " ", dir_dest)
cmd_clone <- glue(shQuote("git"), " clone ", url_repo)
cmd <- glue(cmd_cd, "; ", cmd_clone)

# cd into temp dir
# git clone the repo
system(cmd)

dir_cctz <- path(dir_dest, "cctz")

# ------------------------------------------------------------------------------
# Update headers
# Will overwrite inst/include/cctz

dir_cctz_include_cctz <- path(dir_cctz, "include", "cctz")
dir_pkg_inst_include_cctz <- here("inst", "include", "cctz")

dir_copy(
  path = dir_cctz_include_cctz,
  new_path = dir_pkg_inst_include_cctz,
  overwrite = TRUE
)

# ------------------------------------------------------------------------------
# Update src
# Will overwrite src/

dir_cctz_src <- path(dir_cctz, "src")
dir_pkg_src <- here("src")

dir_copy(
  path = dir_cctz_src,
  new_path = dir_pkg_src,
  overwrite = TRUE
)

files_pkg_src_remove <- path(dir_pkg_src, remove_src)

file_delete(files_pkg_src_remove)

# ------------------------------------------------------------------------------

unlink(dir_dest, recursive = TRUE, force = TRUE)

# ------------------------------------------------------------------------------
