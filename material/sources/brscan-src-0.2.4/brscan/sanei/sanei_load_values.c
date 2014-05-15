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
   If you do not wish that, delete this exception notice.

   This file implements a routine to restore option values saved to a
   file (using sanei_save_values()).  This is a bit tricky since
   setting an option may change the availability of other options.
   The problem is that we don't know the order of the set-value calls
   that resulted in the saved set of option values.  One solution
   might be to simply keep setting all option values until we no
   longer get any changes to the option value set.  However, that has
   the potential for live-lock.  Instead, we keep track of what
   options caused a SANE_INFO_RELOAD_OPTIONS.  For such options, their
   value is set exactly once.  This guarantees convergence after a
   bounded (and usually small) number of iterations.  The resulting
   value set is guaranteed to be the desired (saved) one as long as
   setting an option that affects availability of other options does
   not "lose" its value by setting another option.  I don't think any
   sane backend would do this and since this is SANE, we just proved
   that this algorithm works perfectly.  */

#ifdef _AIX
# include <lalloca.h>	/* MUST come first for AIX! */
#endif

#include "sane/config.h"
#include <lalloca.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_LIBC_H
# include <libc.h>	/* NeXTStep/OpenStep */
#endif

#include "sane/sane.h"
#include "sane/sanei.h"
#include "sane/sanei_wire.h"
#include "sane/sanei_codec_ascii.h"

#define BITS_PER_LONG	(8*sizeof (u_long))

#define SET(set, bit) \
  ((set)[(bit)/BITS_PER_LONG] |= (1UL << (bit)%BITS_PER_LONG))
#define IS_SET(set, bit) \
  (((set)[(bit)/BITS_PER_LONG] & (1UL << (bit)%BITS_PER_LONG)) != 0)

int
sanei_load_values (int fd, SANE_Handle device)
{
  const SANE_Option_Descriptor *opt;
  SANE_Word *word_array;
  SANE_String name, str;
  u_long *caused_reload;
  SANE_Int num_options;
  SANE_Status status;
  int i, keep_going;
  SANE_Word word;
  SANE_Int info;
  off_t offset;
  size_t size;
  char *buf;
  Wire w;

  offset = lseek (fd, 0, SEEK_CUR);
  w.io.fd = fd;
  w.io.read = read;
  w.io.write = write;
  sanei_w_init (&w, sanei_codec_ascii_init);
  sanei_w_set_dir (&w, WIRE_DECODE);
  keep_going = 0;

  sane_control_option (device, 0, SANE_ACTION_GET_VALUE, &num_options, 0);
  size = (num_options + BITS_PER_LONG - 1) / BITS_PER_LONG * sizeof (long);
  caused_reload = alloca (size);
  memset (caused_reload, 0, size);

  while (1)
    {
      sanei_w_space (&w, 3);

      if (!w.status)
	sanei_w_string (&w, &name);

      if (w.status)
	{
	  if (keep_going)
	    {
	      lseek (fd, offset, SEEK_SET);
	      sanei_w_set_dir (&w, WIRE_DECODE);
	      keep_going = 0;
	      continue;
	    }
	  return 0;
	}

      status = SANE_STATUS_GOOD;
      info = 0;
      for (i = 1; (opt = sane_get_option_descriptor (device, i)); ++i)
	{
	  if (!opt->name || strcmp (opt->name, name) != 0)
	    continue;

	  if (IS_SET(caused_reload, i))
	    continue;

	  switch (opt->type)
	    {
	    case SANE_TYPE_BOOL:
	    case SANE_TYPE_INT:
	    case SANE_TYPE_FIXED:
	      if (opt->size == sizeof (SANE_Word))
		{
		  sanei_w_word (&w, &word);
		  status = sane_control_option (device, i,
						SANE_ACTION_SET_VALUE,
						&word, &info);
		}
	      else
		{
		  SANE_Int len;

		  sanei_w_array (&w, &len, (void **) &word_array,
				 (WireCodecFunc) sanei_w_word,
				 sizeof (SANE_Word));
		  status = sane_control_option (device, i,
						SANE_ACTION_SET_VALUE,
						word_array, &info);
		  w.direction = WIRE_FREE;
		  sanei_w_array (&w, &len, (void **) &word_array,
				 (WireCodecFunc) sanei_w_word,
				 sizeof (SANE_Word));
		  w.direction = WIRE_DECODE;
		}
	      break;

	    case SANE_TYPE_STRING:
	      sanei_w_string (&w, &str);
	      buf = malloc (opt->size);
	      strncpy (buf, str, opt->size);
	      buf[opt->size - 1] = '\0';
	      sanei_w_free (&w, (WireCodecFunc) sanei_w_string, &str);

	      status = sane_control_option (device, i, SANE_ACTION_SET_VALUE,
					    buf, &info);
	      break;

	    case SANE_TYPE_BUTTON:
	    case SANE_TYPE_GROUP:
	      break;
	    }
	  break;
	}
      sanei_w_free (&w, (WireCodecFunc) sanei_w_string, &name);

      if (status == SANE_STATUS_GOOD && (info & SANE_INFO_RELOAD_OPTIONS))
	{
	  SET (caused_reload, i);
	  keep_going = 1;
	}
    }
  return 0;
}
