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

#include "sane/config.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "../include/sane/sane.h"
#include "../include/sane/sanei_wire.h"
#include "../include/sane/sanei_codec_ascii.h"

static const char *hexdigit = "0123456789abcdef";

static void
skip_ws (Wire *w)
{
  while (1)
    {
      sanei_w_space (w, 1);
      if (w->status != 0)
	return;

      if (!isspace (*w->buffer.curr))
	return;

      ++w->buffer.curr;
    }
}

static unsigned
get_digit (Wire *w)
{
  unsigned digit;

  sanei_w_space (w, 1);
  digit = tolower(*w->buffer.curr++) - '0';
  if (digit > 9)
    digit -= 'a' - ('9' + 1);
  if (digit > 0xf)
    {
      w->status = EINVAL;
      return 0;
    }
  return digit;
}

static SANE_Byte
get_byte (Wire *w)
{
  return get_digit (w) << 4 | get_digit (w);
}

static void
ascii_w_byte (Wire *w, void *v)
{
  SANE_Byte *b = v;

  switch (w->direction)
    {
    case WIRE_ENCODE:
      sanei_w_space (w, 3);
      *w->buffer.curr++ = hexdigit[(*b >> 4) & 0x0f];
      *w->buffer.curr++ = hexdigit[(*b >> 0) & 0x0f];
      *w->buffer.curr++ = '\n';
      break;

    case WIRE_DECODE:
      skip_ws (w);
      *b = get_byte (w);
      break;

    case WIRE_FREE:
      break;
    }
}

static void
ascii_w_char (Wire *w, void *v)
{
  SANE_Char *c = v;

  switch (w->direction)
    {
    case WIRE_ENCODE:
      sanei_w_space (w, 5);
      *w->buffer.curr++ = '\'';
      if (*c == '\'' || *c == '\\')
	*w->buffer.curr++ = '\\';
      *w->buffer.curr++ = *c;
      *w->buffer.curr++ = '\'';
      *w->buffer.curr++ = '\n';
      break;

    case WIRE_DECODE:
      sanei_w_space (w, 4);
      if (*w->buffer.curr++ != '\'')
	{
	  w->status = EINVAL;
	  return;
	}
      *c = *w->buffer.curr++;
      if (*c == '\\')
	{
	  sanei_w_space (w, 2);
	  *c = *w->buffer.curr++;
	}
      if (*w->buffer.curr++ != '\'')
	{
	  w->status = EINVAL;
	  return;
	}
      break;

    case WIRE_FREE:
      break;
    }
}

static void
ascii_w_string (Wire *w, void *v)
{
  size_t len, alloced_len;
  SANE_String *s = v;
  char * str, ch;
  int done;

  switch (w->direction)
    {
    case WIRE_ENCODE:
      if (*s)
	{
	  sanei_w_space (w, 1);
	  *w->buffer.curr++ = '"';
	  str = *s;
	  while ((ch = *str++))
	    {
	      sanei_w_space (w, 2);
	      if (ch == '"' || ch == '\\')
		*w->buffer.curr++ = '\\';
	      *w->buffer.curr++ = ch;
	    }
	  *w->buffer.curr++ = '"';
	}
      else
	{
	  sanei_w_space (w, 5);
	  *w->buffer.curr++ = '(';
	  *w->buffer.curr++ = 'n';
	  *w->buffer.curr++ = 'i';
	  *w->buffer.curr++ = 'l';
	  *w->buffer.curr++ = ')';
	}
      sanei_w_space (w, 1);
      *w->buffer.curr++ = '\n';
      break;

    case WIRE_DECODE:
      skip_ws (w);
      sanei_w_space (w, 1);
      ch = *w->buffer.curr++;
      if (ch == '"')
	{
	  alloced_len = len = 0;
	  str = 0;
	  done = 0;
	  do
	    {
	      sanei_w_space (w, 1);
	      if (w->status != 0)
		return;

	      ch = *w->buffer.curr++;
	      if (ch == '"')
		done = 1;

	      if (ch == '\\')
		{
		  sanei_w_space (w, 1);
		  ch = *w->buffer.curr++;
		}

	      if (len >= alloced_len)
		{
		  alloced_len += 1024;
		  if (!str)
		    str = malloc (alloced_len);
		  else
		    str = realloc (str, alloced_len);

		  if (str == 0)
		    {
		      /* Malloc failed, so return an error. */
		      w->status = ENOMEM;
		      return;
		    }
		}
	      str[len++] = ch;
	    }
	  while (!done);

	  str[len - 1] = '\0';
	  *s = realloc (str, len);

	  if (*s == 0)
	    {
	      /* Malloc failed, so return an error. */
	      w->status = ENOMEM;
	      return;
	    }
	}
      else if (ch == '(')
	{
	  sanei_w_space (w, 4);
	  if (   *w->buffer.curr++ != 'n'
	      || *w->buffer.curr++ != 'i'
	      || *w->buffer.curr++ != 'l'
	      || *w->buffer.curr++ != ')')
	    {
	      w->status = EINVAL;
	      return;
	    }
	  *s = 0;
	}
      else
	{
	  w->status = EINVAL;
	  return;
	}
      break;

    case WIRE_FREE:
      if (*s)
	free (*s);
      break;
    }
}

static void
ascii_w_word (Wire *w, void *v)
{
  SANE_Word val, *word = v;
  int i, is_negative = 0;
  char buf[16];

  switch (w->direction)
    {
    case WIRE_ENCODE:
      val = *word;
      i = sizeof (buf) - 1;
      if (val < 0)
	{
	  is_negative = 1;
	  val = -val;
	}
      do
	{
	  buf[i--] = '0' + (val % 10);
	  val /= 10;
	}
      while (val);
      if (is_negative)
	buf[i--] = '-';

      sanei_w_space (w, sizeof (buf) - i);
      memcpy (w->buffer.curr, buf + i + 1, sizeof (buf) - i - 1);
      w->buffer.curr += sizeof (buf) - i - 1;
      *w->buffer.curr++ = '\n';
      break;

    case WIRE_DECODE:
      skip_ws (w);
      val = 0;
      sanei_w_space (w, 1);
      if (*w->buffer.curr == '-')
	{
	  is_negative = 1;
	  ++w->buffer.curr;
	}
      while (1)
	{
	  sanei_w_space (w, 1);
	  if (w->status != 0)
	    return;

	  if (!isdigit (*w->buffer.curr))
	    break;

	  val = 10*val + (*w->buffer.curr++ - '0');
	}
      *word = is_negative ? -val : val;
      break;

    case WIRE_FREE:
      break;
    }
}

void
sanei_codec_ascii_init (Wire *w)
{
  w->codec.w_byte = ascii_w_byte;
  w->codec.w_char = ascii_w_char;
  w->codec.w_word = ascii_w_word;
  w->codec.w_string = ascii_w_string;
}
