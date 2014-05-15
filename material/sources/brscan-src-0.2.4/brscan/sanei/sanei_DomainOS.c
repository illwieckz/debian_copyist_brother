/* sane - Scanner Access Now Easy.
   Copyright (C) 1996, 1997 David Mosberger-Tang
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

   This file defines a server for Apollo Domain/OS systems.  It does all
of the scsi_$ calls that are needed for SANE.  This is necessary because
Domain/OS will not allow a child process to access a parent's SCSI
device.  The interface is through a common, mapped area.  Mutex locks
are used to prevent concurrency problems, and eventcounts are used to
notify a waiting process that its request has completed.

    This program is intended to support only one device at a time,
although multiple instances of this program may run concurrently.  It is
intended that this program be forked/execd by a SANE application, and
that it will exit when the application exits.

    Upon startup, the program is invoked with the path to an object that
needs to be mapped for communication.  The parent process will have
already initialized the 'public' eventcounts and locks, and will be
waiting for the ResultReady eventcount to be incremented.  After
initialization, the server will increment this eventcount, and wait for
an incoming request, which is signified by the CommandAvailable
eventcount.  This EC will be incremented after another process has
filled in the parameter area.

DBG levels:
    0   Error - always printed.
    1   Basic monitor - print entry to main functions, or errors that are
         normally suppressed because they are reported at a higher level.
    2   Medium monitor - show intermediate steps in functions
    3   Detailed monitor - if its there, print it

*/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <apollo/base.h>
#include <apollo/ec2.h>
#include <apollo/error.h>
#include <apollo/fault.h>
#include <apollo/ms.h>
#include <apollo/mutex.h>
#include <apollo/pfm.h>
#include <apollo/scsi.h>

#include "sane/config.h"

#include "sane/sanei_scsi.h"

#include "sane/sanei_debug.h"

#include "sanei_DomainOS.h"

/* Timeout period for SCSI wait, in milliseconds.
We are using 100 seconds here. */
#define DomainScsiTimeout 100000

/* Communication Area pointer */
struct DomainServerCommon *com;

/* Handle for fault handler */
pfm_$fh_handle_t FaultHandle;


static struct
   {
   void *DomainSCSIPtr;         /* Pointer to the data block for this device */
   void *DomainSensePtr;        /* Pointer to the sense area for this device */
   u_int in_use  : 1;           /* is this DomainFdInfo in use? */
   u_int fake_fd : 1;           /* is this a fake file descriptor? */
   scsi_$handle_t scsi_handle;  /* SCSI handle */
   scsi_$operation_id_t op_id;  /* op_id of current request */
   } *DomainFdInfo;

/* This function is called error might have occured, but it would be one that I
don't know how to handle, or never expect to happen.  */
static void DomainErrorCheck(status_$t status, const char *message)
   {
   char *subsystem, *module, *code;
   short subsystem_length, module_length, code_length;

   if (status.all)
      {
      DBG(0, "Unrecoverable Domain/OS Error 0x%08x:  %s\n", status.all, message);
      error_$find_text(status, &subsystem, &subsystem_length, &module, &module_length, &code, &code_length);
      if (subsystem_length && module_length && code_length)
         DBG(0, "%.*s (%.*s/%.*s)\n", code_length, code, subsystem_length, subsystem, module_length, module);
      exit(EXIT_FAILURE);
      }
   }


/* This function is the fault handler for the server.  Currently, it only
handles asynchronous faults.  It always returns to the faulting code, but
it disables the handler, so that the server can be killed if the parent is
unable to do so. */
pfm_$fh_func_val_t FaultHandler(pfm_$fault_rec_t *FaultStatusPtr)
   {
   status_$t status;

   DBG(1, "In fault handler, status is %08x\n", FaultStatusPtr->status.all);
   switch (FaultStatusPtr->status.all)
      {
      case fault_$quit:
         pfm_$release_fault_handler(FaultHandle, &status);
         DomainErrorCheck(status, "Can't release fault handler");
         return pfm_$return_to_faulting_code;
      default:
         DBG(0, "Unrecognized fault type %08x, exiting\n", FaultStatusPtr->status.all);
         exit(EXIT_FAILURE);
      }
   }


