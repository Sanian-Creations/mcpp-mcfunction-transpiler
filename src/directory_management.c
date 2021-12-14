#include "directory_management.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <stdio.h> // perror


// TODO: implementation for Linux
// use some ifdefs to select the correct code at compiletime.


// mkdir will "fail" if the given directory already exists,
// so be sure to use the exists() and is_directory() functions
// beforehand.
int make_directory(const char* name) {
  
  /* linux */
  // https://jameshfisher.com/2017/02/24/what-is-mode_t/
  // return mkdir(name, 0777);
  // 0777 means every single permission, no read-only bs.

  /* windows */
  return mkdir(name);
}

bool file_exists(const char* name) {
  return access(name, F_OK) == 0;
}

bool is_regular_file(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

bool is_directory(const char *path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

static int remove_entry(
  const char *path,
  const struct stat *sb,
  int typeflag,
  struct FTW *ftwbuf)
{
  // FTW_D flag means this entry is a directory, which requires a
  // different remove function.
  int error_code = (typeflag & FTW_D) ? rmdir(path) : unlink(path);

  if (error_code) perror(path);
  return error_code;
}

// https://stackoverflow.com/questions/5467725/how-to-delete-a-directory-and-its-contents-in-posix-c
int remove_rf(char *path) {
  const int flags = FTW_DEPTH | FTW_PHYS;
  // Walk the file tree, depth first, do not follow symbolic links.
  // Depth first because directories with content cannot be deleted.
  return nftw(path, remove_entry, 64, flags);
}
