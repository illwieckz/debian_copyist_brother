/* sane - Scanner Access Now Easy.

   based on sources acquired from Plustek Inc.
   Copyright (C) 2002 Gerhard Jaeger <g.jaeger@earthling.net>

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

*/

/** @file sanei_lm983x.h
 *  Interface files for the NS LM9831/2/3 USB chip.
 *
 *  The National Semiconductor LM9831, LM9832, and LM9833 chips are used in
 *  many USB scanners. Examples include Plustek and Mustek devices.
 */

#ifndef sanei_lm983x_h
#define sanei_lm983x_h

#include "../include/sane/config.h"
#include "../include/sane/sane.h"

/**
 * Read one data byte from a specific LM983x register.
 *
 * @param fd    - device file descriptor (SANE_Int)
 * @param reg   - number of register (SANE_Byte)
 * @param value - byte value to be written (SANE_Byte *)
 *
 * @return The SANE status code for the operation (SANE_Status):
 * - SANE_STATUS_GOOD     - on success
 * - SANE_STATUS_IO_ERROR - system write function failed
 * - SANE_STATUS_INVAL    - register out of range
 */
#define sanei_lm983x_read_byte(fd, reg, value) \
          sanei_lm983x_read (fd, reg, value, 1, 0)

/**
 * Initialize sanei_lm983x.
 *
 * Currently, this function only enables the debugging functionality.
 */
extern void sanei_lm983x_init( void );

/**
 * Write one data byte to a specific LM983x register.
 *
 * @param fd    - device file descriptor
 * @param reg   - number of register
 * @param value - byte value to be written
 *
 * @return
 * - SANE_STATUS_GOOD     - on success
 * - SANE_STATUS_IO_ERROR - system write function failed
 * - SANE_STATUS_INVAL    - register out of range
 */
extern SANE_Status sanei_lm983x_write_byte( SANE_Int fd,
                                            SANE_Byte reg, SANE_Byte value );

/**
 * Write one or more data bytes to one or more specific LM983x
 * registers.
 *
 * @param fd        - device file descriptor
 * @param reg       - number of start-register
 * @param buffer    - buffer to be written
 * @param len       - number of bytes to be written
 * @param increment - SANE_TRUE enables the autoincrement of the register
 *                    value during the write cycle, SANE_FALSE disables this
 *
 * @return
 * - SANE_STATUS_GOOD     - on success
 * - SANE_STATUS_IO_ERROR - system read function failed
 * - SANE_STATUS_INVAL    - register out of range or len field was 0
 */
extern SANE_Status sanei_lm983x_write( SANE_Int fd, SANE_Byte reg,
                                       SANE_Byte *buffer, SANE_Word len,
                                                         SANE_Bool increment );

/**
 * Read one or more data bytes from one ore more specific LM983x
 * registers.
 *
 * @param fd        - device file descriptor
 * @param reg       - number of start-register
 * @param buffer    - buffer to receive the data
 * @param len       - number of bytes to receive
 * @param increment - SANE_TRUE enables the autoincrement of the register
 *                    value during the read cylce, SANE_FALSE disables this
 *
 * @return
 * - SANE_STATUS_GOOD     - on success
 * - SANE_STATUS_IO_ERROR - system read function failed
 * - SANE_STATUS_INVAL    - register out of range
 * - SANE_STATUS_EOF      - if nothing can't be read
 */
extern SANE_Status sanei_lm983x_read( SANE_Int fd, SANE_Byte reg,
                                      SANE_Byte *buffer, SANE_Word len,
                                                         SANE_Bool increment );

/**
 * Reset the LM983x chip.
 *
 * @param fd    - device file descriptor
 * @return 
 * - SANE_TRUE  - reset successfuly done
 * - SANE_FALSE - reset failed
 */
extern SANE_Bool sanei_lm983x_reset( SANE_Int fd );
					    					
#endif /* sanei_lm983x_h */

