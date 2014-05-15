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

#ifndef sanei_pv8630_h
#define sanei_pv8630_h

#include "../include/sane/config.h"
#include "../include/sane/sane.h"

typedef enum
{
  PV8630_REQ_READBYTE = 0x00,
  PV8630_REQ_WRITEBYTE = 0x01,
  PV8630_REQ_EPPBULKREAD = 0x02,
  PV8630_REQ_EPPBULKWRITE = 0x03,
  PV8630_REQ_FLUSHBUFFER = 0x04,
  PV8630_REQ_ENABLEINTERRUPT = 0x05,
  PV8630_REQ_DISABLEINTERRUPT = 0x06,
  PV8630_REQ_READWORD = 0x08,
  PV8630_REQ_WRITEWORD = 0x09
}
SANEI_PV_Request;

typedef enum
{
  PV8630_RDATA = 0x00,
  PV8630_REPPADDRESS = 0x01,
  PV8630_UNKNOWN = 0x02,
  PV8630_RMODE = 0x03,
  PV8630_RSTATUS = 0x04
}
SANEI_PV_Index;

extern SANE_Status sanei_pv8630_write_byte (int fd, SANEI_PV_Index index,
					    SANE_Byte byte);
extern SANE_Status sanei_pv8630_read_byte (int fd, SANEI_PV_Index index,
					   SANE_Byte * byte);
extern SANE_Status sanei_pv8630_prep_bulkread (int fd, int len);
extern SANE_Status sanei_pv8630_prep_bulkwrite (int fd, int len);
extern SANE_Status sanei_pv8630_flush_buffer (int fd);
extern SANE_Status sanei_pv8630_bulkwrite (int fd, const void *data,
					   size_t * len);
extern SANE_Status sanei_pv8630_bulkread (int fd, void *data, size_t * len);
extern SANE_Status sanei_pv8630_xpect_byte (int fd, SANEI_PV_Index index,
					    SANE_Byte value, SANE_Byte mask);
extern SANE_Status sanei_pv8630_wait_byte (int fd, SANEI_PV_Index index,
					   SANE_Byte value, SANE_Byte mask,
					   int timeout);
#endif /* sanei_pv8630_h */
