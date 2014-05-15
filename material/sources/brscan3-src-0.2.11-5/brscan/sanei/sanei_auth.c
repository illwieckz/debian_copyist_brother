/* sane - Scanner Access Now Easy.
   Copyright (C) 2000 Jochen Eisinger <jochen.eisinger@gmx.net>
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

   This file implements an interface for user authorization using MD5 digest */

#include "../include/sane/config.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <time.h>


#define BACKEND_NAME sanei_auth
#include "../include/sane/sanei_backend.h"
#include "../include/sane/sanei_debug.h"

#include "../include/sane/sane.h"
#include "../include/sane/sanei.h"
#include "../include/sane/sanei_auth.h"
#include "../include/sane/sanei_config.h"

#include "../include/md5.h"

static int random_seeded = 0;

#define INIT_RND()	do { 					\
				if (random_seeded == 0)	{	\
					random_seeded = 1;	\
					srand(time(NULL));	\
					DBG_INIT();		\
				}				\
			} while (0)


#ifdef HAVE_DEV_URANDOM

static unsigned long
randombits (void)
{

  FILE *dev_urandom;
  unsigned long result = 0;
  char buffer[4];

  if ((dev_urandom = fopen ("/dev/urandom", "r")) == NULL)
    {
      DBG (2, "randombits: could not open /dev/urandom...\n");
      return rand ();
    }

  fread (buffer, 1, 4, dev_urandom);

  fclose (dev_urandom);

  result = buffer[0];
  result <<= 8;
  result += buffer[1];
  result <<= 8;
  result += buffer[2];
  result <<= 8;
  result += buffer[3];

  return result;

}

#else

#define	randombits	rand

#endif


static int
check_passwd (const char *upassword,
	      const char *password,
	      const char *randomstring, const char *username)
{

  unsigned char md5digest[16];
  char tmpstr[512];

  if (strncmp (upassword, "$MD5$", 5) == 0)
    {

      sprintf (tmpstr, "%s%.128s",
	       strstr (randomstring, "$MD5$") + 5, password);
      md5_buffer (tmpstr, strlen (tmpstr), md5digest);

      sprintf (tmpstr, "$MD5$%02x%02x%02x%02x%02x%02x%02x%02x"
	       "%02x%02x%02x%02x%02x%02x%02x%02x",
	       md5digest[0], md5digest[1],
	       md5digest[2], md5digest[3],
	       md5digest[4], md5digest[5],
	       md5digest[6], md5digest[7],
	       md5digest[8], md5digest[9],
	       md5digest[10], md5digest[11],
	       md5digest[12], md5digest[13], md5digest[14], md5digest[15]);


      return (strcmp (upassword, tmpstr) == 0);

    }
  else
    {

      DBG (1, "check_passwd: received plain-text reply from user ``%s''\n",
	   username);

      return (strcmp (upassword, password) == 0);

    }
}


SANE_Status
sanei_authorize (const char *resource,
		 const char *backend, SANE_Auth_Callback authorize)
{
  FILE *passwd_file;
  char passwd_filename[256];
  char line[1024], *linep;
  SANE_Bool entry_found = SANE_FALSE;
  char md5resource[256];
  char username[SANE_MAX_USERNAME_LEN];
  char password[SANE_MAX_PASSWORD_LEN];

  INIT_RND ();

  DBG (4, "called for ``%s'' by %s\n", resource, backend);

  if (strlen (resource) > 127)
    DBG (1, "resource is longer than 127 chars...\n");

  sprintf (passwd_filename, "%s.users", backend);

  passwd_file = sanei_config_open (passwd_filename);

  if (passwd_file == NULL)
    {
      DBG (3, "could not open ``%s''...\n", passwd_filename);
      return SANE_STATUS_GOOD;
    }

  while (sanei_config_read (line, 1024, passwd_file))
    {

      if ((strlen (line) > 0) && (line[strlen (line) - 1] == '\n'))
	line[strlen (line) - 1] = '\n';

      if ((strlen (line) > 0) && (line[strlen (line) - 1] == '\r'))
	line[strlen (line) - 1] = '\r';

      if (strchr (line, ':') != NULL)
	{
	  if (strchr (strchr (line, ':') + 1, ':') != NULL)
	    {

	      if (strcmp (strchr (strchr (line, ':') + 1, ':') + 1, resource)
		  == 0)

		{



		  entry_found = SANE_TRUE;
		  break;

		}
	    }

	}

    }

  if (entry_found == SANE_FALSE)
    {

      fclose (passwd_file);

      DBG (3, "could not find resource ``%s''...\n", resource);
      return SANE_STATUS_GOOD;

    }

  if (authorize == NULL)
    {
      DBG (2, "no authorization callback supplied by frontend\n");
      return SANE_STATUS_ACCESS_DENIED;
    }

  sprintf (md5resource, "%.128s$MD5$%x%lx%08lx",
	   resource, getpid (), time (NULL), randombits ());

  memset (username, 0, SANE_MAX_USERNAME_LEN);
  memset (password, 0, SANE_MAX_PASSWORD_LEN);

  (*authorize) (md5resource, username, password);


  fseek (passwd_file, 0L, SEEK_SET);

  while (sanei_config_read (line, 1024, passwd_file))
    {

      if ((strlen (line) > 0) && (line[strlen (line) - 1] == '\n'))
	line[strlen (line) - 1] = '\n';

      if ((strlen (line) > 0) && (line[strlen (line) - 1] == '\r'))
	line[strlen (line) - 1] = '\r';


      if ((strncmp (line, username, strlen (username)) == 0) &&
	  (((strchr (line, ':')) - line) == (signed) strlen (username)))
	{

	  linep = strchr (line, ':') + 1;

	  if ((strchr (linep, ':') != NULL)
	      && (strcmp (strchr (linep, ':') + 1, resource) == 0))
	    {

	      *(strchr (linep, ':')) = 0;


	      if (check_passwd (password, linep, md5resource, username))
		{
		  fclose (passwd_file);
		  DBG (2, "authorization succeeded\n");
		  return SANE_STATUS_GOOD;
		}
	    }
	}


    }

  fclose (passwd_file);

  DBG (1, "authorization failed\n");

  return SANE_STATUS_ACCESS_DENIED;
}
