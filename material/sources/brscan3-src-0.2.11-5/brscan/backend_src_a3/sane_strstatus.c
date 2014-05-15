/* sane - Scanner Access Now Easy.
   Copyright (C) 1996, 1997 David Mosberger-Tang and Andreas Beck
   This file is part of the SANE package.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.

   As a special exception, the authors of SANE give permission for
   additional uses of the libraries contained in this release of SANE.

   The exception is that, if you link a SANE library with other files
   to produce an executable, this does not by itself cause the
   resulting executable to be covered by the GNU General Public
   License.  Your use of that executable is in no way restricted on
   account of linking the SANE library code into it.

   This exception does not, however, invalidate any other reasons why
   the executable file might be covered by the GNU General Public
   License.

   If you submit changes to SANE to the maintainers to be included in
   a subsequent release, you agree by submitting the changes that
   those changes may be distributed with this exception intact.

   If you write modifications of your own for SANE, it is your choice
   whether to permit this exception to apply to your modifications.
   If you do not wish that, delete this exception notice.

   This file implements the backend-independent parts of SANE.  */

#include <stdio.h>

#include "sane/sane.h"

SANE_String_Const
sane_strstatus (SANE_Status status)
{
  static char buf[80];

  switch (status)
    {
    case SANE_STATUS_GOOD:
      return "Success";

    case SANE_STATUS_UNSUPPORTED:
      return "Operation not supported";

    case SANE_STATUS_CANCELLED:
      return "Operation was cancelled";

    case SANE_STATUS_DEVICE_BUSY:
      return "Device busy";

    case SANE_STATUS_INVAL:
      return "Invalid argument";

    case SANE_STATUS_EOF:
      return "End of file reached";

    case SANE_STATUS_JAMMED:
      return "Document feeder jammed";

    case SANE_STATUS_NO_DOCS:
      return "Document feeder out of documents";

    case SANE_STATUS_COVER_OPEN:
      return "Scanner cover is open";

    case SANE_STATUS_IO_ERROR:
      return "Error during device I/O";

    case SANE_STATUS_NO_MEM:
      return "Out of memory";

    case SANE_STATUS_ACCESS_DENIED:
      return "Access to resource has been denied";

    default:
      /* non-reentrant, but better than nothing */
      sprintf (buf, "Unknown SANE status code %d", status);
      return buf;
    }
}
