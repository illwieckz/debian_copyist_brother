/* Copyright (C) 1997 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "../include/sane/config.h"

#include <errno.h>

#ifndef HAVE_SIGPROCMASK

#define sigprocmask	SOMETHINGELSE
#include <signal.h>
#undef  sigprocmask

int
sigprocmask (int how, int *new, int *old)
{
  int o, n = *new;

  switch (how)
    {
    case 1: o = sigblock (n); break;
    case 2: o = sigsetmask (sigblock (0) & ~n); break;
    case 3: o = sigsetmask (n); break;
    default:
      errno = EINVAL;
      return -1;
    }
  if (old)
    *old = o;
  return 0;
}

#endif /* !HAVE_SIGPROCMASK */
