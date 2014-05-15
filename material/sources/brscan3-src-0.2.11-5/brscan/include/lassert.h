/* sane - Scanner Access Now Easy.
   Copyright (C) 2001 by Henning Meier-Geinitz
   Based on gcc header file assert.h.
   This file is part of the SANE package.

   SANE is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   SANE is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with sane; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Choose suitable implementation of assert.
*/

#ifndef lassert_h
#define lassert_h


#if defined __GNUC__ && defined _AIX
/* The implementation of assert of gcc on AIX is in libgcc.a. This 
   doesn't work with shared libraries. So let's make our own assert(). */
#define assert(arg)  \
 ((void) ((arg) ? 0 : lassert (arg, __FILE__, __LINE__)))
#define lassert(arg, file, lineno)  \
  (printf ("%s:%u: failed assertion\n", file, lineno),  \
   abort (), 0)
#else
# include <assert.h>
#endif

#endif /* lassert_h */
