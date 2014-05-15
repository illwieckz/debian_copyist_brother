/* sane - Scanner Access Now Easy.
   Copyright (C) 1998 David Mosberger
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

   This file provides generic configuration support.  */

#include "sane/config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sane/sanei.h"
#include "sane/sanei_config.h"
#include "sane/sanei_scsi.h"

/* This logically belongs to sanei_config.c but not every backend that
   uses sanei_config() wants to depend on this function.  */

void
sanei_config_attach_matching_devices (const char *name,
				      SANE_Status (*attach) (const char *dev))
{
  int bus = -1, channel = -1, id = -1, lun = -1;
  char *vendor = 0, *model = 0, *type = 0, *end;

  if (strncmp (name, "scsi", 4) == 0)
    {
      name += 4;

      name = sanei_config_skip_whitespace (name);
      if (*name)
	{
	  name = sanei_config_get_string (name, &vendor);
	  if (strcmp (vendor, "*") == 0)
	    {
	      free (vendor);
	      vendor = 0;
	    }
	  name = sanei_config_skip_whitespace (name);
	}

      name = sanei_config_skip_whitespace (name);
      if (*name)
	{
	  name = sanei_config_get_string (name, &model);
	  if (strcmp (model, "*") == 0)
	    {
	      free (model);
	      model = 0;
	    }
	  name = sanei_config_skip_whitespace (name);
	}

      name = sanei_config_skip_whitespace (name);
      if (*name)
	{
	  name = sanei_config_get_string (name, &type);
	  if (strcmp (type, "*") == 0)
	    {
	      free (type);
	      type = 0;
	    }
	  name = sanei_config_skip_whitespace (name);
	}

      if (isdigit (*name))
	{
	  bus = strtol (name, &end, 10);
	  name = sanei_config_skip_whitespace (end);
	}
      else if (*name == '*')
	name = sanei_config_skip_whitespace (++name);

      if (isdigit (*name))
	{
	  channel = strtol (name, &end, 10);
	  name = sanei_config_skip_whitespace (end);
	}
      else if (*name == '*')
	name = sanei_config_skip_whitespace (++name);

      if (isdigit (*name))
	{
	  id = strtol (name, &end, 10);
	  name = sanei_config_skip_whitespace (end);
	}
      else if (*name == '*')
	name = sanei_config_skip_whitespace (++name);

      if (isdigit (*name))
	{
	  lun = strtol (name, &end, 10);
	  name = sanei_config_skip_whitespace (end);
	}
      else if (*name == '*')
	name = sanei_config_skip_whitespace (++name);

      sanei_scsi_find_devices (vendor, model, type, bus, channel, id, lun,
			       attach);

      if (vendor)
	free (vendor);
      if (model)
	free (model);
      if (type)
	free (type);
    }
  else 
    (*attach) (name);
}
