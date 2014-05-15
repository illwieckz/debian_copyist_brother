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

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#ifdef HAVE_LIBC_H
# include <libc.h>	/* NeXTStep/OpenStep */
#endif

#include "sane/sane.h"
#include "sane/sanei.h"
#include "sane/sanei_wire.h"
#include "sane/sanei_codec_ascii.h"

int
sanei_save_values (int fd, SANE_Handle device)
{
  const SANE_Option_Descriptor *opt;
  size_t word_array_size = 0;
  SANE_Word *word_array = 0;
  size_t str_size = 0;
  SANE_String str = 0;
  SANE_Word word;
  Wire w;
  int i;

  w.io.fd = fd;
  w.io.read = read;
  w.io.write = write;
  sanei_w_init (&w, sanei_codec_ascii_init);
  sanei_w_set_dir (&w, WIRE_ENCODE);

  for (i = 0; (opt = sane_get_option_descriptor (device, i)); ++i)
    {
      if ((opt->cap & (SANE_CAP_SOFT_SELECT | SANE_CAP_SOFT_DETECT))
	  !=  (SANE_CAP_SOFT_SELECT | SANE_CAP_SOFT_DETECT)
	  || !opt->name)
	/* if we can't query AND set the option, don't bother saving it */
	continue;

      switch (opt->type)
	{
	case SANE_TYPE_BOOL:
	case SANE_TYPE_INT:
	case SANE_TYPE_FIXED:
	  if (opt->size == sizeof (SANE_Word))
	    {
	      if (sane_control_option (device, i, SANE_ACTION_GET_VALUE,
				       &word, 0)
		  != SANE_STATUS_GOOD)
		continue;
	      sanei_w_string (&w, (SANE_String *) &opt->name);
	      sanei_w_word (&w, &word);
	    }
	  else
	    {
	      SANE_Int len = opt->size / sizeof (SANE_Word);

	      if (opt->size > word_array_size)
		{
		  word_array_size =
		    ((opt->size + 32*sizeof (SANE_Word))
		     & ~(32*sizeof (SANE_Word) - 1));
		  if (word_array)
		    word_array = realloc (word_array, word_array_size);
		  else
		    word_array = malloc (word_array_size);

		  if (word_array == 0)
		    {
		      /* Malloc failed, so return an error. */
		      w.status = ENOMEM;
		      return 1;
		    }
		}
	      if (sane_control_option (device, i, SANE_ACTION_GET_VALUE,
				       word_array, 0)
		  != SANE_STATUS_GOOD)
		continue;
	      sanei_w_string (&w, (SANE_String *) &opt->name);
	      sanei_w_array (&w, &len, (void **) &word_array,
			     (WireCodecFunc) sanei_w_word, sizeof (SANE_Word));
	    }
	  break;

	case SANE_TYPE_STRING:
	  if (opt->size > str_size)
	    {
	      str_size = (opt->size + 1024) & ~1023;
	      if (str)
		str = realloc (str, str_size);
	      else
		str = malloc (str_size);

	      if (str == 0)
		{
		  /* Malloc failed, so return an error. */
		  w.status = ENOMEM;
		  return 1;
		}
	    }
	  if (sane_control_option (device, i, SANE_ACTION_GET_VALUE, str, 0)
	      != SANE_STATUS_GOOD)
	    continue;
	  sanei_w_string (&w, (SANE_String *) &opt->name);
	  sanei_w_string (&w, &str);
	  break;

	case SANE_TYPE_BUTTON:
	case SANE_TYPE_GROUP:
	  break;
	}
    }
  sanei_w_set_dir (&w, WIRE_DECODE);

  if (word_array)
    free (word_array);
  if (str)
    free (str);
  return 0;
}