static void DomainSCSIOpen(void)
   {
   static int num_alloced = 0;
   int fd;
   scsi_$handle_t scsi_handle;
   pinteger len;
   void *DataBasePtr;

   /* Find fake fd. */
   for (fd = 0; fd < num_alloced; ++fd)
      if (!DomainFdInfo[fd].in_use)
         break;

   /* Acquire the device */
   DBG(1, "DomainSCSIOpen: dev='%s', fd=%d\n", com->open_path, fd);
   len = strlen(com->open_path);
   scsi_$acquire((char *)com->open_path, len, &scsi_handle, &com->CommandStatus);
   if (com->CommandStatus.all != status_$ok)
      {
      /* we have a failure, return an error code, and generate debug output */
      DBG(1, "DomainSCSIOpen: acquire failed, Domain/OS status is %08x\n", com->CommandStatus.all);
      error_$print(com->CommandStatus);
      return;
      }
   else
      {
      /* device acquired, setup buffers and buffer pointers */
      DBG(2, "DomainSCSIOpen: acquire OK, handle is %x\n", scsi_handle);
      /* Create/map the data area */
      tmpnam(com->open_path);
      DBG(2, "DomainSCSIOpen: Data block name will be '%s'\n", com->open_path);
      DataBasePtr = ms_$crmapl(com->open_path, strlen(com->open_path), 0, DomainMaxDataSize + DomainSenseSize, ms_$cowriters, &com->CommandStatus);
      DomainErrorCheck(com->CommandStatus, "Creating Data Area");
      assert((((int)DataBasePtr) & 0x3ff) == 0);  /* Relies on Domain/OS mapping new objects on page boundary */
      DBG(2, "Data Buffer block created at %p, length = 0x%lx\n", DataBasePtr, DomainMaxDataSize + DomainSenseSize);
      /* Wire the buffer */
      scsi_$wire(scsi_handle, (void *)DataBasePtr, DomainMaxDataSize + DomainSenseSize, &com->CommandStatus);
      if (com->CommandStatus.all == status_$ok)
         {
         /* success, indicate status */
         DBG(2, "Buffer wire was successful\n");
         }
      else
         {
         /* failure, print detail and return code */
         DBG(1, "Buffer wire failed, Domain/OS status is %08x\n", com->CommandStatus.all);
         error_$print(com->CommandStatus);
         return;
         }
      }

   if (fd >= num_alloced)
      {
      size_t new_size, old_size;

      old_size = num_alloced * sizeof (DomainFdInfo[0]);
      num_alloced = fd + 8;
      new_size = num_alloced * sizeof (DomainFdInfo[0]);
      if (DomainFdInfo)
         DomainFdInfo = realloc (DomainFdInfo, new_size);
      else
         DomainFdInfo = malloc (new_size);
      memset ((char *) DomainFdInfo + old_size, 0, new_size - old_size);
      assert(DomainFdInfo);
      }
   DomainFdInfo[fd].in_use = 1;
   DomainFdInfo[fd].scsi_handle = scsi_handle;
   DomainFdInfo[fd].DomainSCSIPtr = DataBasePtr;
   DomainFdInfo[fd].DomainSensePtr = ((char *)DataBasePtr) + DomainMaxDataSize;
   com->fd = fd;
   }


static void DomainSCSIClose(void)
   {
   DomainFdInfo[com->fd].in_use = 0;
   DBG(1, "sanei_scsi_close:  fd=%d\n", com->fd);
   /* Unwire the buffer */
   scsi_$unwire(DomainFdInfo[com->fd].scsi_handle, DomainFdInfo[com->fd].DomainSCSIPtr, DomainMaxDataSize + DomainSenseSize, true, &com->CommandStatus);
   DomainErrorCheck(com->CommandStatus, "Unwiring SCSI buffers");
   /* Release the device */
   scsi_$release(DomainFdInfo[com->fd].scsi_handle, &com->CommandStatus);
   DomainErrorCheck(com->CommandStatus, "Releasing device");
   /* Unmap the buffer area */
   ms_$unmap(DomainFdInfo[com->fd].DomainSCSIPtr, DomainMaxDataSize + DomainSenseSize, &com->CommandStatus);
   DomainErrorCheck(com->CommandStatus, "Unmapping device data area");
   }


/* I have never seen this called, and I'm not sure what to do with it, so I
guarantee that it will generate a fault, and I can add support for it.  */
static void DomainSCSIFlushAll(void)
   {
   status_$t status;

   DBG(1, "DomainSCSIFlushAll: ()\n");
   DBG(0, "Error - unimplemented feature in module" "BACKEND_NAME");
   assert(1==0);
   }


