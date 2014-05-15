#include "../include/sane/config.h"

#ifndef HAVE_GETENV

char *
getenv(const char *name)
{
  char *retval = 0;
#ifdef HAVE_OS2_H
  if (0 != DosScanEnv (buf, &retval))
    retval = 0;
#else
#  error "Missing getenv() on this platform.  Please implement."
#endif
  return retval;
}

#endif /* !HAVE_GETENV */
