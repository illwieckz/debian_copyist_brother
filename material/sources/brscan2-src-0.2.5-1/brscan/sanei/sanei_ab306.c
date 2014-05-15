/* sane - Scanner Access Now Easy.
   Copyright (C) 1997 Andreas Czechanowski and David Mosberger
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

   This file implements the Mustek-proprietary SCSI-over-parallel-port
   interface.  */

#include "sane/config.h"

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <sys/types.h>

#ifdef HAVE_SYS_IO_H
# include <sys/io.h>	/* use where available (glibc 2.x, for example) */
#elif HAVE_ASM_IO_H
# include <asm/io.h>	/* ugly, but backwards compatible */
#elif defined (__i386__)  && defined (__GNUC__)

static __inline__ void
outb (u_char value, u_long port)
{
  __asm__ __volatile__ ("outb %0,%1" : : "a" (value), "d" ((u_short) port));
}

static __inline__ u_char
inb (u_long port)
{
  u_char value;

  __asm__ __volatile__ ("inb %1,%0" : "=a" (value) : "d" ((u_short)port));
  return value;
}

#else
# define IO_SUPPORT_MISSING
#endif

#include "sane/sane.h"
#include "sane/sanei.h"
#include "sane/sanei_ab306.h"

#if (defined(HAVE_IOPERM) || defined(__FreeBSD__)) && !defined(IO_SUPPORT_MISSING)

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

#define BACKEND_NAME sanei_ab306
#include "sane/sanei_debug.h"

#define PORT_DEV	"/dev/port"
#define AB306_CIO	0x379		/* control i/o port */

#if defined(__FreeBSD__)	
static int dev_io_fd = 0;
#endif

typedef struct port
  {
    u_long	base;			/* i/o base address */
    int		port_fd;		/* >= 0 when using /dev/port */
    u_int	lstat;
    u_int	in_use	      : 1,	/* port in use? */
		active	      : 1;	/* port was active at some point */
  }
Port;

static Port port[] =
  {
    {0x26b, -1, 0, 0, 0},
    {0x2ab, -1, 0, 0, 0},
    {0x2eb, -1, 0, 0, 0},
    {0x22b, -1, 0, 0, 0},
    {0x32b, -1, 0, 0, 0},
    {0x36b, -1, 0, 0, 0},
    {0x3ab, -1, 0, 0, 0},
    {0x3eb, -1, 0, 0, 0}
  };

static const SANE_Byte wakeup[] =
  {
    0x47, 0x55, 0x54, 0x53, 0x02, 0x01, 0x80
  };

static u_char cdb_sizes[8] =
  {
    6, 10, 10, 12, 12, 12, 10, 10
  };
#define CDB_SIZE(opcode)	cdb_sizes[(((opcode) >> 5) & 7)]

static void
ab306_outb (Port *p, u_long addr, u_char val)
{

  if (p->port_fd >= 0)
    {
      if ((u_long) lseek (p->port_fd, addr, SEEK_SET) != addr)
	return;
      if (write (p->port_fd, &val, 1) != 1)
	return;
    }
  else
    outb (val, addr);
}

static int
ab306_inb (Port *p, u_long addr)
{
  u_char ch;

  if (p->port_fd >= 0)
    {
      if ((u_long) lseek (p->port_fd, addr, SEEK_SET) != addr)
	return -1;
      if (read (p->port_fd, &ch, 1) != 1)
	return -1;
      return ch;
    }
  else
    return inb (addr);
}

/* Send a single command-byte over the AB306N-interface.  */
static void
ab306_cout (Port *p, int val)
{
  u_long base = p->base;

  while ((ab306_inb (p, base + 1) & 0x80));	/* wait for dir flag */
  ab306_outb (p, base, val);
  ab306_outb (p, base + 1, 0xe0);
  while ((ab306_inb (p, base + 1) & 0x80) == 0);	/* wait for ack */
  ab306_outb (p, base + 1, 0x60);
}

