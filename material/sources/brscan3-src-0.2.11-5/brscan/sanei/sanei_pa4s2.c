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

   This file implements an interface for the Mustek PP chipset A4S2 */


/* debug levels:
   0 - nothing
   1 - errors
   2 - warnings
   3 - things nice to know
   4 - code flow
   5 - detailed flow
   6 - everything 

   These debug levels can be set using the envirnment variable
   SANE_DEBUG_SANEI_PA4S2 */

#include "sane/config.h"

#define BACKEND_NAME sanei_pa4s2
#include "sane/sanei_backend.h"	/* pick up compatibility defs */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_IO_H
#include <sys/io.h>
#elif HAVE_ASM_IO_H
#include <asm/io.h>		/* ugly, but backwards compatible */
#elif HAVE_SYS_HW_H
#include <sys/hw.h>
#elif defined(__i386__)  && defined (__GNUC__)

static __inline__ void
outb (u_char value, u_long port)
{
  __asm__ __volatile__ ("outb %0,%1"::"a" (value), "d" ((u_short) port));
}

static __inline__ u_char
inb (u_long port)
{
  u_char value;

  __asm__ __volatile__ ("inb %1,%0":"=a" (value):"d" ((u_short) port));
  return value;
}

#else
#define IO_SUPPORT_MISSING
#endif

#include "sane/sane.h"
#include "sane/sanei.h"
#include "sane/sanei_pa4s2.h"


#ifdef NDEBUG
#define DBG_INIT()  /* basically, this is already done in sanei_debug.h... */

#define TEST_DBG_INIT()

#else /* !NDEBUG */

static int sanei_pa4s2_dbg_init_called = SANE_FALSE;

#if (!defined __GNUC__ || __GNUC__ < 2 || \
     __GNUC_MINOR__ < (defined __cplusplus ? 6 : 4))

#define TEST_DBG_INIT() if (sanei_pa4s2_dbg_init_called == SANE_FALSE) \
                          {                                            \
                            DBG_INIT();                                \
                            DBG(6, "sanei_pa4s2: interface called for" \
                              " the first time\n");                    \
                            sanei_pa4s2_dbg_init_called = SANE_TRUE;   \
                          }
#else

#define TEST_DBG_INIT() if (sanei_pa4s2_dbg_init_called == SANE_FALSE)   \
                          {                                              \
                            DBG_INIT();                                  \
                            DBG(6, "%s: interface called for"            \
                              " the first time\n", __PRETTY_FUNCTION__); \
                            sanei_pa4s2_dbg_init_called = SANE_TRUE;     \
                          }

#endif

#endif /* NDEBUG */


#if defined (HAVE_IOPERM) && !defined (IO_SUPPORT_MISSING)

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "sane/saneopts.h"

#define PA4S2_MODE_NIB	0
#define PA4S2_MODE_UNI	1
#define PA4S2_MODE_EPP	2

#define PA4S2_ASIC_ID_1013	0xA8
#define PA4S2_ASIC_ID_1015	0xA5
#define PA4S2_ASIC_ID_1505	0xA2

typedef struct
  {
    u_long base;		/* i/o base address */
    u_int in_use;		/* port in use? */
    u_int enabled;		/* port enabled? */
    u_int mode;			/* protocoll */
    u_char prelock[3];		/* state of port */
  }
PortRec, *Port;


static PortRec port[] =
{
  {0x378, SANE_FALSE, SANE_FALSE, PA4S2_MODE_NIB,
   {0, 0, 0}},
  {0x278, SANE_FALSE, SANE_FALSE, PA4S2_MODE_NIB,
   {0, 0, 0}},
  {0x3BC, SANE_FALSE, SANE_FALSE, PA4S2_MODE_NIB,
   {0, 0, 0}}};

static u_int sanei_pa4s2_interface_options = SANEI_PA4S2_OPT_DEFAULT;

extern int setuid (uid_t);	/* should also be in unistd.h */

static int pa4s2_open (const char *dev, SANE_Status * status);
static void pa4s2_readbegin_epp (u_int base, u_char reg);
static u_char pa4s2_readbyte_epp (u_int base);
static void pa4s2_readend_epp (u_int base);
static void pa4s2_readbegin_uni (u_int base, u_char reg);
static u_char pa4s2_readbyte_uni (u_int base);
static void pa4s2_readend_uni (u_int base);
static void pa4s2_readbegin_nib (u_int base, u_char reg);
static u_char pa4s2_readbyte_nib (u_int base);
static void pa4s2_readend_nib (u_int base);
static void pa4s2_writebyte_any (u_int base, u_char reg, u_char val);
static void pa4s2_enable (u_int base, u_char * prelock);
static void pa4s2_disable (u_int base, u_char * prelock);
static int pa4s2_close (int fd, SANE_Status * status);

