/* sane - Scanner Access Now Easy.
   Copyright (C) 1998 Christian Bucher
   Copyright (C) 1998 Kling & Hautzinger GmbH
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

   This file implements the bi-directional parallel-port
   interface.  */

/*
  RESTRICTIONS:

  - This interface is very timing sensitive, be carefull with setting
    debug levels.
 */

#include "sane/config.h"

#define BACKEND_NAME sanei_pio
#include "sane/sanei_backend.h"		/* pick up compatibility defs */

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_SYS_IO_H
# include <sys/io.h>	/* use where available (glibc 2.x, for example) */
#elif HAVE_ASM_IO_H
# include <asm/io.h>		/* ugly, but backwards compatible */
#elif HAVE_SYS_HW_H
# include <sys/hw.h>
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
# define IO_SUPPORT_MISSING
#endif

#include "sane/sane.h"
#include "sane/sanei.h"
#include "sane/sanei_pio.h"

#if defined (HAVE_IOPERM) && !defined (IO_SUPPORT_MISSING)

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "sane/saneopts.h"

#define PORT_DEV	"/dev/port"

/*    base    278 (lpt2)
   
              ioport  stat    ctrl
      offs    0       1       2
      len     1       1       1     */

/* Port definitions (`N' at end begin of label means negated signal) */

#define PIO_IOPORT		0	/* rel. addr io port      */

#define PIO_STAT		1	/* rel. addr status port  */
#define PIO_STAT_BUSY		(1<<7)	/*  BUSY   Pin            */
#define PIO_STAT_NACKNLG	(1<<6)	/* ~ACKNLG Pin            */

#define PIO_CTRL		2	/* rel. addr control port */
#define PIO_CTRL_IE		(1<<5)	/* Input enable           */
#define PIO_CTRL_IRQE		(1<<4)	/* enable IRQ             */
#define PIO_CTRL_DIR		(1<<3)	/* DIR pin, DIR=1 => out */
#define PIO_CTRL_NINIT		(1<<2)	/* reset output           */
#define PIO_CTRL_FDXT		(1<<1)	/* Paper FEED (unused)    */
#define PIO_CTRL_NSTROBE	(1<<0)	/* strobe pin             */

#define PIO_APPLYRESET		2000	/* reset in 10us at init time  */

#define DL40	 6
#define DL50	 7
#define DL60	 8
#define DL61	 9
#define DL70	10
#define DL71	11

#ifdef NDEBUG
# define DBG_INIT()
#endif

typedef struct
  {
    u_long base;		/* i/o base address */
    int fd;			/* >= 0 when using /dev/port */
    int max_polls;		/* forever if <= 0 */
    u_int in_use;		/* port in use? */
  }
PortRec, *Port;

static PortRec port[] =
  {
    {0x378, -1, 0, 0},
    {0x278, -1, 0, 0}
  };

extern int setuid (uid_t);

static inline int pio_outb (const Port port, u_char val, u_long addr);
static inline int pio_inb (const Port port, u_char * val, u_long addr);
static inline int pio_wait (const Port port, u_char val, u_char mask);
static inline void pio_ctrl (const Port port, u_char val);
static inline void pio_delay (const Port port);
static inline void pio_init (const Port port);
static void pio_reset (const Port port);
static int pio_write (const Port port, const u_char * buf, int n);
static int pio_read (const Port port, u_char * buf, int n);
static int pio_open (const char *dev, SANE_Status * status);

static inline int 
pio_outb (const Port port, u_char val, u_long addr)
{

  if (-1 == port->fd)
    outb (val, addr);
  else
    {
      if (addr != (u_long)lseek (port->fd, addr, SEEK_SET))
	return -1;
      if (1 != write (port->fd, &val, 1))
	return -1;
    }
  return 0;
}

static inline int 
pio_inb (const Port port, u_char * val, u_long addr)
{

  if (-1 == port->fd)
    *val = inb (addr);
  else
    {
      if (addr != (u_long)lseek (port->fd, addr, SEEK_SET))
	return -1;
      if (1 != read (port->fd, val, 1))
	return -1;
    }
  return 0;
}