/* This function must only be called from DomainSCSIEnter.  The current
server architecture requires that the Wait immediately follow the Enter
command.  */
static void DomainSCSIWait(void)
   {
   int count;
   char *ascii_wait_status, *ascii_op_status;
   pinteger return_count;
   scsi_$op_status_t status_list[1];
   scsi_$wait_index_t wait_index;

   /* wait for the command completion */
   wait_index = scsi_$wait(DomainFdInfo[com->fd].scsi_handle, DomainScsiTimeout, true, DomainFdInfo[com->fd].op_id, 1, status_list, &return_count, &com->CommandStatus);
   DBG(2, " scsi_$wait returned status = %08x\n", com->CommandStatus.all);
   if (com->CommandStatus.all == status_$ok)
      {
      switch (wait_index)
         {
         case scsi_device_advance:  ascii_wait_status = "scsi_device_advance"; break;
         case scsi_timeout:         ascii_wait_status = "scsi_timeout"; break;
         case scsi_async_fault:     ascii_wait_status = "scsi_async_fault"; break;
         default:                   ascii_wait_status = "unknown"; break;
         }
      DBG(2, " scsi_$wait status is %s, return_count is %d\n", ascii_wait_status, return_count);
      if (wait_index != scsi_device_advance)
         {
         DBG(1, "Error - SCSI timeout, or async fault\n");
         com->CommandStatus.all = scsi_$operation_timeout;
         }
      else for (count = 0; count < return_count; count++)
         {
         switch (status_list[count].op_status)
            {
            case scsi_good_status:                ascii_op_status = "scsi_good_status"; break;
            case scsi_check_condition:            ascii_op_status = "scsi_check_condition"; break;
            case scsi_condition_met:              ascii_op_status = "scsi_condition_met"; break;
            case scsi_rsv1:                       ascii_op_status = "scsi_rsv1"; break;
            case scsi_busy:                       ascii_op_status = "scsi_busy"; break;
            case scsi_rsv2:                       ascii_op_status = "scsi_rsv2"; break;
            case scsi_rsv3:                       ascii_op_status = "scsi_rsv3"; break;
            case scsi_rsv4:                       ascii_op_status = "scsi_rsv4"; break;
            case scsi_intermediate_good:          ascii_op_status = "scsi_intermediate_good"; break;
            case scsi_rsv5:                       ascii_op_status = "scsi_rsv5"; break;
            case scsi_intermediate_condition_met: ascii_op_status = "scsi_intermediate_condition_met"; break;
            case scsi_rsv6:                       ascii_op_status = "scsi_rsv6"; break;
            case scsi_reservation_conflict:       ascii_op_status = "scsi_reservation_conflict"; break;
            case scsi_rsv7:                       ascii_op_status = "scsi_rsv7"; break;
            case scsi_rsv8:                       ascii_op_status = "scsi_rsv8"; break;
            case scsi_rsv9:                       ascii_op_status = "scsi_rsv9"; break;
            case scsi_undefined_status:           ascii_op_status = "scsi_undefined_status"; break;
            default:                              ascii_op_status = "unknown"; break;
            }
         DBG(2, " list[%d]: op=%x  cmd_status=%08x, status=%s\n", count, status_list[count].op, status_list[count].cmd_status.all, ascii_op_status);
         switch (status_list[count].cmd_status.all)
            {
            case status_$ok:
               switch (status_list[count].op_status)
                  {
                  case scsi_good_status:
                     break;
                  case scsi_busy:
                     com->CommandStatus.all = status_$ok | 0x80000000;
                     com->SCSIStatus = scsi_busy;
                     break;
                  case scsi_check_condition:
                     {
                     static unsigned char scanner_sense_cdb[] = {3, 0, 0, 0, DomainSenseSize, 0};
                     static scsi_$cdb_t sense_cdb;
                     static linteger sense_cdb_size;
                     static scsi_$operation_id_t sense_op_id;
                     static status_$t sense_status;
                     static pinteger sense_return_count;
                     static int temp;
            
                     /* Issue the sense command (wire, issue, wait, unwire */
                     sense_cdb_size = sizeof(scanner_sense_cdb);
                     memcpy(&sense_cdb, scanner_sense_cdb, sense_cdb_size);
                     scsi_$do_command_2(DomainFdInfo[com->fd].scsi_handle, sense_cdb, sense_cdb_size, DomainFdInfo[com->fd].DomainSensePtr, DomainSenseSize, scsi_read, &sense_op_id, &sense_status);
                     DomainErrorCheck(sense_status, "Executing sense command");
                     scsi_$wait(DomainFdInfo[com->fd].scsi_handle, DomainScsiTimeout, true, sense_op_id, 1, status_list, &sense_return_count, &sense_status);
                     /* The following debug output is scanner specific */
                     DBG(2, "Sense information:  Error code=%02x, ASC=%02x, ASCQ=%02x\n", ((u_char *)DomainFdInfo[com->fd].DomainSensePtr)[0], ((char *)DomainFdInfo[com->fd].DomainSensePtr)[0xc], ((char *)DomainFdInfo[com->fd].DomainSensePtr)[0xd]);
                     DBG(2, " Sense dump:\n");
                     for (temp = 0; temp < DomainSenseSize; temp++)
                        DBG(2, " %02x", ((u_char *)DomainFdInfo[com->fd].DomainSensePtr)[temp]);
                     DBG(2, "\n");
                     /* see if buffer underrun - ILI/Valid are set, and command was a read */
                     /* Warning - this might be UMAX specific */
                     if ((((char *)DomainFdInfo[com->fd].DomainSensePtr)[0] == 0xf0) && (((char *)DomainFdInfo[com->fd].DomainSensePtr)[2] & 0x20) && (com->cdb.g0.cmd == 0x28))
                        {
                        /* Warning - the following code is specific to endianness and int size */
                        /*   Its also very ugly */
                        DBG(2, "Shortening destination length by %x bytes\n", *(int *)(((char *)DomainFdInfo[com->fd].DomainSensePtr)+3));
                        com->dst_size -= *(int *)(((char *)DomainFdInfo[com->fd].DomainSensePtr)+3);
                        DBG(2, "Final dest size is %x\n", com->dst_size);
                        }
                     else
                        {
                        /* Set this status so that the sense handler can be called */
                        com->CommandStatus.all = status_$ok | 0x80000000;
                        com->SCSIStatus = scsi_check_condition;
                        }
                     }
                     break;
                  default:
                     /* I fault out in this case because I want to know about this error,
                        and this guarantees that it will get attention. */
                     DBG(0, "Unrecoverable Domain/OS scsi handler error:  status=%08x\n", status_list[count].op_status);
                     exit(EXIT_FAILURE);
                  }
               break;
            /* Handle recognized error conditions by copying the error code over */
            case scsi_$operation_timeout:
            case scsi_$dma_underrun:  /* received by some backend code */
            case scsi_$hdwr_failure:  /* received when both scanners were active */
               com->CommandStatus = status_list[count].cmd_status;
               break;
            default:
               DBG(0, "Unrecoverable DomainOS scsi handler error:  status=%08x\n", status_list[count].cmd_status.all);
               error_$print(status_list[count].cmd_status);
               exit(EXIT_FAILURE);
            }
         }
      /* Dump the buffer contents */
      if (com->direction == scsi_read)
         {
         DBG(3, "first words of buffer are:\n");
         for (return_count = 0; return_count < com->dst_size; return_count++)
            DBG(3, "%02X%c", ((unsigned char *)DomainFdInfo[com->fd].DomainSCSIPtr)[return_count], (return_count % 16) == 15 ? '\n' : ' ');
         DBG(3, "\n");
         }
      }
   else
      {
      /* scsi_$wait failed */
      DBG(1, "scsi_$wait failed, status is %08x\n", com->CommandStatus.all);
      }
   }


