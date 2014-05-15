/* sane - Scanner Access Now Easy.
   Copyright (C) 1997 Andreas Czechanowski
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
   If you do not wish that, delete this exception notice.  */


/** @file sanei_ab306.h
 * Support for the Mustek-proprietary SCSI-over-parallel-port
 * interface AB306.
 *
 * This chip is used by the Mustek Paragon 600 II N. It's similar to a parport
 * but has it's own ISA card and cable. 
 * 
 * /dev/port access is used where available. If this doesn't work, inb/outb
 * functions or inb/outb assembler code is used.
 *
 * Depending on the operating system it may be necessary to run this code as
 * root.
 */


#ifndef sanei_ab306_h
#define sanei_ab306_h

#include <sys/types.h>

#include <sane/sane.h>

/**
 * Open the connection to an AB306 device.
 *
 * The scanner is also turned on.
 *
 * @param dev Port address as text.
 * @param fd  Information about port address and I/O method. fd is not a file
 *            descriptor. The name and type are used for compatibility reasons.
 *
 * @return
 * - SANE_STATUS_GOOD - on success
 * - SANE_STATUS_INVAL - if the port address can't be interpreted
 * - SANE_STATUS_IO_ERROR - if the device file for a port couldn't be accessed
 */
SANE_Status	sanei_ab306_open (const char *dev, int *fd);

/**
 * Close the connection to an AB306 device.
 *
 * @param fd  Information about port address and I/O method.
 *
 */
void		sanei_ab306_close (int fd);

/**
 * Exit ab306.
 *
 * Also powers down all devices.
 *
 */
void		sanei_ab306_exit (void);

/**
 * Get the permission for direct access to the ports.
 *
 * The only occasion this function must be called, is when the I/O privilege
 * was lost, e.g. after forking. Otherwise, it's called automatically by
 * sanei_ab306_open().
 *
 * @param fd  Information about port address and I/O method.
 *
 * @return
 * - SANE_STATUS_GOOD - on success
 * - SANE_STATUS_IO_ERROR - if the i/o privilege was denied by the operating
 *   system
 */
SANE_Status	sanei_ab306_get_io_privilege (int fd);
/**
 * Find out if the device is ready to accept new commands.
 *
 * @param fd  Information about port address and I/O method.
 *
 * @return
 * - SANE_STATUS_GOOD - if the device is ready
 * - SANE_STATUS_DEVICE_BUSY if the device is still busy (try again later)
 */
SANE_Status	sanei_ab306_test_ready (int fd);

/**
 * Send a command to the AB306 device.
 *
 * @param fd  Information about port address and I/O method.
 * @param src Data to be sent to the device.
 * @param src_size Size of data to be sent to the device.
 * @param dst Data to be received from the device.
 * @param dst_size Size of data to be received from the device
 *
 * @return
 * - SANE_STATUS_GOOD - on success
 * - SANE_STATUS_IO_ERROR - if an error occured during the dialog with the
 *   device
 */
SANE_Status	sanei_ab306_cmd (int fd, const void *src, size_t src_size,
				 void *dst, size_t *dst_size);

/**
 * Read scanned image data.
 *
 * @param fd  Information about port address and I/O method.
 * @param planes Bytes per pixel (3 for color, 1 for all other modes)
 * @param buf Buffer for image data.
 * @param lines Number of lines
 * @param bpl Bytes per line
 *
 * @return
 * - SANE_STATUS_GOOD - on success
 * - SANE_STATUS_IO_ERROR - if an error occured during the dialog with the 
 *   device
 */
SANE_Status	sanei_ab306_rdata (int fd, int planes,
				   SANE_Byte *buf, int lines, int bpl);

#endif /* sanei_ab306_h */