static inline int 
pio_wait (const Port port, u_char val, u_char mask)
{
  int n, stat = 0;

  DBG (DL60, "wait on port 0x%03lx for %02x mask %02x\n",
       port->base, (int) val, (int) mask);
  DBG (DL61, "   BUSY    %s\n", (mask & PIO_STAT_BUSY) ?
       (val & PIO_STAT_BUSY ? "on" : "off") : "-");
  DBG (DL61, "   NACKNLG %s\n",
       (mask & PIO_STAT_NACKNLG) ? (val & PIO_STAT_NACKNLG ? "on" : "off")
       : "-");

  for (n = 1; (port->max_polls <= 0) || (n <= port->max_polls); n++)
    {
      stat = inb (port->base + PIO_STAT);
      if ((stat & mask) == (val & mask))
	{
	  DBG (DL60, "got %02x after %d tries\n", stat, n);
	  DBG (DL61, "   BUSY    %s\n", stat & PIO_STAT_BUSY ? "on" : "off");
	  DBG (DL61, "   NACKNLG %s\n",
	       stat & PIO_STAT_NACKNLG ? "on" : "off");

	  return stat;
	}
    }
  DBG (DL60, "got %02x aborting after %d\n", stat, port->max_polls);
  DBG (DL61, "   BUSY    %s\n", stat & PIO_STAT_BUSY ? "on" : "off");
  DBG (DL61, "   NACKNLG %s\n", stat & PIO_STAT_NACKNLG ? "on" : "off");
  DBG (1, "polling time out, abort\n");
  exit (-1);
}

static inline void
pio_ctrl (const Port port, u_char val)
{
  DBG (DL60, "ctrl on port 0x%03lx %02x %02x\n",
       port->base, (int) val, (int) val ^ PIO_CTRL_NINIT);

  val ^= PIO_CTRL_NINIT;

  DBG (DL61, "   IE      %s\n", val & PIO_CTRL_IE ? "on" : "off");
  DBG (DL61, "   IRQE    %s\n", val & PIO_CTRL_IRQE ? "on" : "off");
  DBG (DL61, "   DIR     %s\n", val & PIO_CTRL_DIR ? "on" : "off");
  DBG (DL61, "   NINIT   %s\n", val & PIO_CTRL_NINIT ? "on" : "off");
  DBG (DL61, "   FDXT    %s\n", val & PIO_CTRL_FDXT ? "on" : "off");
  DBG (DL61, "   NSTROBE %s\n", val & PIO_CTRL_NSTROBE ? "on" : "off");

  outb (val, port->base + PIO_CTRL);

  return;
}

static inline void 
pio_delay (const Port port)
{
  inb (port->base + PIO_STAT);	/* delay */

  return;
}

static inline void 
pio_init (const Port port)
{
  pio_ctrl (port, PIO_CTRL_IE);
  return;
}

static void 
pio_reset (const Port port)
{
  int n;

  DBG (DL40, "reset\n");

  for (n = PIO_APPLYRESET; --n >= 0;)
    {
      outb ((PIO_CTRL_IE | PIO_CTRL_NINIT) ^ PIO_CTRL_NINIT,
	    port->base + PIO_CTRL);
    }
  pio_init (port);

  DBG (DL40, "end reset\n");

  return;
}

static int 
pio_write (const Port port, const u_char * buf, int n)
{
  int k;

  DBG (DL40, "write\n");

  pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY);	/* busy */
  pio_ctrl (port, PIO_CTRL_DIR | PIO_CTRL_IE);		/* praeoutput */
  pio_wait (port, PIO_STAT_NACKNLG, PIO_STAT_NACKNLG);	/* acknlg */
  pio_ctrl (port, PIO_CTRL_DIR);			/* output */

  for (k = 0; k < n; k++, buf++)
    {
      DBG (DL40, "write byte\n");
#ifdef HANDSHAKE_BUSY
      pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY);	/* busy */
#else
      pio_wait (port, PIO_STAT_BUSY | PIO_STAT_NACKNLG,
		PIO_STAT_BUSY | PIO_STAT_NACKNLG);	/* busyack */
#endif
      DBG (DL60, "out  %02x\n", (int) *buf);

      outb (*buf, port->base + PIO_IOPORT);

      pio_delay (port);
      pio_delay (port);
      pio_delay (port);
      pio_ctrl (port, PIO_CTRL_DIR | PIO_CTRL_NSTROBE);	/* outputstrobe */

      pio_delay (port);
      pio_delay (port);
      pio_delay (port);
      pio_ctrl (port, PIO_CTRL_DIR);	/* output */

      pio_delay (port);
      pio_delay (port);
      pio_delay (port);

      DBG (DL40, "end write byte\n");
    }

#ifdef HANDSHAKE_BUSY
  pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY);	/* busy */
#else
  pio_wait (port, PIO_STAT_BUSY | PIO_STAT_NACKNLG,
	    PIO_STAT_BUSY | PIO_STAT_NACKNLG);	/* busyack */
#endif

  pio_ctrl (port, PIO_CTRL_DIR | PIO_CTRL_IE);	/* praeoutput */
  DBG (DL40, "end write\n");
  return k;
}

