library(here)
library(fs)
library(glue)

# ------------------------------------------------------------------------------

# Files in cctz/src that we don't need
remove_src <- c(
  "cctz_benchmark.cc",
  "civil_time_test.cc",
  "time_tool.cc",
  "time_zone_format_test.cc",
  "time_zone_lookup_test.cc"
)

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