static int
pa4s2_open (const char *dev, SANE_Status * status)
{

  static int first_time = SANE_TRUE;
  u_long base;
  int n;

  DBG (4, "pa4s2_open: trying to attach dev `%s`\n", dev);
  DBG (6, "pa4s2_open: static int first_time = %u\n", first_time);

  if (first_time == SANE_TRUE)
    {

      DBG (5, "pa4s2_open: called for the first time\n");

      first_time = SANE_FALSE;

      DBG (4, "pa4s2_open: trying to setuid root\n");

      if (0 > setuid (0))
	{

	  DBG (1, "pa4s2_open: setuid failed: errno = %d\n",
	       errno);
	  DBG (5, "pa4s2_open: returning SANE_STATUS_INVAL\n");

	  *status = SANE_STATUS_INVAL;
	  return -1;

	}

      DBG (3, "pa4s2_open: the application is now root\n");
      DBG (3, "pa4s2_open: this is a high security risk...\n");

      DBG (6, "pa4s2_open: ... you'd better start praying\n");

    }

  {
    char *end;

    DBG (5, "pa4s2_open: reading port number\n");

    base = strtol (dev, &end, 0);

    if ((end == dev) || (*end != '\0'))
      {

	DBG (1, "pa4s2_open: `%s` is not a valid port number\n",
	     dev);
	DBG (6, "pa4s2_open: the part I did not understand"
	     " was ...`%s`\n", end);
	DBG (5, "pa4s2_open: returning SANE_STATUS_INVAL\n");

	*status = SANE_STATUS_INVAL;

	return -1;

      }

  }

  DBG (6, "pa4s2_open: read port number 0x%03lx\n", base);

  if (base == 0)
    {

      DBG (1, "pa4s2_open: 0x%03lx is not a valid base address\n",
	   base);
      DBG (5, "pa4s2_open: returning SANE_STATUS_INVAL\n");

      *status = SANE_STATUS_INVAL;
      return -1;

    }

  DBG (5, "pa4s2_open: looking up port in list\n");

  for (n = 0; n < NELEMS (port); n++)
    if (port[n].base == base)
      break;

  if (NELEMS (port) <= n)
    {

      DBG (1, "pa4s2_open: 0x%03lx is not a valid base address\n",
	   base);
      DBG (5, "pa4s2_open: returning SANE_STATUS_INVAL\n");

      *status = SANE_STATUS_INVAL;
      return -1;
    }

  DBG (6, "pa4s2_open: port is in list at port[%d]\n", n);

  if (port[n].in_use == SANE_TRUE)
    {

      DBG (1, "pa4s2_open: port 0x%03lx is already in use\n", base);
      DBG (5, "pa4s2_open: returning SANE_STATUS_DEVICE_BUSY\n");

      *status = SANE_STATUS_DEVICE_BUSY;
      return -1;

    }

  DBG (5, "pa4s2_open: setting up port data\n");

  DBG (6, "pa4s2_open: base=0x%03lx in_use=SANE_TRUE\n", base);
  DBG (6, "pa4s2_open: enabled=SANE_FALSE mode=PA4S2_MODE_NIB\n");

  port[n].base = base;
  port[n].in_use = SANE_TRUE;
  port[n].enabled = SANE_FALSE;
  port[n].mode = PA4S2_MODE_NIB;

  DBG (5, "pa4s2_open: getting io permissions\n");

  if (ioperm (port[n].base, 5, 1))
    {

      DBG (1, "pa4s2_open: cannot get io privilege for port"
	   " 0x%03lx\n", port[n].base);


      DBG (5, "pa4s2_open: marking port[%d] as unused\n", n);
      port[n].in_use = SANE_FALSE;

      DBG (5, "pa4s2_open: returning SANE_STATUS_IO_ERROR\n");
      *status = SANE_STATUS_IO_ERROR;
      return -1;

    }

  DBG (3, "pa4s2_open: device `%s` opened...\n", dev);

  DBG (5, "pa4s2_open: returning SANE_STATUS_GOOD\n");
  *status = SANE_STATUS_GOOD;

  DBG (4, "pa4s2_open: open dev `%s` as fd %u\n", dev, n);

  return n;

}


static void
pa4s2_readbegin_epp (u_int base, u_char reg)
{

  DBG (6, "pa4s2_readbegin_epp: selecting register %u at 0x%03x\n",
       (int) reg, base);

  outb (0x20, base);
  outb (0x04, base + 2);
  outb (0x06, base + 2);
  outb (0x04, base + 2);
  outb (reg + 0x18, base + 3);

}