/* Read a single response-byte from the SANEI_AB306N-interface.  */
static int
ab306_cin (Port *p)
{
  u_long base = p->base;
  u_char val;

  while ((ab306_inb (p, base + 1) & 0x80) == 0);	/* wait for dir flag */
  val = ab306_inb (p, base);
  ab306_outb (p, base + 1, 0xe0);			/* ack received byte */
  while (ab306_inb (p, base + 1) & 0x80);
  ab306_outb (p, base + 1, 0x60);			/* reset ack */
  return val;
}

static SANE_Status
ab306_write (Port *p, const void *buf, size_t len)
{
  u_long base = p->base;
  u_int i;
  int cksum = 0;

  DBG(3, "ab306_write: waiting for scanner to be ready %02x\n",
      ab306_inb (p, base + 1));
  while ((ab306_inb (p, base + 1) & 0x20) == 0);
  usleep (10000);

  DBG(4, "ab306_write: writing data\n");
  for (i = 0; i < len; ++i)
    {
      ab306_cout (p, ((const u_char *) buf)[i]);
      cksum += ((const u_char *) buf)[i];
    }

  DBG(4, "ab306_write: writing checksum\n");
  ab306_cout (p, -cksum & 0xff);

  DBG(3, "ab306_write: waiting for scanner to be NOT ready %02x\n",
      ab306_inb (p, base + 1));
  while ((ab306_inb (p, base + 1) & 0x20) != 0);
  usleep (10000);

  DBG(4, "ab306_write: reading ack\n");
  cksum = ab306_cin (p);
  if (cksum != 0xa5)
    {
      DBG(0, "ab306_write: checksum error (%02x!=a5) when sending command!\n",
	  cksum);
      return SANE_STATUS_IO_ERROR;
    }
  return SANE_STATUS_GOOD;
}

/* Abort a running scan by pulling C6 low for a while.  */
static void
ab306_abort (Port *p)
{
  ab306_outb (p, p->base + 1, 0x20);
  while ((ab306_inb (p, p->base + 1) & 0x80));
  ab306_outb (p, p->base + 1, 0x60);
}

/* Open the device, <dev> must contain a valid port number (as string)
   returns port number and I/O method in <*fdp> (not a file
   descriptor) turns the scanner on setting C5 and C6.  */
SANE_Status
sanei_ab306_open (const char *dev, int *fdp)
{
  static int first_time = 1;
  SANE_Status status;
  u_char byte;
  u_long base;
  char *end;
  int i, j;

  if (first_time)
    {
      first_time = 0;
      DBG_INIT();
    }

  base = strtol (dev, &end, 0);
  if (end == dev || *end)
    {
      DBG(1, "sanei_ab306_open: `%s' is not a valid port number\n", dev);
      return SANE_STATUS_INVAL;
    }

  for (i = 0; i < NELEMS(port); ++i)
    if (port[i].base == base)
      break;

  if (port[i].base != base)
    {
      DBG(1, "sanei_ab306_open: %lx is not a valid base address\n", base);
      return SANE_STATUS_INVAL;
    }

  if (port[i].in_use)
    {
      DBG(1, "sanei_ab306_open: port %lx is already in use\n", base);
      return SANE_STATUS_DEVICE_BUSY;
    }

  status = sanei_ab306_get_io_privilege (i);

#if defined(__FreeBSD__)
  status = sanei_ab306_get_io_privilege (i);
  if (status != SANE_STATUS_GOOD)
    return status;
  
  DBG(1, "sanei_ab306_ioport: using inb/outb access\n");
  for (j = 0; j < NELEMS(wakeup); ++j)
    {
      byte = wakeup[j];
      if (j == NELEMS(wakeup) - 1)
	byte |= i;
      outb (byte, AB306_CIO);
    }

#else /* !defined(__FreeBSD__) */
  if (ioperm (AB306_CIO, 1, 1) != 0)
    {
      DBG(1, "sanei_ab306_ioport: using /dev/port access\n");
      if (port[i].port_fd < 0)
	port[i].port_fd = open (PORT_DEV, O_RDWR);
      if (port[i].port_fd < 0)
	return SANE_STATUS_IO_ERROR;
      for (j = 0; j < NELEMS(wakeup); ++j)
	{
	  if (lseek (port[i].port_fd, AB306_CIO, SEEK_SET) != AB306_CIO)
	    return SANE_STATUS_IO_ERROR;
	  byte = wakeup[j];
	  if (j == NELEMS(wakeup) - 1)
	    byte |= i;
	  if (write (port[i].port_fd, &byte, 1) != 1)
	    return SANE_STATUS_IO_ERROR;
	}
    }
  else
    {
      DBG(1, "sanei_ab306_ioport: using inb/outb access\n");
      for (j = 0; j < NELEMS(wakeup); ++j)
	{
	  byte = wakeup[j];
	  if (j == NELEMS(wakeup) - 1)
	    byte |= i;
	  outb (byte, AB306_CIO);
	}
      status = sanei_ab306_get_io_privilege (i);
      if (status != SANE_STATUS_GOOD)
	return status;
    }
#endif /* !defined(__FreeBSD__) */

  ab306_outb (port + i, port[i].base + 1, 0x60);
  port[i].in_use = 1;
  port[i].active = 1;
  *fdp = i;
  return SANE_STATUS_GOOD;
}

