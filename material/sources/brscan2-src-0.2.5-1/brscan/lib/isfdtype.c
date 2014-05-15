#include "../include/sane/config.h"

#ifndef HAVE_ISFDTYPE

#include <sys/stat.h>

int
isfdtype(int fd, int fdtype)
{
  struct stat st;
  
  if (fstat(fd, &st) == -1) return 0; /* couldn't stat fd */

  if (st.st_mode == 0)
    return 1; /* At least Irix doesn't seem to know socket type */
#if defined(S_ISSOCK)
  return S_ISSOCK(st.st_mode) != 0;
#elif defined(S_IFSOCK) && defined(S_IFMT)
  return (st.st_mode & S_IFMT) == S_IFSOCK;
#else
  return 0;
#endif
}

#endif /* !HAVE_ISFDTYPE */
