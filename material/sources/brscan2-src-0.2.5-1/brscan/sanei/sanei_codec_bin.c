/* sane - Scanner Access Now Easy.
   Copyright (C) 1997 David Mosberger-Tang
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
   If you do not wish that, delete this exception notice.  */

#include "../include/sane/config.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sane/sane.h"
#include "../include/sane/sanei_wire.h"
#include "../include/sane/sanei_codec_bin.h"

static void
bin_w_byte (Wire *w, void *v)
{
  SANE_Byte *b = v;

  sanei_w_space (w, 1);
  switch (w->direction)
    {
    case WIRE_ENCODE:
      *w->buffer.curr++ = *b;
      break;

    case WIRE_DECODE:
      *b = *w->buffer.curr++;
      break;

    case WIRE_FREE:
	break;
    }
}

static void
bin_w_string (Wire *w, void *v)
{
  SANE_Word len;
  SANE_String *s = v;

  if (w->direction != WIRE_DECODE)
    {
      len = 0;
      if (*s)
	len = strlen (*s) + 1;
    }
  sanei_w_array (w, &len, v, w->codec.w_byte, 1);
  if (w->direction == WIRE_DECODE && !len)
    *s = 0;
}

static void
bin_w_word (Wire *w, void *v)
{
  SANE_Word val, *word = v;

  sanei_w_space (w, 4);
  switch (w->direction)
    {
    case WIRE_ENCODE:
      val = *word;
      /* store in bigendian byte-order: */
      w->buffer.curr[0] = (val >> 24) & 0xff;
      w->buffer.curr[1] = (val >> 16) & 0xff;
      w->buffer.curr[2] = (val >>  8) & 0xff;
      w->buffer.curr[3] = (val >>  0) & 0xff;
      w->buffer.curr += 4;
      break;

    case WIRE_DECODE:
      val = (  ((w->buffer.curr[0] & 0xff) << 24)
	     | ((w->buffer.curr[1] & 0xff) << 16)
	     | ((w->buffer.curr[2] & 0xff) <<  8)
	     | ((w->buffer.curr[3] & 0xff) <<  0));
      *word = val;
      w->buffer.curr += 4;
      break;

    case WIRE_FREE:
      break;
    }
}

void
sanei_codec_bin_init (Wire *w)
{
  w->codec.w_byte = bin_w_byte;
  w->codec.w_char = bin_w_byte;
  w->codec.w_word = bin_w_word;
  w->codec.w_string = bin_w_string;
}