void
sanei_ab306_close (int fd)
{
  Port *p = port + fd;

  if (p->in_use)
    {
      if (p->port_fd >= 0)
	{
	  close (p->port_fd);
	  p->port_fd = -1;
	}
      p->in_use = 0;
    }
}

/* Get I/O permission to the configuration port and the desired
   operating ports.  */
SANE_Status
sanei_ab306_get_io_privilege (int fd)
{
  if (port[fd].port_fd < 0)
    {
#if defined(__FreeBSD__)
      if (dev_io_fd == 0)
	dev_io_fd = open ("/dev/io", O_RDONLY);
      if (dev_io_fd < 0)
        return SANE_STATUS_IO_ERROR;
#else /* !defined(__FreeBSD__) */
      if (ioperm (port[fd].base, 3, 1) != 0)
	return SANE_STATUS_IO_ERROR;
#endif /* !defined(__FreeBSD__) */
    }
  return SANE_STATUS_GOOD;
}

/* Send a command via the SANEI_AB306N-interface, get response when
   <dst_size> is > 0.  */
SANE_Status
sanei_ab306_cmd (int fd, const void *src, size_t src_size,
		 void *dst, size_t * dst_size)
{
  Port *p = port + fd;
  const u_char *cp = src;
  size_t cdb_size = CDB_SIZE(cp[0]);
  SANE_Status status;
  u_char byte;

  /* If this is a READ_SCANNED_DATA command, reset lstat: */
  switch (cp[0])
    {
    case 0x08:	/* scsi READ_SCANNED_DATA command */
      /* Initialize lstat to the current status, because we need bit 4
	 (0x10) as toggle bit for reading lines.  */
      p->lstat = 0x34;
      break;

    case 0x1b:	/* scsi START_STOP command */
      if (!cp[4])
	{
	  /* it's a STOP */
	  ab306_abort (p);
	  return SANE_STATUS_GOOD;
	}
      break;

    default:
      break;
    }

  status = ab306_write (p, src, 6);
  if (status != SANE_STATUS_GOOD)
    return status;

  if (src_size > cdb_size)
    {
      status = ab306_write (p, cp + cdb_size, src_size - cdb_size);
      if (status != SANE_STATUS_GOOD)
	return status;
    }

  if (dst && *dst_size > 0)
    {
      u_int i, cksum = 0;

      DBG(3, "sanei_ab306_cmd: waiting for scanner to be NOT ready %02x\n",
	  ab306_inb (p, p->base + 1));
      while ((ab306_inb (p, p->base + 1) & 0x20) != 0);

      for (i = 0; i < *dst_size; i++)
	{
	  byte = ab306_cin (p);
	  cksum += byte;
	  ((u_char *) dst)[i] = byte;
	}
      cksum += ab306_cin (p);		/* add in checksum */

      if ((cksum & 0xff) != 0)
	{
	  DBG(0, "sanei_ab306_cmd: checksum error (%2x!=0) when receiving "
	      "after command!\n", cksum);
	  return SANE_STATUS_IO_ERROR;
	}
      ab306_cout (p, 0);		/* dummy byte (will be discarded) */
    }
  return SANE_STATUS_GOOD;
}

