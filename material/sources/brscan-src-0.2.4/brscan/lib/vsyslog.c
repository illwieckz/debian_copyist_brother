#include "../include/sane/config.h"

#include "stdio.h"
#include <syslog.h>
#include <stdarg.h>

#ifndef HAVE_VSYSLOG

void
vsyslog(int priority, const char *format, va_list args)
{
  char buf[1024];
  vsnprintf(buf, sizeof(buf), format, args);
  syslog(priority, "%s", buf);
}

#endif /* !HAVE_VSYSLOG */