static int 
pio_read (const Port port, u_char * buf, int n)
{
  int k;

  DBG (DL40, "read\n");

  pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY);	/* busy */
  pio_ctrl (port, PIO_CTRL_IE);	/* input */

  for (k = 0; k < n; k++, buf++)
    {
      DBG (DL40, "read byte\n");

#ifdef HANDSHAKE_BUSY
      pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY);	/* busy */
#else
      pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY | PIO_STAT_NACKNLG);
      /* busynack */
#endif
      pio_ctrl (port, PIO_CTRL_IE | PIO_CTRL_NSTROBE);	/* inputstrobe */

      pio_delay (port);
      pio_delay (port);
      pio_delay (port);
      pio_ctrl (port, PIO_CTRL_IE);	/* input */
#ifdef HANDSHAKE_BUSY
      pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY);	/* busy */
#else
      pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY | PIO_STAT_NACKNLG);
      /* busynack */
#endif

      *buf = inb (port->base + PIO_IOPORT);
      DBG (DL60, "in   %02x\n", (int) *buf);
      DBG (DL40, "end read byte\n");
    }

  pio_wait (port, PIO_STAT_BUSY, PIO_STAT_BUSY);	/* busy */
  pio_ctrl (port, PIO_CTRL_IE);	/* input */
  DBG (DL40, "end read\n");
  return k;
}

/*
     Open the device, <dev> must contain a valid port number (as string).   
 */

static int 
pio_open (const char *dev, SANE_Status * status)
{
  static int first_time = 1;
  u_long base;
  int n;

  if (first_time)
    {
      first_time = 0;

      DBG_INIT ();
      /* set root uid */
      if (0 > setuid (0))
	{
	  DBG (1, "sanei_pio_open: setuid failed: errno = %d\n", errno);
	  *status = SANE_STATUS_INVAL;
	  return -1;
	}
    }
  /* read port number */
  {
    char *end;

    base = strtol (dev, &end, 0);

    if ((end == dev) || *end)
      {
	DBG (1, "sanei_pio_open: `%s' is not a valid port number\n", dev);
	*status = SANE_STATUS_INVAL;
	return -1;
      }
  }

  if (0 == base)
    {
      DBG (1, "sanei_pio_open: 0x%03lx is not a valid base address\n", base);
      *status = SANE_STATUS_INVAL;
      return -1;
    }

  for (n = 0; n < NELEMS (port); n++)
    if (port[n].base == base)
      break;

  if (NELEMS (port) <= n)
    {
      DBG (1, "sanei_pio_open: 0x%03lx is not a valid base address\n", base);
      *status = SANE_STATUS_INVAL;
      return -1;
    }

  if (port[n].in_use)
    {
      DBG (1, "sanei_pio_open: port 0x%03lx is already in use\n", base);
      *status = SANE_STATUS_DEVICE_BUSY;
      return -1;
    }
  port[n].base = base;
  port[n].fd = -1;
  port[n].max_polls = 0;
  port[n].in_use = 1;

  if (ioperm (port[n].base, 3, 1))
    {
      DBG (1, "sanei_pio_open: cannot get io privilege for port 0x%03lx\n",
	   port[n].base);
      *status = SANE_STATUS_IO_ERROR;
      return -1;
    }

  pio_reset (&port[n]);

  *status = SANE_STATUS_GOOD;
  return n;
}

SANE_Status 
sanei_pio_open (const char *dev, int *fdp)
{
  SANE_Status status;

  *fdp = pio_open (dev, &status);
  return status;
}

void 
sanei_pio_close (int fd)
{
  Port p = port + fd;

  if ((0 > fd) && (NELEMS (port) <= fd))
    return;

  if (!p->in_use)
    return;

  if (-1 != p->fd)
    {
      close (p->fd);
      p->fd = -1;
    }

  p->in_use = 0;

  return;
}

int 
sanei_pio_read (int fd, u_char * buf, int n)
{
  if ((0 > fd) && (NELEMS (port) <= fd))
    return -1;

  if (!port[fd].in_use)
    return -1;

  return pio_read (&port[fd], buf, n);
}

int 
sanei_pio_write (int fd, const u_char * buf, int n)
{
  if ((0 > fd) && (NELEMS (port) <= fd))
    return -1;

  if (!port[fd].in_use)
    return -1;

  return pio_write (&port[fd], buf, n);
}

#else /* !HAVE_IOPERM */

SANE_Status
sanei_pio_open (const char *dev, int *fdp)
{
  *fdp = -1;
  return SANE_STATUS_INVAL;
}


void 
sanei_pio_close (int fd)
{
  return;
}

int 
sanei_pio_read (int fd, u_char * buf, int n)
{
  return -1;
}

int 
sanei_pio_write (int fd, const u_char * buf, int n)
{
  return -1;
}

#endif /* !HAVE_IOPERM */
