/* sane - Scanner Access Now Easy.
   Copyright (C) 2001 Henning Meier-Geinitz
   This file is part of the SANE package.

   SANE is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   SANE is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with sane; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/** @file sanei_usb.h
 * This file provides a generic USB interface.  
 *
 * Currently, only access to device files as provided by the Linux USB
 * scanner driver is supported. FreeBSD and OpenBSD with their uscanner 
 * drivers also work. However, detection and control messages aren't
 * supported on these platforms.
 *
 * See <a 
 * href="http://www.mostang.com/sane/man/sane-usb.5.html">man sane-usb(5)</a>
 * for user-oriented documentation.
 */

#ifndef sanei_usb_h
#define sanei_usb_h

#include "../include/sane/config.h"
#include "../include/sane/sane.h"

/** Initialize sanei_usb.
 *
 * Call this before any other sanei_usb function.
 */
extern void
sanei_usb_init (void);

/** Get the vendor and product ids.
 *
 * Currently, only scanners supported by the Linux USB scanner module can be
 * found. The Linux version must be 2.4.8 or higher. 
 *
 * @param fd device file descriptor
 * @param vendor vendor id
 * @param product product id
 *
 *@return 
 * - SANE_STATUS_GOOD - if the ids could be determinded
 * - SANE_STATUS_UNSUPPORTED - if the OS doesn't support detection of ids
 */
extern SANE_Status
sanei_usb_get_vendor_product (SANE_Int fd, SANE_Word * vendor,
			      SANE_Word * product);

/** Find devices that match given vendor and product ids.
 *
 * For limitations, see function sanei_usb_get_vendor_product().
 * The function attach is called for every device which has been found.
 *
 * @param vendor vendor id
 * @param product product id
 * @param attach attach function
 *
 * @return SANE_STATUS_GOOD - on success (even if no scanner was found)
 */
extern SANE_Status
sanei_usb_find_devices (SANE_Int vendor, SANE_Int product,
			SANE_Status (*attach) (SANE_String_Const devname));

/** Open a USB device.
 *
 * The device is opened by its filename devname and the file descriptor is
 * returned in fd on success.
 *
 * @param devname filename of the device file
 * @param fd device file descriptor
 *
 * @return
 * - SANE_STATUS_GOOD - on success
 * - SANE_STATUS_ACCESS_DENIED - if the file couldn't be accessed due to
 *   permissions
 * - SANE_STATUS_INVAL - on every other error
 */
extern SANE_Status
sanei_usb_open (SANE_String_Const devname, SANE_Int *fd);

/** Close a USB device.
 * 
 * @param fd file descriptor of the device
 */
extern void
sanei_usb_close (SANE_Int fd);

/** Initiate a bulk transfer read.
 *
 * Read up to size bytes from the device to buffer. After the read, size
 * contains the number of bytes actually read.
 *
 * @param fd file descriptor of the device
 * @param buffer buffer to store read data in
 * @param size size of the data
 *
 * @return 
 * - SANE_STATUS_GOOD - on succes
 * - SANE_STATUS_EOF - if zero bytes have been read
 * - SANE_STATUS_IO_ERROR - if an error occured during the read
 * - SANE_STATUS_INVAL - on every other error
 *
 */ 
extern SANE_Status
sanei_usb_read_bulk (SANE_Int fd, SANE_Byte * buffer, size_t *size);

/** Initiate a bulk transfer write.
 *
 * Write up to size bytes from buffer to the device. After the write size
 * contains the number of bytes actually written.
 *
 * @param fd file descriptor of the device
 * @param buffer buffer to write to device
 * @param size size of the data
 *
 * @return 
 * - SANE_STATUS_GOOD - on succes
 * - SANE_STATUS_IO_ERROR - if an error occured during the write
 * - SANE_STATUS_INVAL - on every other error
 */
extern SANE_Status
sanei_usb_write_bulk (SANE_Int fd, SANE_Byte * buffer, size_t *size);

/** Send/receive a control message to/from a USB device.
 *
 * This function is only supported for Linux 2.4.13 and newer.
 * The arguments rtype, req, value, and index are passed directly to
 * usb_control_msg (see the <a href="http://www.linux-usb.org">Linux
 * USB project</a> and <a href="http://www.usb.org/developers/docs.html">
 * www.usb.org developers information</a> for documentation).
 * 
 * @param fd file descriptor of the device file
 * @param data buffer to send/receive data
 * @param len length of data to send/receive
 * @param rtype specifies the characteristics of the request (e.g. data
 *    direction)
 * @param req actual request
 * @param value parameter specific to the request
 * @param index parameter specific to the request (often used to select
 *     endpoint)
 *
 * @return
 * - SANE_STATUS_GOOD - on success
 * - SANE_STATUS_IO_ERROR - on error
 * - SANE_STATUS_UNSUPPORTED - if the feature is not supported by the OS or
 *   SANE.
 */
extern SANE_Status
sanei_usb_control_msg (SANE_Int fd, SANE_Int rtype, SANE_Int req,
		       SANE_Int value, SANE_Int index, SANE_Int len,
		       SANE_Byte *data);

/** Expand device name patterns into a list of devices.
 *
 * Apart from a normal device name (such as /dev/usb/scanner0), this
 * function currently supports USB device specifications of the form:
 *
 *	usb VENDOR PRODUCT
 *
 * VENDOR and PRODUCT are non-negative integer numbers in decimal or
 * hexadecimal format. A similar function for SCSI devices can be found
 * in include/sane/config.h.
 *
 * @param name device name pattern
 * @param attach attach function
 *
 */
extern void 
sanei_usb_attach_matching_devices (const char *name,
				   SANE_Status (*attach) (const char *dev));

#endif /* sanei_usb_h */