static void DomainSCSIEnter(void)
   {
   static int count;

   /* Give some debug info */
   DBG(1, "Entering DomainSCSIEnter, fd=%d, opcode=%02X\n", com->fd, com->cdb.all[0]);
   DBG(2, " CDB Contents: ");
   for (count = 0; count < com->cdb_size; count++)
      DBG(2, " %02X", com->cdb.all[count]);
   DBG(2, "\n");
   DBG(2, "Buffer address is 0x%08x\n", DomainFdInfo[com->fd].DomainSCSIPtr);
   DBG(2, "Buffer size is %x\n", com->buf_size);
   DBG(2, "Direction is %s\n", (com->direction == scsi_read) ? "READ" : "WRITE");
   /* now queue the command */
   scsi_$do_command_2(DomainFdInfo[com->fd].scsi_handle, com->cdb, com->cdb_size, DomainFdInfo[com->fd].DomainSCSIPtr, com->buf_size, com->direction, &DomainFdInfo[com->fd].op_id, &com->CommandStatus);
   if (com->CommandStatus.all == status_$ok)
      {
      /* success, indicate status */
      DBG(2, " scsi_$do_command_2 was successful, op_id is %x\n", DomainFdInfo[com->fd].op_id);

      /* If we supported multiple outstanding requests for one device, this would be
         a good breakpoint.  We would store the op_id in a private place, and construct
         a queue for each device.  This complicates things, and SANE doesn't seem to need
         it, so it won't be implemented.  The current server architecture does the wait
         automatically, and status for the entire operation is returned.  This means that
         the sanei_scsi_req_enter and sanei_scsi_req_wait calls don't make sense, and
         should generate an error. */
      DomainSCSIWait();
      }
   else
      {
      /* failure, print detail and return code */
      DBG(1, " scsi_$do_command_2 failed, status is %08x\n", com->CommandStatus.all);
      }
   }


