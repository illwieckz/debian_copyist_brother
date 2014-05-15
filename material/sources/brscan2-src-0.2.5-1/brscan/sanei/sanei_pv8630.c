/* sane - Scanner Access Now Easy.

   Copyright (C) 2000 Adrian Perez Jorge
   Copyright (C) 2001 Frank Zago
   Copyright (C) 2001 Marcio Teixeira

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

   Interface files for the PowerVision 8630 chip, a USB to
   parallel converter used in many scanners.

 */

#include "../include/sane/config.h"

#include <stdlib.h>
#include <unistd.h>

#define BACKEND_NAME	sanei_pv8630
#include "../include/sane/sane.h"
#include "../include/sane/sanei_debug.h"
#include "../include/sane/sanei_usb.h"
#include "../include/sane/sanei_pv8630.h"

#define DBG_error   1

/* Write one control byte */
SANE_Status
sanei_pv8630_write_byte (int fd, SANEI_PV_Index index, SANE_Byte byte)
{
  SANE_Status status;

  status =
    sanei_usb_control_msg (fd, 0x40, PV8630_REQ_WRITEBYTE, byte, index, 0,
			   NULL);

  if (status != SANE_STATUS_GOOD)
    DBG (DBG_error, "sanei_pv8630_write_byte error\n");
  return status;
}

/* Read one control byte */
SANE_Status
sanei_pv8630_read_byte (int fd, SANEI_PV_Index index, SANE_Byte * byte)
{
  SANE_Status status;

  status =
    sanei_usb_control_msg (fd, 0xc0, PV8630_REQ_READBYTE, 0, index, 1, byte);

  if (status != SANE_STATUS_GOOD)
    DBG (DBG_error, "sanei_pv8630_read_byte error\n");
  return status;
}

/* Prepare a bulk read. len is the size of the data going to be
 * read by pv8630_bulkread(). */
SANE_Status
sanei_pv8630_prep_bulkread (int fd, int len)
{
  SANE_Status status;

  status =
    sanei_usb_control_msg (fd, 0x40, PV8630_REQ_EPPBULKREAD, len & 0xffff,
			   len >> 16, 0, NULL);

  if (status != SANE_STATUS_GOOD)
    DBG (DBG_error, "sanei_pv8630_prep_bulkread error\n");
  return status;
}

/* Prepare a bulk write. len is the size of the data going to be
 * written by pv8630_bulkwrite(). */
SANE_Status
sanei_pv8630_prep_bulkwrite (int fd, int len)
{
  SANE_Status status;

  status =
    sanei_usb_control_msg (fd, 0x40, PV8630_REQ_EPPBULKWRITE, len & 0xffff,
			   len >> 16, 0, NULL);

  if (status != SANE_STATUS_GOOD)
      DBG (DBG_error, "sanei_pv8630_prep_bulkwrite error\n");
  return status;
}

/* Flush the buffer. */
SANE_Status
sanei_pv8630_flush_buffer (int fd)
{
  SANE_Status status;

  status =
    sanei_usb_control_msg (fd, 0x40, PV8630_REQ_FLUSHBUFFER, 0, 0, 0, NULL);

  if (status != SANE_STATUS_GOOD)
    DBG (DBG_error, "sanei_pv8630_flush_buffer error\n");
  return status;
}

/* Do a bulk write. The length must have previously been sent via
 * pv8630_prep_bulkwrite(). */
SANE_Status
sanei_pv8630_bulkwrite (int fd, const void *data, size_t * len)
{
  SANE_Status status;

  status = sanei_usb_write_bulk (fd, (char *) data, len);

  if (status != SANE_STATUS_GOOD)
    DBG (DBG_error, "sanei_pv8630_bulkwrite error\n");
  return status;
}

/* Do a bulk read. The length must have previously been sent via
 * pv8630_prep_bulkread(). */
SANE_Status
sanei_pv8630_bulkread (int fd, void *data, size_t * len)
{
  SANE_Status status;

  status = sanei_usb_read_bulk (fd, data, len);

  if (status != SANE_STATUS_GOOD)
    DBG (DBG_error, "sanei_pv8630_bulkread error\n");
  return status;
}

/* Expects a specific byte in a register */
SANE_Status
sanei_pv8630_xpect_byte (int fd, SANEI_PV_Index index, SANE_Byte value,
			 SANE_Byte mask)
{
  SANE_Status status;
  SANE_Byte s;

  status = sanei_pv8630_read_byte (fd, index, &s);
  if (status != SANE_STATUS_GOOD)
      return status;

  if ((s & mask) != value)
    {
      DBG (DBG_error, "sanei_pv8630_xpect_byte: expected %x, got %x\n", value,
	   s);
      return SANE_STATUS_IO_ERROR;
    }
  return SANE_STATUS_GOOD;
}

/* Wait for the status register to present a given status. A timeout value
   is given in tenths of a second. */
SANE_Status
sanei_pv8630_wait_byte (int fd, SANEI_PV_Index index, SANE_Byte value,
			SANE_Byte mask, int timeout)
{
  SANE_Status status;
  SANE_Byte s;
  int n;

  for (n = 0; n < timeout; n++)
    {

      status = sanei_pv8630_read_byte (fd, index, &s);
      if (status != SANE_STATUS_GOOD)
	return status;

      if ((s & mask) == value)
	return SANE_STATUS_GOOD;

      usleep (100000);
    }

  DBG (DBG_error, "sanei_pv8630_wait_byte: timeout waiting for %x (got %x)\n",
       value, s);
  return SANE_STATUS_IO_ERROR;
}
