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
   If you do not wish that, delete this exception notice.  */

#include "sane/config.h"

#include <string.h>

#include <sys/types.h>

#include "sane/sane.h"
#include "sane/sanei.h"

SANE_Status
sanei_constrain_value (const SANE_Option_Descriptor * opt, void * value,
		       SANE_Word * info)
{
  const SANE_String_Const * string_list;
  const SANE_Word * word_list;
  int i, num_matches, match;
  const SANE_Range * range;
  SANE_Word w, v;
  size_t len;

  switch (opt->constraint_type)
    {
    case SANE_CONSTRAINT_RANGE:
      w = *(SANE_Word *) value;
      range = opt->constraint.range;

      if (w < range->min)
      {
        *(SANE_Word *) value = range->min;
        if (info)
        {
          *info |= SANE_INFO_INEXACT;
        }
      }

      if (w > range->max)
      {
        *(SANE_Word *) value = range->max;
        if (info)
        {
          *info |= SANE_INFO_INEXACT;
        }
      }

      w = *(SANE_Word *) value;

      if (range->quant)
	{
	  v = (w - range->min + range->quant/2) / range->quant;
	  v = v * range->quant + range->min;
	  if (v != w)
	    {
	      *(SANE_Word *) value = v;
	      if (info)
		*info |= SANE_INFO_INEXACT;
	    }
	}
      break;

    case SANE_CONSTRAINT_WORD_LIST:
      w = *(SANE_Word *) value;
      word_list = opt->constraint.word_list;
      for (i = 1; w != word_list[i]; ++i)
	if (i >= word_list[0])
	  return SANE_STATUS_INVAL;
      break;

    case SANE_CONSTRAINT_STRING_LIST:
      /* Matching algorithm: take the longest unique match ignoring
	 case.  If there is an exact match, it is admissible even if
	 the same string is a prefix of a longer option name. */
      string_list = opt->constraint.string_list;
      len = strlen (value);

      /* count how many matches of length LEN characters we have: */
      num_matches = 0;
      match = -1;
      for (i = 0; string_list[i]; ++i)
	if (strncasecmp (value, string_list[i], len) == 0
	    && len <= strlen (string_list[i]))
	  {
	    match = i;
	    if (len == strlen (string_list[i]))
	      {
		/* exact match... */
		if (strcmp (value, string_list[i]) != 0)
		  /* ...but case differs */
		  strcpy (value, string_list[match]);
		return SANE_STATUS_GOOD;
	      }
	    ++num_matches;
	  }

      if (num_matches > 1)
	return SANE_STATUS_INVAL;
      else if (num_matches == 1)
	{
	  strcpy (value, string_list[match]);
	  return SANE_STATUS_GOOD;
	}
      return SANE_STATUS_INVAL;

    default:
      break;
    }
  return SANE_STATUS_GOOD;
}