static u_char
pa4s2_readbyte_epp (u_int base)
{

  u_char val = inb (base + 4);

  DBG (6, "pa4s2_readbyte_epp: reading value 0x%02x at 0x%03x\n",
       (int) val, base);

  return val;

}

static void
pa4s2_readend_epp (u_int base)
{

  DBG (6, "pa4s2_readend_epp: end of reading sequence\n");

  outb (0x04, base + 2);
  outb (0x00, base + 2);
  outb (0x04, base + 2);

}

static void
pa4s2_readbegin_uni (u_int base, u_char reg)
{

  DBG (6, "pa4s2_readbegin_uni: selecting register %u at 0x%03x\n",
       (int) reg, base);

  outb (reg | 0x58, base + 0);
  outb (0x04, base + 2);
  outb (0x06, base + 2);
  outb (0x04, base + 2);
  outb (0x04, base + 2);

}

static u_char
pa4s2_readbyte_uni (u_int base)
{
  u_char val;

  outb (0x05, base + 2);
  val = inb (base + 2);
  val <<= 4;
  val &= 0xE0;
  val |= (inb (base + 1) >> 3);
  outb (0x04, base + 2);

  DBG (6, "pa4s2_readbyte_uni: reading value 0x%02x at 0x%03x\n",
       (int) val, base);

  return val;
}

static void
pa4s2_readend_uni (u_int base)
{

  DBG (6, "pa4s2_readend_uni: end of reading sequence\n");
  DBG (129, "unused base 0x%03X\n", base);

}

static void
pa4s2_readbegin_nib (u_int base, u_char reg)
{

  DBG (6, "pa4s2_readbegin_nib: selecting register %u at 0x%03x\n",
       (int) reg, base);



  outb (reg | 0x18, base);
  outb (0x04, base + 2);
  outb (0x06, base + 2);
  outb (0x04, base + 2);
  outb (0x04, base + 2);

}

static u_char
pa4s2_readbyte_nib (u_int base)
{

  u_char val;

  outb (0x05, base + 2);
  val = inb (base + 1);
  val >>= 4;
  outb (0x58, base);
  val |= inb (base + 1) & 0xF0;
  val ^= 0x88;
  outb (0x00, base);
  outb (0x04, base + 2);

  DBG (6, "pa4s2_readbyte_nib: reading value 0x%02x at 0x%03x\n",
       (int) val, base);

  return val;

}

static void
pa4s2_readend_nib (u_int base)
{

  DBG (6, "pa4s2_readend_nib: end of reading sequence\n");
  DBG (129, "unused base 0x%03X\n", base);

}

static void
pa4s2_writebyte_any (u_int base, u_char reg, u_char val)
{

  DBG (6, "pa4s2_writebyte_any: writing value 0x%02x"
       " in reg %u at 0x%03x\n", (int) val, (int) reg, base);

  outb (reg | 0x10, base);
  outb (0x04, base + 2);
  outb (0x06, base + 2);
  outb (0x06, base + 2);
  outb (0x06, base + 2);
  outb (0x06, base + 2);
  outb (0x04, base + 2);
  outb (0x04, base + 2);
  outb (val, base);
  outb (0x05, base + 2);
  outb (0x05, base + 2);
  outb (0x05, base + 2);
  outb (0x04, base + 2);
  outb (0x04, base + 2);
  outb (0x04, base + 2);
  outb (0x04, base + 2);

}

static void
pa4s2_enable (u_int base, u_char * prelock)
{

  prelock[0] = inb (base);
  prelock[1] = inb (base + 1);
  prelock[2] = inb (base + 2);
  outb ((prelock[2] & 0x0F) | 0x04, base + 2);

  DBG (6, "pa4s2_enable: prelock[] = {0x%02x, 0x%02x, 0x%02x}\n",
       (int) prelock[0], (int) prelock[1], (int) prelock[2]);

  outb (0x15, base);
  outb (0x95, base);
  outb (0x35, base);
  outb (0xB5, base);
  outb (0x55, base);
  outb (0xD5, base);
  outb (0x75, base);
  outb (0xF5, base);
  outb (0x01, base);
  outb (0x81, base);

}