/* Read scan-data from the AB306N-device. Read <lines> lines, of which
   every one has <bpl> bytes.  */
SANE_Status
sanei_ab306_rdata (int fd, int planes, SANE_Byte * buf, int lines, int bpl)
{
  Port *p = port + fd;
  int lcnt, pcnt, bcnt, xmax;
  SANE_Byte *lsave_bp;
  int nstat;

  DBG(2, "sanei_ab306_rdata: start\n");

  /* lstat should be set by a call to sanei_ab306_init_toggle before ! */
  while ((ab306_inb (p, p->base + 1) & 0x80) == 0);
  /* the lines-loop: */
  for (lcnt = 0; lcnt < lines; ++lcnt)
    {
      lsave_bp = buf;
      /* the planes-loop: */
      for (pcnt = 0; pcnt < planes; ++pcnt)
	{
	  xmax = bpl / planes;
	  do
	    nstat = ab306_inb (p, p->base + 1);
	  while (((p->lstat ^ nstat) & 0x10) == 0);
	  
	  if (p->port_fd >= 0)
	    {
	      /* the pixel-loop: */
	      for (bcnt = 0; bcnt < xmax; bcnt++)
		{
		  if ((u_long) lseek (p->port_fd, p->base, SEEK_SET) != p->base)
		    return SANE_STATUS_IO_ERROR;
		  if (read (p->port_fd, buf, 1) != 1)
		    return SANE_STATUS_IO_ERROR;
		  ++buf;
		}
	    }
	  else
	    {
	      /* the pixel-loop: */
	      for (bcnt = 0; bcnt < xmax; bcnt++)
		{
		  *(u_char *) buf = inb (p->base);
		  ++buf;
		}
	    }
	  p->lstat = nstat;
	}
    }
  DBG(2, "sanei_ab306_rdata: done\n");
  return SANE_STATUS_GOOD;
}

void
sanei_ab306_exit (void)
{
  int i;

  for (i = 0; i < NELEMS(port); ++i)
    if (port[i].active)
      {
	port[i].active = 0;
	/* power off the scanner: */
	ab306_outb (port + i, port[i].base + 1, 0x00);
      }
#if defined(__FreeBSD)
  if (dev_io_fd >0)
    close (dev_io_fd);
#endif /* defined(__FreeBSD__) */
}

SANE_Status
sanei_ab306_test_ready (int fd)
{
  Port *p = port + fd;
  u_char byte;

  byte = ab306_inb (p, p->base + 1);
  if (byte & 0x20)
    return SANE_STATUS_GOOD;

  return SANE_STATUS_DEVICE_BUSY;
}

#else /* !HAVE_IOPERM */

SANE_Status
sanei_ab306_open (const char *devname, int *fdp)
{
  *fdp = -1;
  return SANE_STATUS_INVAL;
}

void
sanei_ab306_close (int fd)
{
}

void
sanei_ab306_exit (void)
{
}

SANE_Status
sanei_ab306_get_io_privilege (int fd)
{
  return SANE_STATUS_INVAL;
}

SANE_Status
sanei_ab306_test_ready (int fd)
{
  return SANE_STATUS_GOOD;	/* non-existent device is always ready... */
}

SANE_Status
sanei_ab306_cmd (int fd, const void *src, size_t src_size,
		 void *dst, size_t *dst_size)
{
  return SANE_STATUS_INVAL;
}

SANE_Status
sanei_ab306_rdata (int fd, int planes, SANE_Byte *buf, int lines, int bpl)
{
  return SANE_STATUS_INVAL;
}

#endif /* !HAVE_IOPERM */
