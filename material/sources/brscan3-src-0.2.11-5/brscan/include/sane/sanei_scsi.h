/* sane - Scanner Access Now Easy.
   Copyright (C) 1996, 1997 David Mosberger-Tang
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

   Generic interface to SCSI drivers.  */

#ifndef sanei_scsi_h
#define sanei_scsi_h

#include <sys/types.h>

#include <sane/sane.h>

typedef SANE_Status (*SANEI_SCSI_Sense_Handler) (int fd,
						 u_char *sense_buffer,
						 void *arg);

extern int sanei_scsi_max_request_size;

/* Find each SCSI devices that matches the pattern specified by the
   arguments.  String arguments can be "omitted" by passing NULL,
   integer arguments can be "omitted" by passing -1.  Callback ATTACH
   gets invoked once for each device.  The DEV argument passed to this
   callback is the real devicename.

   Example: vendor="HP" model=NULL, type=NULL, bus=3, id=-1, lun=-1 would
	    attach all HP devices on SCSI bus 3.  */
extern void sanei_scsi_find_devices (const char *vendor, const char *model,
				     const char *type,
				     int bus, int channel, int id, int lun,
				     SANE_Status (*attach) (const char *dev));

extern SANE_Status sanei_scsi_open (const char * device_name, int * fd,
				    SANEI_SCSI_Sense_Handler sense_handler,
				    void *sense_arg);

/* The extended open call allows a backend to ask for a specific
   buffer size. sanei_scsi_open tries to allocate a buffer of the
   size given by *buffersize upon entry to this function. If
   sanei_scsi_open_extended returns successfully, *buffersize
   contains the available buffer size. This value may be both
   smaller or larger than the value requested by the backend;
   it can even be zero. The backend must decide, if it got enough
   buffer memory to work.

   Note that the value of *buffersize may differ for different
   files.
*/
extern SANE_Status sanei_scsi_open_extended (
       const char * device_name, int * fd,
       SANEI_SCSI_Sense_Handler sense_handler,
       void *sense_arg, int *buffersize);

/* Let backends decide, which open call to use:
   if HAVE_SANEI_SCSI_OPEN_EXTENDED is defined, sanei_scsi_open_extended
   may be used.
   May also be used to decide, if sanei_scsi_req_flush_all or
   sanei_scsi_req_flush_all_extended should be used.
*/
#define HAVE_SANEI_SCSI_OPEN_EXTENDED

/* One or more scsi commands can be enqueued by calling req_enter().
   SRC is the pointer to the SCSI command and associated write data
   and SRC_SIZE is the length of the command and data.  DST is a
   pointer to a buffer in which data is returned (if any).  It may be
   NULL if no data is returned by the command.  On input *DST_SIZE is
   the size of the buffer pointed to by DST, on exit, *DST_SIZE is set
   to the number of bytes returned in the buffer (which is less than
   or equal to the buffer size).  DST_SIZE may be NULL if no data is
   expected.  IDP is a pointer to a void* that uniquely identifies
   the entered request.

   NOTE: Some systems may not support multiple outstanding commands.
   On such systems, enter() may block.  In other words, it is not
   proper to assume that enter() is a non-blocking routine.  

   src points to a buffer which contains the SCSI command followed
   by data to be written to the device (if any)
*/
extern SANE_Status sanei_scsi_req_enter (int fd,
					 const void * src, size_t src_size,
					 void * dst, size_t * dst_size,
					 void **idp);

/* Same as above, but with separate buffers for the SCSI command and
   for the data to be sent to the device.

   With sanei_scsi_req_enter, the length of te SCSI command block must be
   guessed. While that works in most cases, Canon scanners for example
   use the vendor specific commands 0xd4, 0xd5 and 0xd6. The Canon 
   scanners want to get 6 byte command blocks for these commands, 
   but sanei_scsi_req_enter and sanei_scsi_cmd send 12 bytes. 

   cmd:      pointer to SCSI command
   cmd_size: size of the command
   src:      pointer to the buffer with data to be sent rto the scanner
   src_size: size of that buffer
   dst, dst_size, idp: see above
   
   If dst_size and *dst_size are non-zero, a "read command" (ie,
   data transfer from the device to the host) is assumed.
*/
extern SANE_Status sanei_scsi_req_enter2 (int fd,
					 const void * cmd, size_t cmd_size,
					 const void * src, size_t src_size,
					 void * dst, size_t * dst_size,
					 void **idp);

/* Wait for the completion of the SCSI command with id ID.  */
extern SANE_Status sanei_scsi_req_wait (void *id);

/* This is a convenience function that is equivalent to a pair of
   enter()/wait() calls.  */
extern SANE_Status sanei_scsi_cmd (int fd,
				   const void * src, size_t src_size,
				   void * dst, size_t * dst_size);

extern SANE_Status sanei_scsi_cmd2 (int fd,
				   const void * cmd, size_t cmd_size,
				   const void * src, size_t src_size,
				   void * dst, size_t * dst_size);

/* Flush all pending SCSI commands. This function work only,
   if zero or one SCSI file handles are open.
*/
extern void sanei_scsi_req_flush_all (void);

/* Flush all SCSI commands pending for one handle */
extern void sanei_scsi_req_flush_all_extended (int fd);

extern void sanei_scsi_close (int fd);

#endif /* sanei_scsi_h */