static void
pa4s2_disable (u_int base, u_char * prelock)
{

  if ((sanei_pa4s2_interface_options & SANEI_PA4S2_OPT_ALT_LOCK) != 0)
    {

      DBG (6, "pa4s2_disable: using alternative command set\n");

      outb (0x00, base);
      outb (0x04, base + 2);
      outb (0x06, base + 2);
      outb (0x04, base + 2);

    }

  outb (prelock[2] & 0x0F, base + 2);

  outb (0x15, base);
  outb (0x95, base);
  outb (0x35, base);
  outb (0xB5, base);
  outb (0x55, base);
  outb (0xD5, base);
  outb (0x75, base);
  outb (0xF5, base);
  outb (0x00, base);
  outb (0x80, base);

  outb (prelock[0], base);
  outb (prelock[1], base + 1);
  outb (prelock[2], base + 2);

  DBG (6, "pa4s2_disable: state restored\n");

}

static int
pa4s2_close (int fd, SANE_Status * status)
{
  DBG (4, "pa4s2_close: fd=%d\n", fd);

  DBG (6, "pa4s2_close: this is port 0x%03lx\n", port[fd].base);


  DBG (5, "pa4s2_close: checking whether port is enabled\n");

  if (port[fd].enabled == SANE_TRUE)
    {

      DBG (6, "pa4s2_close: disabling port\n");
      pa4s2_disable (port[fd].base, port[fd].prelock);

    }


  DBG (5, "pa4s2_close: trying to free io port\n");
  if (ioperm (port[fd].base, 5, 0))
    {

      DBG (1, "pa4s2_close: can't free port 0x%03lx\n", port[fd].base);

      DBG (5, "pa4s2_close: returning SANE_STATUS_IO_ERROR\n");
      *status = SANE_STATUS_IO_ERROR;
      return -1;

    }

  DBG (5, "pa4s2_close: marking port as unused\n");

  port[fd].in_use = SANE_FALSE;

  DBG (5, "pa4s2_close: returning SANE_STATUS_GOOD\n");

  *status = SANE_STATUS_GOOD;

  return 0;

}