/* This function is not currently used. */
static void DomainSCSIReqWait(void)
   {
   DBG(1, "sanei_scsi_req_wait: (id=%p)\n", NULL);
   return;
   }


/* Startup the server */
static void sanei_DomainOS_init(char *path)
   {
   int done, index;
   long CommandTriggerValue;
   ec2_$ptr_t CommandAvailablePtr[1];
   status_$t status;
   unsigned long length_mapped;

   DBG(1, "Starting Domain SANE Server, common area path = '%s'\n", path);
   com = ms_$mapl(path, strlen(path), 0, sizeof(struct DomainServerCommon), ms_$cowriters, ms_$wr, true, &length_mapped, &status);
   DomainErrorCheck(status, "Can't open common area");
   if (length_mapped < sizeof(struct DomainServerCommon))
      {
      DBG(0, "Error - can't open common area '%s' to required length\n", path);
      DBG(0, " Required length = %lx, returned length = %lx\n", sizeof(struct DomainServerCommon), length_mapped);
      exit(EXIT_FAILURE);
      }
   /* Make the file temporary, so it will disappear when it is closed */
   ms_$mk_temporary(com, &status);
   DomainErrorCheck(status, "Can't make common file temporary");
   DBG(2, "Domain Server common area mapped, length is %lx\n", length_mapped);
   /* The communication area is open, give the initial response */
   ec2_$advance(&com->ResultReady, &status);
   DomainErrorCheck(status, "Can't advance ResultReady EC after startup");
   /* Enter the command loop */
   CommandAvailablePtr[0] = &com->CommandAvailable;
   CommandTriggerValue = ec2_$read(com->CommandAvailable) + 1;
   /* Inhibit asynchronous faults */
/*   pfm_$inhibit();*/
   /* Establish the fault handler */
   FaultHandle = pfm_$establish_fault_handler(pfm_$all_faults, 0, FaultHandler, &status);
   DomainErrorCheck(status, "Can't establish fault handler");
   done = 0;
   do
      {
      /* Wait for the command */
      DBG(2, "Waiting for incoming command\n");
      do
         {
         index = ec2_$wait_svc(CommandAvailablePtr, &CommandTriggerValue, 1, &status);
         }
      while (status.all == ec2_$wait_quit);
      DomainErrorCheck(status, "Error waiting on CommandAvailable EC");
      assert (index == 1);
      /* Get the trigger value for next time - this avoids a race/deadlock */
      CommandTriggerValue = ec2_$read(com->CommandAvailable) + 1;
      /* decode/execute the command */
      DBG(2, "Received a command - opcode is %x\n", com->opcode);
      switch(com->opcode)
         {
         case Open:
            DomainSCSIOpen();
            ec2_$advance(&com->CommandAccepted, &status);
            DomainErrorCheck(status, "Can't advance CommandAccepted EC on open");
            break;
         case Close:
            DomainSCSIClose();
            ec2_$advance(&com->CommandAccepted, &status);
            DomainErrorCheck(status, "Can't advance CommandAccepted EC on close");
            break;
         case Enter:
            DomainSCSIEnter();
            ec2_$advance(&com->CommandAccepted, &status);
            DomainErrorCheck(status, "Can't advance CommandAccepted EC on enter");
            break;
         case Exit:
            done = 1;
            /* This lets the parent know that the command was accepted.  It can be
               used to avoid sending a signal.  */
            ec2_$advance(&com->CommandAccepted, &status);
            DomainErrorCheck(status, "Can't advance CommandAccepted EC on exit");
            break;
         default:
            DBG(1, "Invalid command %x received\n", com->opcode);
         }
      DBG(2, "Command processing complete\n");
      }
   while (!done);
   /* This would be a good place to close all devices, but for now we'll assume
      they have already been closed by a well-behaved program */
   /* Unmap the common area */
   ms_$unmap(com, sizeof(struct DomainServerCommon), &status);
   DomainErrorCheck(status, "Error unmapping common area");
   DBG(1, "Exiting Domain SANE Server\n");
/*   pfm_$enable();*/
   exit(EXIT_SUCCESS);
   }
