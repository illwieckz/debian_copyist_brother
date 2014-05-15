/* sane - Scanner Access Now Easy.
   Copyright (C) 1997 Jeffrey S. Freedman
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

#include  "sane/config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/param.h>

#include "sane/sanei.h"
#include "sane/sanei_config.h"

#define BACKEND_NAME	sanei_config
#include "sane/sanei_debug.h"

#ifndef PATH_MAX
# define PATH_MAX	1024
#endif

#if defined(HAVE_OS2_H)
# define DIR_SEP	";"
# define PATH_SEP	'\\'
#else
# define DIR_SEP	":"
# define PATH_SEP	'/'
#endif

#define DEFAULT_DIRS	"." DIR_SEP STRINGIFY(PATH_SANE_CONFIG_DIR)

static const char *dir_list;

FILE *
sanei_config_open (const char *filename)
{
  char *copy, *next, *dir, result[PATH_MAX];
  FILE *fp = 0;
  size_t len;
  void *mem = 0;

  if (!dir_list)
    {
      DBG_INIT();

      dir_list = getenv ("SANE_CONFIG_DIR");
      if (dir_list)
	{
	  len = strlen (dir_list);
	  if ((len > 0) && (dir_list[len - 1] == DIR_SEP[0]))
	    {
	      /* append default search directories: */
	      mem = malloc (len + sizeof (DEFAULT_DIRS));

	      memcpy (mem, dir_list, len);
	      memcpy ((char *) mem + len, DEFAULT_DIRS, sizeof (DEFAULT_DIRS));
	      dir_list = mem;
	    }
	}
      else
	dir_list = DEFAULT_DIRS;
    }

  copy = strdup (dir_list);

  if (mem)
    free(mem);

  for (next = copy; (dir = strsep (&next, DIR_SEP)) != 0; )
    {
      snprintf (result, sizeof (result), "%s%c%s", dir, PATH_SEP, filename);
      DBG(4, "sanei_config_open: attempting to open `%s'\n", result);
      fp = fopen (result, "r");
      if (fp)
	{
	  DBG(3, "sanei_config_open: using file `%s'\n", result);
	  break;
	}
    }
  free (copy);

  if (!fp)
    DBG(2, "sanei_config_open: could not find config file `%s'\n", filename);

  return fp;
}

const char *
sanei_config_skip_whitespace (const char *str)
{
  while (str && *str && isspace (*str))
    ++str;
  return str;
}

const char *
sanei_config_get_string (const char *str, char **string_const)
{
  const char *start;
  size_t len;

  str = sanei_config_skip_whitespace (str);

  if (*str == '"')
    {
      start = ++str;
      while (*str && *str != '"')
	++str;
      len = str - start;
      if (*str == '"')
	++str;
      else
	start = 0;		/* final double quote is missing */
    }
  else
    {
      start = str;
      while (*str && !isspace (*str))
	++str;
      len = str - start;
    }
  if (start)
    *string_const = strndup (start, len);
  return str;
}

char *
sanei_config_read (char *str, int n, FILE *stream)
{
   char* rc;
   char* start;
   int   len;

      /* read line from stream */
   rc = fgets( str, n, stream);
   if (rc == NULL)
      return NULL;

      /* remove ending whitespaces */
   len = strlen( str);
   while( (0 < len) && (isspace( str[--len])) )
      str[len] = '\0';

      /* remove starting whitespaces */
   start = str;
   while( isspace( *start))
      start++;

   if (start != str) 
      do {
         *str++ = *start++;
      } while( *str);

   return rc;
}