SANE_Status
sanei_pa4s2_open (const char *dev, int *fd)
{

  u_char asic, val;
  SANE_Status status;

  TEST_DBG_INIT ();

  DBG(4, "sanei_pa4s2_open: called for device '%s'\n", dev);
  DBG(5, "sanei_pa4s2_open: trying to connect to port\n");

  if ((*fd = pa4s2_open (dev, &status)) == -1)
    {

      DBG (5, "sanei_pa4s2_open: connection failed\n");

      return status;

    }

  DBG (6, "sanei_pa4s2_open: connected to device using fd %u\n", *fd);

  DBG (5, "sanei_pa4s2_open: checking for scanner\n");

  sanei_pa4s2_enable (*fd, SANE_TRUE);

  DBG (6, "sanei_pa4s2_open: reading ASIC id\n");

  sanei_pa4s2_readbegin (*fd, 0);

  sanei_pa4s2_readbyte (*fd, &asic);

  sanei_pa4s2_readend (*fd);

  switch (asic)
    {

    case PA4S2_ASIC_ID_1013:
      DBG (3, "sanei_pa4s2_open: detected ASIC id 1013\n");
      break;

    case PA4S2_ASIC_ID_1015:
      DBG (3, "sanei_pa4s2_open: detected ASIC id 1015\n");
      break;

    case PA4S2_ASIC_ID_1505:
      DBG (3, "sanei_pa4s2_open: detected ASIC id 1505\n");
      break;

    default:
      DBG (1, "sanei_pa4s2_open: could not find scanner\n");
      DBG (3, "sanei_pa4s2_open: reported ASIC id 0x%02x\n",
	   asic);

      sanei_pa4s2_enable (*fd, SANE_FALSE);
      DBG (5, "sanei_pa4s2_open: closing port\n");

      sanei_pa4s2_close (*fd);

      DBG (5, "sanei_pa4s2_open: returning"
	   " SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  sanei_pa4s2_enable (*fd, SANE_FALSE);

  DBG (4, "sanei_pa4s2_open: trying better modes\n");

  while (port[*fd].mode <= PA4S2_MODE_EPP)
    {

      if ((port[*fd].mode == PA4S2_MODE_UNI) &&
      ((sanei_pa4s2_interface_options & SANEI_PA4S2_OPT_TRY_MODE_UNI) == 0))
	{

	  DBG (3, "sanei_pa4s2_open: skipping mode UNI\n");
	  port[*fd].mode++;
	  continue;

	}

      DBG (5, "sanei_pa4s2_open: trying mode %u\n", port[*fd].mode);

      sanei_pa4s2_enable (*fd, SANE_TRUE);

      sanei_pa4s2_readbegin (*fd, 0);

      sanei_pa4s2_readbyte (*fd, &val);

      if (val != asic)
	{

	  sanei_pa4s2_readend (*fd);
	  sanei_pa4s2_enable (*fd, SANE_FALSE);
	  DBG (5, "sanei_pa4s2_open: mode failed\n");
	  DBG (6, "sanei_pa4s2_open: returned ASIC-ID 0x%02x\n",
	       (int) val);
	  break;

	}

      sanei_pa4s2_readend (*fd);
      sanei_pa4s2_enable (*fd, SANE_FALSE);

      DBG (5, "sanei_pa4s2_open: mode works\n");

      port[*fd].mode++;

    }

  port[*fd].mode--;

  if ((port[*fd].mode == PA4S2_MODE_UNI) &&
      ((sanei_pa4s2_interface_options & SANEI_PA4S2_OPT_TRY_MODE_UNI) == 0))
    {
      port[*fd].mode--;
    }

  DBG (5, "sanei_pa4s2_open: using mode %u\n", port[*fd].mode);

  DBG (4, "sanei_pa4s2_open: returning SANE_STATUS_GOOD\n");

  return SANE_STATUS_GOOD;

}

void
sanei_pa4s2_close (int fd)
{

  SANE_Status status;

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_close: fd = %d\n", fd);

  if ((fd < 0) || (fd >= NELEMS (port)))
    {

      DBG (2, "sanei_pa4s2_close: fd %d is invalid\n", fd);
      DBG (5, "sanei_pa4s2_close: failed\n");
      return;

    }

  if (port[fd].in_use == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_close: port is not in use\n");
      DBG (6, "sanei_pa4s2_close: port is 0x%03lx\n", port[fd].base);
      DBG (5, "sanei_pa4s2_close: failed\n");
      return;

    }

  DBG (5, "sanei_pa4s2_close: freeing resources\n");

  if (pa4s2_close (fd, &status) == -1)
    {

      DBG (2, "sanei_pa4s2_close: could not close scanner\n");
      DBG (5, "sanei_pa4s2_close: failed\n");
      return;
    }

  DBG (5, "sanei_pa4s2_close: finished\n");

}

SANE_Status
sanei_pa4s2_enable (int fd, int enable)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_enable: called for fd %d with value %d\n",
       fd, enable);

  if ((fd < 0) || (fd >= NELEMS (port)))
    {

      DBG (2, "sanei_pa4s2_enable: fd %d is invalid\n", fd);
      DBG (5, "sanei_pa4s2_enable: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].in_use == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_enable: port is not in use\n");
      DBG (6, "sanei_pa4s2_enable: port is 0x%03lx\n", port[fd].base);
      DBG (5, "sanei_pa4s2_enable: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if ((enable != SANE_TRUE) && (enable != SANE_FALSE))
    {

      DBG (2, "sanei_pa4s2_enable: invalid value %d\n", enable);
      DBG (5, "sanei_pa4s2_enable: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if ((unsigned int) enable == port[fd].enabled)
    {

      DBG (3, "sanei_pa4s2_enable: senseless call...\n");
      DBG (4, "sanei_pa4s2_enable: aborting\n");
      DBG (5, "sanei_pa4s2_enable: returning SANE_STATUS_GOOD\n");

      return SANE_STATUS_GOOD;

    }

  if (enable == SANE_TRUE)
    {

      DBG (4, "sanei_pa4s2_enable: enable port 0x%03lx\n",
	   port[fd].base);

      pa4s2_enable (port[fd].base, port[fd].prelock);

    }
  else
    {

      DBG (4, "sanei_pa4s2_enable: disable port 0x%03lx\n",
	   port[fd].base);

      pa4s2_disable (port[fd].base, port[fd].prelock);

    }

  port[fd].enabled = enable;

  DBG (5, "sanei_pa4s2_enable: returning SANE_STATUS_GOOD\n");

  return SANE_STATUS_GOOD;
}

SANE_Status
sanei_pa4s2_readbegin (int fd, u_char reg)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_readbegin: called for fd %d and register %u\n",
       fd, (int) reg);

  if ((fd < 0) || (fd >= NELEMS (port)))
    {

      DBG (2, "sanei_pa4s2_readbegin: invalid fd %d\n", fd);
      DBG (5, "sanei_pa4s2_readbegin: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].in_use == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_readbegin: port is not in use\n");
      DBG (6, "sanei_pa4s2_readbegin: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_readbegin: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].enabled == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_readbegin: port is not enabled\n");
      DBG (6, "sanei_pa4s2_readbegin: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_readbegin: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  switch (port[fd].mode)
    {

    case PA4S2_MODE_EPP:

      DBG (5, "sanei_pa4s2_readbegin: EPP readbegin\n");
      pa4s2_readbegin_epp (port[fd].base, reg);
      break;

    case PA4S2_MODE_UNI:

      DBG (5, "sanei_pa4s2_readbegin: UNI readbegin\n");
      pa4s2_readbegin_uni (port[fd].base, reg);
      break;

    case PA4S2_MODE_NIB:

      DBG (5, "sanei_pa4s2_readbegin: NIB readbegin\n");
      pa4s2_readbegin_nib (port[fd].base, reg);
      break;

    default:

      DBG (1, "sanei_pa4s2_readbegin: port info broken\n");
      DBG (3, "sanei_pa4s2_readbegin: invalid port mode\n");
      DBG (6, "sanei_pa4s2_readbegin: port mode %u\n",
	   port[fd].mode);
      DBG (5, "sanei_pa4s2_readbegin: return"
	   " SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  DBG (5, "sanei_pa4s2_readbegin: returning SANE_STATUS_GOOD\n");

  return SANE_STATUS_GOOD;
}

SANE_Status
sanei_pa4s2_readbyte (int fd, u_char * val)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_readbyte: called with fd %d\n", fd);

  if ((fd < 0) || (fd >= NELEMS (port)))
    {

      DBG (2, "sanei_pa4s2_readbyte: invalid fd %d\n", fd);
      DBG (5, "sanei_pa4s2_readbyte: returning SANE_STATUS_INVAL\n");
      return SANE_STATUS_INVAL;

    }

  if (port[fd].in_use == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_readbyte: port is not in use\n");
      DBG (6, "sanei_pa4s2_readbyte: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_readbyte: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].enabled == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_readbyte: port is not enabled\n");
      DBG (6, "sanei_pa4s2_readbyte: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_readbyte: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  DBG (4, "sanei_pa4s2_readbyte: we hope, the backend called\n");
  DBG (4, "sanei_pa4s2_readbyte: readbegin, so the port is ok...\n");

  DBG (6, "sanei_pa4s2_readbyte: this means, I did not check it - it's\n");
  DBG (6, "sanei_pa4s2_readbyte: not my fault, if your PC burns down.\n");

  switch (port[fd].mode)
    {

    case PA4S2_MODE_EPP:

      DBG (5, "sanei_pa4s2_readbyte: read in EPP mode\n");
      *val = pa4s2_readbyte_epp (port[fd].base);
      break;


    case PA4S2_MODE_UNI:

      DBG (5, "sanei_pa4s2_readbyte: read in UNI mode\n");
      *val = pa4s2_readbyte_uni (port[fd].base);
      break;


    case PA4S2_MODE_NIB:

      DBG (5, "sanei_pa4s2_readbyte: read in NIB mode\n");
      *val = pa4s2_readbyte_nib (port[fd].base);
      break;

    default:

      DBG (1, "sanei_pa4s2_readbyte: port info broken\n");
      DBG (2, "sanei_pa4s2_readbyte: probably the port wasn't"
	   " correct configured...\n");
      DBG (3, "sanei_pa4s2_readbyte: invalid port mode\n");
      DBG (6, "sanei_pa4s2_readbyte: port mode %u\n",
	   port[fd].mode);
      DBG (6, "sanei_pa4s2_readbyte: I told you!!!\n");
      DBG (5, "sanei_pa4s2_readbyte: return"
	   " SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;
    }

  DBG (5, "sanei_pa4s2_readbyte: read finished\n");

  DBG (6, "sanei_pa4s2_readbyte: got value 0x%02x\n", (int) *val);

  DBG (5, "sanei_pa4s2_readbyte: returning SANE_STATUS_GOOD\n");

  return SANE_STATUS_GOOD;

}

SANE_Status
sanei_pa4s2_readend (int fd)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_readend: called for fd %d\n", fd);

  if ((fd < 0) || (fd >= NELEMS (port)))
    {

      DBG (2, "sanei_pa4s2_readend: invalid fd %d\n", fd);
      DBG (5, "sanei_pa4s2_readend: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].in_use == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_readend: port is not in use\n");
      DBG (6, "sanei_pa4s2_readend: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_readend: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].enabled == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_readend: port is not enabled\n");
      DBG (6, "sanei_pa4s2_readend: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_readend: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  DBG (4, "sanei_pa4s2_readend: we hope, the backend called\n");
  DBG (4, "sanei_pa4s2_readend: readbegin, so the port is ok...\n");

  DBG (6, "sanei_pa4s2_readend: this means, I did not check it - it's\n");
  DBG (6, "sanei_pa4s2_readend: not my fault, if your PC burns down.\n");

  switch (port[fd].mode)
    {

    case PA4S2_MODE_EPP:

      DBG (5, "sanei_pa4s2_readend: EPP mode readend\n");
      pa4s2_readend_epp (port[fd].base);
      break;


    case PA4S2_MODE_UNI:

      DBG (5, "sanei_pa4s2_readend: UNI mode readend\n");
      pa4s2_readend_uni (port[fd].base);
      break;


    case PA4S2_MODE_NIB:

      DBG (5, "sanei_pa4s2_readend: NIB mode readend\n");
      pa4s2_readend_nib (port[fd].base);
      break;

    default:

      DBG (1, "sanei_pa4s2_readend: port info broken\n");
      DBG (2, "sanei_pa4s2_readend: probably the port wasn't"
	   " correct configured...\n");
      DBG (3, "sanei_pa4s2_readend: invalid port mode\n");
      DBG (6, "sanei_pa4s2_readend: port mode %u\n",
	   port[fd].mode);
      DBG (6, "sanei_pa4s2_readend: I told you!!!\n");
      DBG (5, "sanei_pa4s2_readend: return"
	   " SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;
    }


  DBG (5, "sanei_pa4s2_readend: returning SANE_STATUS_GOOD\n");

  return SANE_STATUS_GOOD;

}

SANE_Status
sanei_pa4s2_writebyte (int fd, u_char reg, u_char val)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_writebyte: called for fd %d, reg %u and val %u\n",
       fd, (int) reg, (int) val);

  if ((fd < 0) || (fd >= NELEMS (port)))
    {

      DBG (2, "sanei_pa4s2_writebyte: invalid fd %d\n", fd);
      DBG (5, "sanei_pa4s2_writebyte: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].in_use == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_writebyte: port is not in use\n");
      DBG (6, "sanei_pa4s2_writebyte: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_writebyte: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  if (port[fd].enabled == SANE_FALSE)
    {

      DBG (2, "sanei_pa4s2_writebyte: port is not enabled\n");
      DBG (6, "sanei_pa4s2_writebyte: port is 0x%03lx\n",
	   port[fd].base);
      DBG (5, "sanei_pa4s2_readbegin: returning SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  switch (port[fd].mode)
    {

    case PA4S2_MODE_EPP:
    case PA4S2_MODE_UNI:
    case PA4S2_MODE_NIB:

      DBG (5, "sanei_pa4s2_writebyte: NIB/UNI/EPP write\n");
      pa4s2_writebyte_any (port[fd].base, reg, val);
      break;

    default:

      DBG (1, "sanei_pa4s2_writebyte: port info broken\n");
      DBG (3, "sanei_pa4s2_writebyte: invalid port mode\n");
      DBG (6, "sanei_pa4s2_writebyte: port mode %u\n",
	   port[fd].mode);
      DBG (5, "sanei_pa4s2_writebyte: return"
	   " SANE_STATUS_INVAL\n");

      return SANE_STATUS_INVAL;

    }

  DBG (5, "sanei_pa4s2_writebyte: returning SANE_STATUS_GOOD\n");

  return SANE_STATUS_GOOD;
}

SANE_Status
sanei_pa4s2_options (u_int * options, int set)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_options: called with options %u and set = %d\n",
       *options, set);

  if ((set != SANE_TRUE) && (set != SANE_FALSE))
    DBG (2, "sanei_pa4s2_options: value of set is invalid\n");

  if ((set == SANE_TRUE) && (*options > 3))
    DBG (2, "sanei_pa4s2_options: value of *options is invalid\n");

  if (set == SANE_TRUE)
    {

      DBG (5, "sanei_pa4s2_options: setting options to %u\n", *options);

      sanei_pa4s2_interface_options = *options;

    }
  else
    {

      DBG (5, "sanei_pa4s2_options: options are set to %u\n",
	   sanei_pa4s2_interface_options);

      *options = sanei_pa4s2_interface_options;

    }

  DBG (5, "sanei_pa4s2_options: returning SANE_STATUS_GOOD\n");

  return SANE_STATUS_GOOD;

}

#else /* !HAVE_IOPERM */


SANE_Status
sanei_pa4s2_open (const char *dev, int *fd)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_open: called for device `%s`\n", dev);
  DBG (3, "sanei_pa4s2_open: A4S2 support not compiled\n");
  DBG (6, "sanei_pa4s2_open: basically, this backend does only compile\n");
  DBG (6, "sanei_pa4s2_open: on x86 architectures. Furthermore it\n");
  DBG (6, "sanei_pa4s2_open: needs ioperm() and inb()/outb() calls.\n");
  DBG (5, "sanei_pa4s2_open: returning SANE_STATUS_INVAL\n");

  return SANE_STATUS_INVAL;

}

void
sanei_pa4s2_close (int fd)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_close: called for fd %d\n", fd);
  DBG (2, "sanei_pa4s2_close: fd %d is invalid\n", fd);
  DBG (3, "sanei_pa4s2_close: A4S2 support not compiled\n");
  DBG (6, "sanei_pa4s2_close: so I wonder, why this function is called"
       " anyway.\n");
  DBG (6, "sanei_pa4s2_close: maybe this is a bug in the backend.\n");
  DBG (5, "sanei_pa4s2_close: returning\n");

  return;
}

SANE_Status
sanei_pa4s2_enable (int fd, int enable)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_enable: called for fd %d with value=%d\n",
       fd, enable);
  DBG (2, "sanei_pa4s2_enable: fd %d is invalid\n", fd);

  if ((enable != SANE_TRUE) && (enable != SANE_FALSE))
    DBG (2, "sanei_pa4s2_enable: value %d is invalid\n", enable);

  DBG (3, "sanei_pa4s2_enable: A4S2 support not compiled\n");
  DBG (6, "sanei_pa4s2_enable: oops, I think there's someone going to\n");
  DBG (6, "sanei_pa4s2_enable: produce a lot of garbage...\n");
  DBG (5, "sanei_pa4s2_enable: returning SANE_STATUS_INVAL\n");

  return SANE_STATUS_INVAL;
}

SANE_Status
sanei_pa4s2_readbegin (int fd, u_char reg)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_readbegin: called for fd %d and register %d\n",
       fd, (int) reg);
  DBG (2, "sanei_pa4s2_readbegin: fd %d is invalid\n", fd);

  DBG (3, "sanei_pa4s2_readbegin: A4S2 support not compiled\n");
  DBG (6, "sanei_pa4s2_readbegin: don't look - this is going to be\n");
  DBG (6, "sanei_pa4s2_readbegin: worse then you'd expect...\n");
  DBG (5, "sanei_pa4s2_readbegin: returning SANE_STATUS_INVAL\n");

  return SANE_STATUS_INVAL;

}

SANE_Status
sanei_pa4s2_readbyte (int fd, u_char * val)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_readbyte: called for fd %d\n", fd);
  DBG (2, "sanei_pa4s2_readbyte: fd %d is invalid\n", fd);
  DBG (3, "sanei_pa4s2_readbyte: A4S2 support not compiled\n");
  DBG (6, "sanei_pa4s2_readbyte: shit happens\n");
  DBG (5, "sanei_pa4s2_readbyte: returning SANE_STATUS_INVAL\n");

  return SANE_STATUS_INVAL;
}

SANE_Status
sanei_pa4s2_readend (int fd)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_readend: called for fd %d\n", fd);
  DBG (2, "sanei_pa4s2_readend: fd %d is invalid\n", fd);
  DBG (3, "sanei_pa4s2_readend: A4S2 support not compiled\n");
  DBG (6, "sanei_pa4s2_readend: it's too late anyway\n");
  DBG (5, "sanei_pa4s2_readend: returning SANE_STATUS_INVAL\n");

  return SANE_STATUS_INVAL;

}

SANE_Status
sanei_pa4s2_writebyte (int fd, u_char reg, u_char val)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_writebyte: called for fd %d and register %d, "
       "value = %u\n", fd, (int) reg, (int) val);
  DBG (2, "sanei_pa4s2_writebyte: fd %d is invalid\n", fd);
  DBG (3, "sanei_pa4s2_writebyte: A4S2 support not compiled\n");
  DBG (6, "sanei_pa4s2_writebyte: whatever backend you're using, tell\n");
  DBG (6, "sanei_pa4s2_writebyte: the maintainer his code has bugs...\n");
  DBG (5, "sanei_pa4s2_writebyte: returning SANE_STATUS_INVAL\n");

  return SANE_STATUS_INVAL;

}

SANE_Status
sanei_pa4s2_options (u_int * options, int set)
{

  TEST_DBG_INIT ();

  DBG (4, "sanei_pa4s2_options: called with options %u and set = %d\n",
       *options, set);

  if ((set != SANE_TRUE) && (set != SANE_FALSE))
    DBG (2, "sanei_pa4s2_options: value of set is invalid\n");

  if ((set == SANE_TRUE) && (*options > 3))
    DBG (2, "sanei_pa4s2_options: value of *options is invalid\n");

  DBG (3, "sanei_pa4s2_options: A4S2 support not compiled\n");
  DBG (5, "sanei_pa4s2_options: returning SANE_STATUS_INVAL\n");

  return SANE_STATUS_INVAL;

}


#endif /* !HAVE_IOPERM */
