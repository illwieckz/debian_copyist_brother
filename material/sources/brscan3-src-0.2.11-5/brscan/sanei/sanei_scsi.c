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

   This file provides a generic SCSI interface.  */

#ifdef _AIX
# include "../include/lalloca.h"		/* MUST come first for AIX! */
#endif

#include "../include/sane/config.h"
#include "../include/lalloca.h"
#include "../include/lassert.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/types.h>

#define STUBBED_INTERFACE	0
#define LINUX_INTERFACE		1
#define BSD_INTERFACE		2
#define	HPUX_INTERFACE		3
#define OPENSTEP_INTERFACE	4
#define DECUNIX_INTERFACE	5
#define SCO_OS5_INTERFACE	6
#define IRIX_INTERFACE		7
#define SOLARIS_INTERFACE	8
#define SOLARIS_SG_INTERFACE	9
#define OS2_INTERFACE		10
#define AIX_GSC_INTERFACE	11
#define DOMAINOS_INTERFACE	12
#define FREEBSD_CAM_INTERFACE	13
#define SYSVR4_INTERFACE	14
#define SCO_UW71_INTERFACE	15
#define SOLARIS_USCSI_INTERFACE	16

/* time out for SCSI commands in seconds */
#define SANE_SCSICMD_TIMEOUT 60

#if defined (HAVE_SCSI_SG_H)
# define USE LINUX_INTERFACE
# include <scsi/sg.h>
#elif defined (HAVE__USR_SRC_LINUX_INCLUDE_SCSI_SG_H)
# define USE LINUX_INTERFACE
# include "/usr/src/linux/include/scsi/sg.h"
#elif defined (HAVE_SYS_SCSICMD)
# define USE SCSO_OS5_INTERFACE
# include <sys/scsi.h>
# include <sys/scsicmd.h>
#elif defined (HAVE_CAMLIB_H)
# define USE FREEBSD_CAM_INTERFACE
# include <stdio.h>  /* there is a bug in scsi_all.h */
# include <cam/cam.h>
# include <cam/cam_ccb.h>
# include <cam/scsi/scsi_message.h>
# include <cam/scsi/scsi_pass.h>
# include <camlib.h>
#elif defined (HAVE_SYS_SCSIIO_H)
# define USE BSD_INTERFACE
# include <sys/scsiio.h>
# ifdef HAVE_SCSI_H
#  include <scsi.h>
# endif
#elif defined (HAVE_BSD_DEV_SCSIREG_H)
# define USE OPENSTEP_INTERFACE
# include <bsd/dev/scsireg.h>
#elif defined (HAVE_IO_CAM_CAM_H)
# define USE DECUNIX_INTERFACE
# include <io/common/iotypes.h>
# include <io/cam/cam.h>
# include <io/cam/dec_cam.h>
# include <io/cam/uagt.h>
# include <io/cam/scsi_all.h>
#elif defined (HAVE_SYS_DSREQ_H)
# define USE IRIX_INTERFACE
# include <sys/dsreq.h>
# include <invent.h>
#elif defined (HAVE_SYS_SCSI_H)
# include <sys/scsi.h>
# ifdef HAVE_SYS_SDI_COMM_H 
#  ifdef HAVE_SYS_PASSTHRUDEF_H 
#   define USE SCO_UW71_INTERFACE 
#   include <sys/scsi.h> 
#   include <sys/sdi_edt.h>
#   include <sys/sdi.h> 
#   include <sys/passthrudef.h> 
#  else 
#   define USE SYSVR4_INTERFACE /* Unixware 2.x tested */ 
#   define HAVE_SYSV_DRIVER 
#   include <sys/sdi_edt.h> 
#   include <sys/sdi_comm.h> 
#  endif 
# else 
#  ifdef SCTL_READ
#   define USE HPUX_INTERFACE
#  else
#   ifdef HAVE_GSCDDS_H
#    define USE AIX_GSC_INTERFACE
#    include <gscdds.h>
#   else
    /* This happens for AIX without gsc and possibly other platforms... */
#   endif
#  endif
# endif
#elif defined (HAVE_OS2_H)
# define USE OS2_INTERFACE
# define INCL_DOSFILEMGR
# define INCL_DOS
# define INCL_DOSDEVICES
# define INCL_DOSDEVIOCTL
# define INCL_DOSMEMMGR
# include <os2.h>
# include "os2_srb.h"
#elif defined (HAVE_SYS_SCSI_SGDEFS_H)
# define USE SOLARIS_SG_INTERFACE
# include <sys/scsi/sgdefs.h>
#elif defined (HAVE_SYS_SCSI_TARGETS_SCGIO_H)
# define USE SOLARIS_INTERFACE
# define SOL2
# include <sys/scsi/targets/scgio.h>
#elif defined (HAVE_SYS_SCSI_SCSI_H)
  /*
   * the "offical" solaris uscsi(7I) interface; comes last, so that users
   * installing the SCG/SG driver can still use these generic scsi interfaces
   */
# define USE SOLARIS_USCSI_INTERFACE
# define SOL2
# include <sys/scsi/scsi.h>
#elif defined (HAVE_APOLLO_SCSI_H)
# define USE DOMAINOS_INTERFACE
# include <signal.h>	/* Only used for signal name for KillDomainServer */
# include <apollo/base.h>
# include <apollo/ec2.h>
# include <apollo/error.h>
# include <apollo/ms.h>
# include <apollo/mutex.h>
# include <apollo/scsi.h>
# include <apollo/time.h>
# include "sanei_DomainOS.h"
#endif

#ifndef USE
# define USE STUBBED_INTERFACE
#endif

#include "../include/sane/sanei.h"
#include "../include/sane/sanei_config.h"
#include "../include/sane/sanei_scsi.h"

#define BACKEND_NAME	sanei_scsi
#include "../include/sane/sanei_debug.h"

#if USE == DECUNIX_INTERFACE
static int cam_fd = -1;		/* used for SCSI CAM based interfaces */
#endif

#if USE == SOLARIS_INTERFACE || USE == SOLARIS_USCSI_INTERFACE
static int unit_ready (int fd);
#endif

#ifdef SG_BIG_BUFF
# define MAX_DATA	SG_BIG_BUFF
#endif

#if USE == SYSVR4_INTERFACE 
# define MAX_DATA 56*1024 /* don't increase or kernel will dump 
			   * tested with adsl, adsa and umax backend 
			   * it depends on the lowend scsi 
			   * drivers . But the most restriction 
			   * is in the UNIXWARE KERNEL witch do 
			   * not allow more then 64kB DMA transfers */ 
static char lastrcmd[16]; /* hold command block of last read command */ 
#endif

#if USE == OPENSTEP_INTERFACE
# define MAX_DATA	(120*1024)
#endif

#if USE == IRIX_INTERFACE
# define MAX_DATA	(256*1024)
#endif

#if USE == FREEBSD_CAM_INTERFACE
# define MAX_DATA	(DFLTPHYS - PAGE_SIZE)
#endif

#if USE == SOLARIS_INTERFACE
# define MAX_DATA	(128*1024)
#endif

#if USE == SOLARIS_SG_INTERFACE
# define MAX_DATA	(128*1024)
#endif

#if USE == SOLARIS_USCSI_INTERFACE
# define MAX_DATA	(128*1024)
#endif

#if USE == OS2_INTERFACE
# define MAX_DATA	(64*1024)
#endif


#ifndef MAX_DATA
# define MAX_DATA	(32*1024)
#endif

int sanei_scsi_max_request_size = MAX_DATA;
#if USE == LINUX_INTERFACE
/* the following #defines follow Douglas Gilbert's sample code
   to maintain run time compatibility with the old and the
   new SG driver for Linux
*/
#include "linux_sg3_err.h"  /* contains several definitions of error codes */
#ifndef SG_SET_COMMAND_Q
#define SG_SET_COMMAND_Q 0x2271
#endif
#ifndef SG_SET_RESERVED_SIZE
#define SG_SET_RESERVED_SIZE 0x2275
#endif
#ifndef SG_GET_RESERVED_SIZE
#define SG_GET_RESERVED_SIZE 0x2272
#endif
#ifndef SG_GET_SCSI_ID
#define SG_GET_SCSI_ID 0x2276
#else
#define SG_GET_SCSI_ID_FOUND
#endif
#ifndef SG_GET_VERSION_NUM
#define SG_GET_VERSION_NUM 0x2282
#endif
#ifndef SG_NEXT_CMD_LEN
#define SG_NEXT_CMD_LEN 0x2283
#endif

#ifndef SCSIBUFFERSIZE
#define SCSIBUFFERSIZE (128 * 1024)
#endif

/* the struct returned by the SG ioctl call SG_GET_SCSI_ID changed
   from version 2.1.34 to 2.1.35, and we need the informations from
   the field s_queue_depth, which was introduced in 2.1.35.
   To get this file compiling also with older versions of sg.h, the
   struct is re-defined here.
*/
typedef struct xsg_scsi_id {
    int host_no;        /* as in "scsi<n>" where 'n' is one of 0, 1, 2 etc */
    int channel;
    int scsi_id;        /* scsi id of target device */
    int lun;
    int scsi_type;      /* TYPE_... defined in scsi/scsi.h */
    short h_cmd_per_lun;/* host (adapter) maximum commands per lun */
    short d_queue_depth;/* device (or adapter) maximum queue length */
    int unused1;        /* probably find a good use, set 0 for now */
    int unused2;        /* ditto */
} SG_scsi_id;

typedef struct req
  {
    struct req *next;
    int fd;
    u_int running:1, done:1;
    SANE_Status status;
    size_t *dst_len;
    void *dst;
/* take the definition of the ioctl parameter SG_IO as a
   compiler flag if the new SG driver is available
*/
    union
      {
        struct
          {
            struct sg_header hdr;
            /* Make sure this is the last element, the real size is
              SG_BIG_BUFF and machine dependant */
            u_int8_t data[1];
          }
        cdb;
#ifdef SG_IO
/* at present, Linux's SCSI system limits the sense buffer to 16 bytes
   which is definitely too small. Hoping that this will change at some time,
   let's set the sense buffer size to 64.
*/
#define SENSE_MAX 64
#define MAX_CDB 12
        struct
          {
            struct sg_io_hdr hdr;
            u_char sense_buffer[SENSE_MAX];
            u_int8_t data[1];
          } sg3;
#endif
      }
    sgdata;
  }
req;

typedef struct Fdparms
  {
    int sg_queue_used, sg_queue_max;
    size_t buffersize;
    req *sane_qhead, *sane_qtail, *sane_free_list;
  }
fdparms;

#endif

#if USE == FREEBSD_CAM_INTERFACE
# define CAM_MAXDEVS	128
struct cam_device *cam_devices[CAM_MAXDEVS] = { NULL };
#endif

static struct
  {
    u_int in_use:1;		/* is this fd_info in use? */
    u_int fake_fd:1;		/* is this a fake file descriptor? */
    u_int bus, target, lun;	/* nexus info; used for some interfaces only */
    SANEI_SCSI_Sense_Handler sense_handler;
    void *sense_handler_arg;
    void *pdata;		/* platform-specific data */
  }
 *fd_info;

static u_char cdb_sizes[8] =
  {
    6, 10, 10, 12, 12, 12, 10, 10
  };
#define CDB_SIZE(opcode)	cdb_sizes[(((opcode) >> 5) & 7)]


#if USE == DOMAINOS_INTERFACE

/*
   This includes the server code.  Most of these routines are private to the
   actual server.  The only public ones are:
   sanei_DomainOS_init     Used to initialize the server
   DomainErrorCheck        A common error handling routine
 */

#include "sanei_DomainOS.c"

int ServerInitialized = 0;
pid_t ServerPID;
struct DomainServerCommon *com;
long CommandTriggerValue[2];
ec2_$ptr_t CommandAcceptedPtr[2];
long ResultTriggerValue[2];
ec2_$ptr_t ResultReadyPtr[2];
time_$clock_t Wait16S = { 64, 0 };		/* Delay of about 16 Seconds */


/* This function is registered as an exit function.  It's purpose is
   to make sure that the Domain SANE Server is stopped.  It tries to
   send an Exit command, and if that fails, it will send SIGQUIT to
   the server.  It will also unmap the common area before it
   returns. */
static void
KillDomainServer (void)
{
  static boolean GotTheLock;
  static status_$t status;
  static pinteger index;

  DBG (1, "Asking Domain SANE Server to exit\n");
  /* First, try to send a command to exit */
  if (GotTheLock = mutex_$lock (&com->CommandLock, Wait16S))
    {
      /* Set the wait time to 16 Seconds (units are 4uS) */
      com->opcode = Exit;
      CommandTriggerValue[0] = ec2_$read (com->CommandAccepted) + 1;
      ec2_$advance (&com->CommandAvailable, &status);
      DomainErrorCheck (status, "Can't advance CommandAvailable EC");
      /* For this wait, we want to allow a timeout as well */
      CommandTriggerValue[1] = (ec2_$read (*CommandAcceptedPtr[1])
				+ DomainECWaitConstant);
      index = ec2_$wait_svc (CommandAcceptedPtr, CommandTriggerValue, 2,
			     &status);
      DomainErrorCheck (status, "Error waiting on Exit command acceptance EC");
      /* Release the lock */
      mutex_$unlock (&com->CommandLock);
      if (index == 1)
	DBG (1, "Domain SANE Server responded to exit request\n");
      else
	DBG (1, "Domain SANE Server did not respond to exit request\n");
    }
  else
    DBG (0, "Could not get mutex lock for killing server\n");
  if ((!GotTheLock) || (index != 1))
    {
      /* If we get here, then we never got the mutex lock, or we timed out
         waiting for an Exit command ack. */
      /* It's now time to be brutal with the server */
      DBG (1, "Sending QUIT signal to Domain SANE Server\n");
      kill (ServerPID, SIGQUIT);
    }
  /* unmap the common area */
  ms_$unmap (com, sizeof (struct DomainServerCommon), &status);
  DomainErrorCheck (status, "Error unmapping common area");
}
#endif /* USE == DOMAINOS_INTERFACE */


#if USE == OS2_INTERFACE

/* Driver info:  */
static HFILE driver_handle = 0;	/* file handle for device driver */
static PVOID aspi_buf = 0;	/* Big data buffer locked by driver. */
static int aspi_ref_count = 0;	/* # of fds using ASPI */
static char tmpAspi[MAXPATHLEN];	/* scsi chain scan */
#define INQUIRY					0x12
#define set_inquiry_return_size(icb,val)	icb[0x04]=val
#define IN_periph_devtype_cpu			0x03
#define IN_periph_devtype_scanner		0x06
#define get_inquiry_vendor(in, buf)		strncpy(buf, in + 0x08, 0x08)
#define get_inquiry_product(in, buf)		strncpy(buf, in + 0x10, 0x010)
#define get_inquiry_version(in, buf)		strncpy(buf, in + 0x20, 0x04)
#define get_inquiry_periph_devtype(in)		(in[0] & 0x1f)
#define get_inquiry_additional_length(in)	in[0x04]
#define set_inquiry_length(out,n)		out[0x04]=n-5

/* Open OS2 ASPI driver.

   Output: 0 if error, which is reported.  */
static int
open_aspi (void)
{
  ULONG rc;
  ULONG ActionTaken;
  USHORT lockSegmentReturn;
  unsigned long cbreturn;
  unsigned long cbParam;
  SRB SRBlock;			/* SCSI Request Block */

  int i, num_adapters;		/* no. of scsi adapters installed */

  char *devtypes[] =
  {
    "disk", "tape", "printer", "processor", "CD-writer",
    "CD-drive", "scanner", "optical-drive", "jukebox",
    "communicator"
  };
  FILE *tmp;

  if (driver_handle)
    {
      aspi_ref_count++;		/* increment internal usage counter */
      return 1;			/* Already open. */
    }
   aspi_buf = _tcalloc(sanei_scsi_max_request_size, 1);
   if (aspi_buf == NULL)
     { DBG(1, "sanei_scsi_open_aspi: _tcalloc failed");
       return 0;
     }
  rc = DosOpen ((PSZ) "aspirou$",	/* open driver */
		&driver_handle,
		&ActionTaken,
		0,
		0,
		FILE_OPEN,
		OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READWRITE,
		NULL);
  if (rc)
    {
      /* opening failed -> return false */
      DBG (1, "open_aspi:  opening failed.\n");
      return 0;
    }

  /* Lock aspi_buf. */
  rc = DosDevIOCtl (driver_handle, 0x92, 0x04,	/* pass aspi_buf pointer */
		    (void *) aspi_buf, sizeof (PVOID),	/* to driver */
		    &cbParam, (void *) &lockSegmentReturn,
		    sizeof (USHORT), &cbreturn);
  if (rc || lockSegmentReturn)
    {
      /* DosDevIOCtl failed */
      DBG (1, "sanei_scsi_open_aspi:  Can't lock buffer. rc= %d \n",rc);
      return 0;
    }

  /* query number of installed adapters */
  memset (&SRBlock, 0, sizeof (SRBlock));
  SRBlock.cmd = SRB_Inquiry;	/* host adapter inquiry */

  SRBlock.ha_num = 0;		/* host adapter number */

  SRBlock.flags = 0;		/* no flags set */

  rc = DosDevIOCtl (driver_handle, 0x92, 0x02,
		    (void *) &SRBlock, sizeof (SRB), &cbParam,
		    (void *) &SRBlock, sizeof (SRB), &cbreturn);
  num_adapters = SRBlock.u.inq.num_ha;

  DBG (1, "OS/2: installed adapters %d\n", num_adapters);
  DBG (1, "OS/2: ASPI manager is '%s'\n", SRBlock.u.inq.aspimgr_id);
  DBG (1, "OS/2: host adapter is '%s'\n", SRBlock.u.inq.host_id);
  DBG (1, "OS/2: unique id is    '%s'\n", SRBlock.u.inq.unique_id);

  strcpy (tmpAspi, "asXXXXXX");
  mktemp (tmpAspi);
  DBG (2, "open_aspi: open temporary file '%s'\n", tmpAspi);
  tmp = fopen (tmpAspi, "w");
  if (!tmp)
    {				/* can't open tmp file */

      DBG (1, "open_aspi:  Can't open temporary file.\n");
      return 0;
    }

  /* scan all installed adapters */
  for (i = 0; i < num_adapters; i++)
    {
      int id;
      /* query adapter name */
      memset (&SRBlock, 0, sizeof (SRBlock));
      SRBlock.cmd = SRB_Inquiry;	/* host adapter inquiry */

      SRBlock.ha_num = i;	/* host adapter number */

      SRBlock.flags = 0;	/* no flags set */

      rc = DosDevIOCtl (driver_handle, 0x92, 0x02,
			(void *) &SRBlock, sizeof (SRB), &cbParam,
			(void *) &SRBlock, sizeof (SRB), &cbreturn);
      DBG (1, "OS/2: adapter#%02d '%s'\n", i, SRBlock.u.inq.host_id);

      /* scan scsi chain (need 15 for wide?) */
      for (id = 0; id < 7; id++)
	{
	  unsigned char len;
	  unsigned char vendor[9];
	  unsigned char product[17];
	  unsigned char version[5];
	  unsigned char *pp;

	  memset (&SRBlock, 0, sizeof (SRBlock));
	  SRBlock.cmd = SRB_Device;	/* get device type */

	  SRBlock.ha_num = i;	/* host adapter number */

	  SRBlock.flags = 0;	/* no flags set */

	  SRBlock.u.dev.target = id;	/* target id */

	  SRBlock.u.dev.lun = 0;	/* target LUN */

	  rc = DosDevIOCtl (driver_handle, 0x92, 0x02,
			    (void *) &SRBlock, sizeof (SRB), &cbParam,
			    (void *) &SRBlock, sizeof (SRB), &cbreturn);
	  DBG (1, "OS/2:             id#%02d status=%02xh\n",
	       id, SRBlock.status);

	  /* skip if device not connected */
	  if (SRBlock.status == SRB_BadDevice)
	    continue;

	  DBG (1, "OS/2:                   type is '%s'\n",
	       SRBlock.u.dev.devtype < sizeof (devtypes) / sizeof (char *)?
	       devtypes[SRBlock.u.dev.devtype] : "unknown device");

	  /* query adapter string id */
	  memset (&SRBlock, 0, sizeof (SRBlock));
	  SRBlock.cmd = SRB_Command;	/* execute SCSI command */

	  SRBlock.ha_num = i;	/* host adapter number */
	  SRBlock.flags = SRB_Read | SRB_Post;	/* data transfer, posting */
	  SRBlock.u.cmd.target = id;	/* Target SCSI ID */
	  SRBlock.u.cmd.lun = 0;	/* Target SCSI LUN */
	  SRBlock.u.cmd.data_len = 5;	/* # of bytes transferred */
	  SRBlock.u.cmd.sense_len = 16;		/* length of sense buffer */
	  SRBlock.u.cmd.data_ptr = NULL;	/* pointer to data buffer */
	  SRBlock.u.cmd.link_ptr = NULL;	/* pointer to next SRB */
	  SRBlock.u.cmd.cdb_len = 6;	/* SCSI command length */
	  SRBlock.u.cmd.cdb_st[0] = INQUIRY;	/* inquiry command */
	  SRBlock.u.cmd.cdb_st[1] = 0;	/* ?? length */
	  SRBlock.u.cmd.cdb_st[2] = 0;	/* transfer length MSB */
	  SRBlock.u.cmd.cdb_st[3] = 0;	/* transfer length */
	  SRBlock.u.cmd.cdb_st[4] = 5;	/* transfer length LSB */
	  SRBlock.u.cmd.cdb_st[5] = 0;
	  rc = DosDevIOCtl (driver_handle, 0x92, 0x02,
			    (void *) &SRBlock, sizeof (SRB), &cbParam,
			    (void *) &SRBlock, sizeof (SRB), &cbreturn);
	  len = ((char *) aspi_buf)[4];		/* additional length */

	  /* query id string */
	  memset (&SRBlock, 0, sizeof (SRBlock));
	  SRBlock.cmd = SRB_Command;	/* execute SCSI command */
	  SRBlock.ha_num = i;	/* host adapter number */
	  SRBlock.flags = SRB_Read | SRB_Post;	/* data transfer, posting */
	  SRBlock.u.cmd.target = id;	/* Target SCSI ID */
	  SRBlock.u.cmd.lun = 0;	/* Target SCSI LUN */
	  SRBlock.u.cmd.data_len = 5 + len;	/* # of bytes transferred */
	  SRBlock.u.cmd.sense_len = 16;		/* length of sense buffer */
	  SRBlock.u.cmd.data_ptr = NULL;	/* pointer to data buffer */
	  SRBlock.u.cmd.link_ptr = NULL;	/* pointer to next SRB */
	  SRBlock.u.cmd.cdb_len = 6;	/* SCSI command length */
	  SRBlock.u.cmd.cdb_st[0] = 0x12;	/* inquiry command */
	  SRBlock.u.cmd.cdb_st[1] = 0;	/* ?? length */
	  SRBlock.u.cmd.cdb_st[2] = 0;	/* transfer length MSB */
	  SRBlock.u.cmd.cdb_st[3] = 0;	/* transfer length */
	  SRBlock.u.cmd.cdb_st[4] = 5 + len;	/* transfer length LSB */
	  SRBlock.u.cmd.cdb_st[5] = 0;
	  rc = DosDevIOCtl (driver_handle, 0x92, 0x02,
			    (void *) &SRBlock, sizeof (SRB), &cbParam,
			    (void *) &SRBlock, sizeof (SRB), &cbreturn);
	  DBG (1, "OS/2         '%s'\n", (char *) aspi_buf + 8);
	  /* write data */
	  get_inquiry_vendor ((char *) aspi_buf, vendor);
	  get_inquiry_product ((char *) aspi_buf, product);
	  get_inquiry_version ((char *) aspi_buf, version);

	  pp = &vendor[7];
	  vendor[8] = '\0';
	  while (pp >= vendor && (*pp == ' ' || *pp >= 127))
	    *pp-- = '\0';

	  pp = &product[15];
	  product[16] = '\0';
	  while (pp >= product && (*pp == ' ' || *pp >= 127))
	    *pp-- = '\0';

	  pp = product;
	  do
	    {
	      if (isspace (*pp))
		*pp = '_';
	    }
	  while (*++pp);

	  pp = &version[3];
	  version[4] = '\0';
	  while (pp >= version && (*pp == ' ' || *(pp - 1) >= 127))
	    *pp-- = '\0';
	  fprintf (tmp, "Vendor: %s ", vendor);
	  fprintf (tmp, "Model: %s ", product);
	  fprintf (tmp, "Rev: %s ", version);
	  fprintf (tmp, "scsi %d Channel: 0 Id: %d Lun: 0\n", i, id);
	}
    }
  DBG (2, "open_aspi: close temporary file '%s'\n", tmpAspi);
  fclose (tmp);

  aspi_ref_count++;		/* increment internal usage counter */

  return 1;
}

/* Close driver and free everything.  */

static void
close_aspi (void)
{
  aspi_ref_count--;			/* decrement internal usage counter */

  if (aspi_ref_count)
    return;				/* wait for usage==0 */

  if (driver_handle)		/* Close driver. */
    DosClose (driver_handle);
  driver_handle = 0;
  if (aspi_buf)			/* Free buffer. */
    _tfree (aspi_buf);
  aspi_buf = 0;

  errno = 0;
  if (unlink (tmpAspi))			/* remove scsi descriptions */
    DBG( 2, "OS/2: error#%d while removing temporary '%s'\n", errno, tmpAspi);
  strcpy (tmpAspi, "");

  DBG (1, "OS/2: ASPI closed\n");
}

#endif /* USE_OS2_INTERFACE */

static int num_alloced = 0;

#if USE == LINUX_INTERFACE

static int sg_version = 0;

static SANE_Status
get_max_buffer_size(const char *file) 
{
  int fd;
  int buffersize = SCSIBUFFERSIZE, i;
  size_t len;
  char *cc, *cc1, buf[32];

  
  fd = open(file, O_RDWR);

  if (fd > 0)
    {
      cc = getenv("SANE_SG_BUFFERSIZE");
      if (cc)
        {
          i = strtol(cc, &cc1, 10);
          if (cc != cc1 && i >= 32768)
            buffersize = i;
        }
      
      ioctl(fd, SG_SET_RESERVED_SIZE, &buffersize);
      if (0 == ioctl(fd, SG_GET_RESERVED_SIZE, &buffersize)) 
        {
          if (buffersize < sanei_scsi_max_request_size)
            sanei_scsi_max_request_size = buffersize;
          close(fd);
          DBG(4, "get_max_buffer_size for %s: %i\n", file, sanei_scsi_max_request_size);
          return SANE_STATUS_GOOD;
        }
      else
        {
          close(fd);
          /* ioctl not available: we have the old SG driver */
          fd = open ("/proc/sys/kernel/sg-big-buff", O_RDONLY);
          if (fd > 0 && (len = read (fd, buf, sizeof (buf) - 1)) > 0)
            {
              buf[len] = '\0';
              sanei_scsi_max_request_size = atoi (buf);
              close(fd);
            }
          else
            sanei_scsi_max_request_size = buffersize < SG_BIG_BUFF ?
                                          buffersize : SG_BIG_BUFF;
          return SANE_STATUS_IO_ERROR;
        }
    }
  else 
    return SANE_STATUS_GOOD;
}


SANE_Status
sanei_scsi_open_extended (const char *dev, int *fdp,
                         SANEI_SCSI_Sense_Handler handler,
                         void *handler_arg, int *buffersize)

#else

SANE_Status
sanei_scsi_open (const char *dev, int *fdp,
		 SANEI_SCSI_Sense_Handler handler, void *handler_arg)

#endif

{
  u_int bus = 0, target = 0, lun = 0, fake_fd = 0;
  char *real_dev = 0;
  void *pdata = 0;
  int fd;
#if USE == LINUX_INTERFACE
  static int first_time = 1;
#endif

  DBG_INIT ();

#if USE == LINUX_INTERFACE
  if (first_time)
    {
      char *cc, *cc1;
      int i;

      first_time = 0;
      
      /* Try to determine a reliable value for sanei_scsi_max_request_size:
      
         With newer versions of the SG driver, check the available buffer
         size by opening all SG device files belonging to a scanner,
         issue the ioctl calls for setting and reading the reserved
         buffer size, and take the smallest value. 
         
         For older version of the SG driver, which don't support variable
         buffer size, try to read /proc/sys/kernel/sg-big-biff ; if
         this fails (SG driver too old, or loaded as a module), use
         SG_BIG_BUFF
      */
      
      sanei_scsi_max_request_size = SCSIBUFFERSIZE;
      cc = getenv("SANE_SG_BUFFERSIZE");
      if (cc)
        {
          i = strtol(cc, &cc1, 10);
          if (cc != cc1 && i >= 32768)
            sanei_scsi_max_request_size = i;
        }
      sanei_scsi_find_devices(0, 0, "Scanner", -1, -1, -1, -1, 
                              get_max_buffer_size);
      sanei_scsi_find_devices(0, 0, "Processor", -1, -1, -1, -1, 
                              get_max_buffer_size);
      DBG (4, "sanei_scsi_open: sanei_scsi_max_request_size=%d bytes\n",
           sanei_scsi_max_request_size);
    }
#endif

#if USE == OS2_INTERFACE
  if (sscanf (dev, "b%dt%dl%d", &bus, &target, &lun) != 3)
    {
      DBG (1, "sanei_scsi_open: device name %s is not valid\n", dev);
      return SANE_STATUS_INVAL;
    }
  if (!open_aspi ())
    {
      /* Open driver if necessary. */
      close_aspi ();
      return SANE_STATUS_INVAL;
    }

  /* Find fake fd. */
  for (fd = 0; fd < num_alloced; ++fd)
    if (!fd_info[fd].in_use)
      break;
  fake_fd = 1;
#elif USE == DECUNIX_INTERFACE
  {
    UAGT_CAM_SCAN cam_scan;

    if (sscanf (dev, "b%dt%dl%d", &bus, &target, &lun) != 3)
      {
	DBG (1, "sanei_scsi_open: device name `%s´ is not valid: %s\n",
	     dev, strerror (errno));
	return SANE_STATUS_INVAL;
      }

    if (cam_fd < 0)
      {
	cam_fd = open ("/dev/cam", O_RDWR);
	if (cam_fd < 0)
	  {
	    DBG (1, "sanei_scsi_open: open(/dev/cam) failed: %s\n",
		 strerror (errno));
	    return SANE_STATUS_INVAL;
	  }
      }
    cam_scan.ucs_bus = bus;
    cam_scan.ucs_target = target;
    cam_scan.ucs_lun = lun;
    if (ioctl (cam_fd, UAGT_CAM_SINGLE_SCAN, &cam_scan) < 0)
      {
	DBG (1, "sanei_scsi_open: ioctl(UAGT_CAM_SINGLE_SCAN) failed: %s\n",
	     strerror (errno));
	return SANE_STATUS_INVAL;
      }

    for (fd = 0; fd < num_alloced; ++fd)
      if (!fd_info[fd].in_use)
	break;
    fake_fd = 1;
  }
#elif USE == DOMAINOS_INTERFACE
  {
    static int index;
    static status_$t status;
    static unsigned long length_mapped;

    DBG (1, "sanei_scsi_open: (dev='%s', int * fdp=%p, "
	 "SANEI_SCSI_Sense_Handler handler=%p)\n", dev, fdp, handler);

    /* See if the server process has started yet */
    if (!ServerInitialized)
      {
	static char *CommonAreaPath;

	/* Initialize the server */
	DBG (2, "Initializing Domain Server\n");

	/* Map the area */
	CommonAreaPath = tmpnam (NULL);
	DBG (2, "Domain Server Common area name is '%s'\n", CommonAreaPath);
	com = ms_$crmapl (CommonAreaPath, strlen (CommonAreaPath), 0,
			  sizeof (struct DomainServerCommon), ms_$cowriters,
			  &status);
	DomainErrorCheck (status, "Can't open common area");
	DBG (2, "Domain Server common area mapped\n");

	/* Initialize the eventcounts */
	ec2_$init (&com->CommandAvailable);
	ec2_$init (&com->CommandAccepted);
	ec2_$init (&com->ResultReady);
	ec2_$init (&com->ResultAccepted);
	DBG (2, "Domain Server EC's initialized\n");
	/* Initialize the mutex locks */
	mutex_$init (&com->CommandLock);
	mutex_$init (&com->ResultLock);
	DBG (2, "Domain Server MutexLock's initialized\n");

	/* Initialize pointers to ECs */
	CommandAcceptedPtr[0] = &com->CommandAccepted;
	ResultReadyPtr[0] = &com->ResultReady;
	time_$get_ec (time_$clockh_key, &CommandAcceptedPtr[1], &status);
	DomainErrorCheck (status, "Can't get time EC");
	ResultReadyPtr[1] = CommandAcceptedPtr[1];

	/* Read the ResultReady EC value, to avoid race with the server */
	ResultTriggerValue[0] = ec2_$read (com->ResultReady) + 1;

	/* Now invoke the server */
	ServerPID = fork ();
	if (!ServerPID)
	  {
	    /* I am the child, call the initialization routine */
	    sanei_DomainOS_init (CommonAreaPath);
	    /* We get here when the server is done, so we just exit. */
	    exit (EXIT_SUCCESS);
	  }

	/* The communication area is open, wait for the initial response */
	ResultTriggerValue[1] = (ec2_$read (*ResultReadyPtr[1])
				 + DomainECWaitConstant);
	index = ec2_$wait_svc (ResultReadyPtr, ResultTriggerValue, 2, &status);
	DomainErrorCheck (status, "Error waiting on initial open EC");
	if (index != 1)
	  {
	    DBG (0, "Domain SANE Server never responded on startup\n");
	    /* Send a quit signal to the server */
	    kill (ServerPID, SIGQUIT);
	    return SANE_STATUS_INVAL;
	  }
	/* Register a function to kill the server when we are done */
	assert (!atexit (KillDomainServer));
	ServerInitialized = 1;
      }

    /* Find fake fd. */
    for (fd = 0; fd < num_alloced; ++fd)
      if (!fd_info[fd].in_use)
	break;
    fake_fd = 1;

    /* Send the command open to the server */
    if (!mutex_$lock (&com->CommandLock, Wait16S))
      {
	DBG (0, "Could not obtain mutex lock for Open\n");
	return SANE_STATUS_INVAL;
      }
    com->opcode = Open;
    strcpy (com->open_path, dev);
    CommandTriggerValue[0] = ec2_$read (com->CommandAccepted) + 1;
    ec2_$advance (&com->CommandAvailable, &status);
    DomainErrorCheck (status, "Can't advance CommandAvailable EC");
    CommandTriggerValue[1] = (ec2_$read (*CommandAcceptedPtr[1])
			      + DomainECWaitConstant);
    index = ec2_$wait_svc (CommandAcceptedPtr, CommandTriggerValue, 2,
			   &status);
    DomainErrorCheck (status, "Error waiting on Open command acceptance EC");
    if (index != 1)
      {
	DBG (0, "Domain SANE Server never accepted Open Command\n");
	return SANE_STATUS_INVAL;
      }

    /* Read the result */
    status = com->CommandStatus;
    DomainErrorCheck (status, "Opening device in server");

    /* Now map the data area, and make it temporary */
    DBG (2, "Mapping server's data block, name is '%s'\n", com->open_path);
    pdata = ms_$mapl (com->open_path, strlen (com->open_path), 0,
		      DomainMaxDataSize + DomainSenseSize, ms_$cowriters,
		      ms_$wr, true, &length_mapped, &status);
    DomainErrorCheck (status, "Mapping Server Data block");
    assert (length_mapped >= DomainMaxDataSize + DomainSenseSize);
    ms_$mk_temporary (pdata, &status);
    DomainErrorCheck (status, "Can't make data block temporary");

    /* Release the lock */
    mutex_$unlock (&com->CommandLock);

    if (status.all != status_$ok)
      {
	/* we have a failure, return an error code, and generate debug
	   output */
	DBG (1, "sanei_scsi_open: acquire failed, Domain/OS status is %08x\n",
	     status.all);
	error_$print (status);
	return SANE_STATUS_INVAL;
      }
    else
      {
	/* device acquired, what else to do? */
	fd = com->fd;
      }
  }
#elif USE == FREEBSD_CAM_INTERFACE
   if(1)  {	/* 'if(1) {' makes my emacs c-mode indent better than
		   just '{' unfortunately, this only works if all of
		   the '{' are that way. */

      struct cam_device *curdev;

      fake_fd = 1;
      fd = -1;

      if((curdev = cam_open_pass(dev, O_RDWR, NULL)) != NULL) {
	for (fd = 0; fd < CAM_MAXDEVS && cam_devices[fd] != NULL; fd++)
	  ;

	if (fd == CAM_MAXDEVS)
	  {
	    DBG(1, "sanei_scsi_open: too many CAM devices (%d)\n", fd);
	    cam_close_device(curdev);
	    return SANE_STATUS_INVAL;
	  }
	cam_devices[fd] = curdev;
      }
      else {
	DBG(1, "sanei_scsi_open: can't open device `%s´: %s\n", dev,
	    strerror(errno));
	 return SANE_STATUS_INVAL;
      }
   }
#elif USE == SCO_UW71_INTERFACE 
   { 
     pt_scsi_address_t dev_addr; 
     pt_handle_t pt_handle; 
     int bus, cnt, id, lun; 

     if (4 != sscanf(dev, "/dev/passthru0:%d,%d,%d,%d", &bus, &cnt, &id, &lun))
       { 
	 DBG (1, "sanei_scsi_open: device name `%s´ is not valid: %s\n", 
	      dev, strerror (errno)); 
	 return SANE_STATUS_INVAL; 
       } 
     dev_addr.psa_bus = bus; 
     dev_addr.psa_controller = cnt; 
     dev_addr.psa_target = id; 
     dev_addr.psa_lun = lun; 

     if (0 != pt_open(PASSTHRU_SCSI_ADDRESS, &dev_addr, PT_EXCLUSIVE,
		      &pt_handle)) 
       { 
	 DBG (1, "sanei_scsi_open: pt_open failed: %s!\n", strerror(errno)); 
	 return SANE_STATUS_INVAL; 
       } 
     else 
       fd = (int)pt_handle; 
   } 
#else
#if defined(SGIOCSTL) || (USE == SOLARIS_INTERFACE)
  {
    size_t len;

    /* OpenStep and the Solaris SCG driver are a bit broken in that
       the device name refers to a scsi _bus_, not an individual scsi
       device.  Hence, SANE has to fudge with the device name so we
       know which target to connect to.  For this purpose, we use the
       last character in the device name as the target index.  'a' is
       target 0, 'b', target 1, and so on... */

    len = strlen (dev);
    if (len <= 1)
      {
	DBG (1, "sanei_scsi_open: devicename `%s' too short\n", dev);
	return SANE_STATUS_INVAL;
      }

    real_dev = strdup (dev);
    real_dev[len - 1] = '\0';

    target = dev[len - 1] - 'a';
    if (target > 7)
      {
	DBG (1, "sanei_scsi_open: `%c' is not a valid target id\n",
	     dev[len - 1]);
	return SANE_STATUS_INVAL;
      }
    dev = real_dev;
  }
#endif /* defined(SGIOCSTL) || (USE == SOLARIS_INTERFACE) */

  fd = open (dev, O_RDWR | O_EXCL);
  if (fd < 0)
    {
      SANE_Status status = SANE_STATUS_INVAL;

      if (errno == EACCES)
	status = SANE_STATUS_ACCESS_DENIED;

      DBG (1, "sanei_scsi_open: open of `%s' failed: %s\n",
	   dev, strerror (errno));
      return status;
    }

  if (real_dev)
    free (real_dev);

#ifdef SG_SET_TIMEOUT
  /* Set large timeout since some scanners are slow but do not
     disconnect... ;-( */
  {
    int timeout;
    timeout = SANE_SCSICMD_TIMEOUT * HZ;	/* how about 1 minute? ;-) */
    ioctl (fd, SG_SET_TIMEOUT, &timeout);
  }
#endif

#ifdef SGIOCSTL
  {
    struct scsi_adr sa;

    sa.sa_target = target;
    sa.sa_lun = 0;
    if (ioctl (fd, SGIOCSTL, &sa) == -1)
      {
	DBG (1, "sanei_scsi_open: failed to attach to target: %u (%s)\n",
	     sa.sa_target, strerror (errno));
	return SANE_STATUS_INVAL;
      }
  }
#endif /* SGIOCSTL */
#if USE == LINUX_INTERFACE
  {
    SG_scsi_id sid;
    int ioctl_val;
    int real_buffersize;
    fdparms *fdpa = 0;

    pdata = fdpa = malloc(sizeof(fdparms));
    if (!pdata)
      {
        close(fd);
        return SANE_STATUS_NO_MEM;
      }
    memset(fdpa, 0, sizeof(fdparms));
    /* default: allow only one command to be sent to the SG driver
    */
    fdpa->sg_queue_max = 1;

    /* Try to read the SG version. If the ioctl call is successful,
       we have the new SG driver, and we can increase the buffer size
       using another ioctl call.
       If we have SG version 2.1.35 or above, we can additionally enable
       command queueing.
    */
    if (0 == ioctl(fd, SG_GET_VERSION_NUM, &sg_version))
      {
        DBG(1, "sanei_scsi_open: SG driver version: %i\n", sg_version);

        /* try to reserve a SG buffer of the size specified by *buffersize
        */
        ioctl(fd, SG_SET_RESERVED_SIZE, buffersize);

        /* the set call may not be able to allocate as much memory
           as requested, thus we read the actual buffer size.
        */
        if (0 == ioctl(fd, SG_GET_RESERVED_SIZE, &real_buffersize))
          {
             /* if we got more memory than requested, we stick with
                with the requested value, in order to allow
                sanei_scsi_open to check the buffer size exactly.
             */
             if (real_buffersize < *buffersize)
               *buffersize = real_buffersize;
             fdpa->buffersize = *buffersize;
          }
        else
          {
            DBG(1, "sanei_scsi_open: cannot read SG buffer size - %s\n",
                strerror(errno));
            close(fd);
            return SANE_STATUS_NO_MEM;
          }
        DBG(1, "sanei_scsi_open_extended: using %i bytes as SCSI buffer\n",
            *buffersize);

        if (sg_version >= 20135)
          {
            DBG(1, "trying to enable low level command queueing\n");

            if (0 == ioctl(fd, SG_GET_SCSI_ID, &sid))
              {
                DBG(1, "sanei_scsi_open: Host adapter queue depth: %i\n",
                    sid.d_queue_depth);

                ioctl_val = 1;
                if(0 == ioctl(fd, SG_SET_COMMAND_Q, &ioctl_val))
                  {
                    fdpa->sg_queue_max = sid.d_queue_depth;
                    if (fdpa->sg_queue_max <= 0)
                      fdpa->sg_queue_max = 1;
                  }
              }
          }
      }
    else
      {
        /* we have the old SG driver: */
        if (sanei_scsi_max_request_size < *buffersize)
          *buffersize = sanei_scsi_max_request_size;
          fdpa->buffersize = *buffersize;
      }
    if (sg_version == 0)
      {
        DBG(1, "sanei_scsi_open: using old SG driver logic\n");
      }
    else
      {
        DBG(1, "sanei_scsi_open: SG driver can change buffer size at run time\n");
        if (fdpa->sg_queue_max > 1)
          DBG(1, "sanei_scsi_open: low level command queueing enabled\n");
#ifdef SG_IO
        if (sg_version >= 30000)
          {
            DBG(1, "sanei_scsi_open: using new SG header structure\n");
          }
#endif
      }
  }
#endif /* LINUX_INTERFACE */
#endif /* !DECUNIX_INTERFACE */

  if (fd >= num_alloced)
    {
      size_t new_size, old_size;

      old_size = num_alloced * sizeof (fd_info[0]);
      num_alloced = fd + 8;
      new_size = num_alloced * sizeof (fd_info[0]);
      if (fd_info)
	fd_info = realloc (fd_info, new_size);
      else
	fd_info = malloc (new_size);
      memset ((char *) fd_info + old_size, 0, new_size - old_size);
      if (!fd_info)
	{
	  if (!fake_fd)
	    close (fd);
	  return SANE_STATUS_NO_MEM;
	}
    }
  fd_info[fd].in_use = 1;
  fd_info[fd].sense_handler = handler;
  fd_info[fd].sense_handler_arg = handler_arg;
  fd_info[fd].fake_fd = fake_fd;
  fd_info[fd].bus = bus;
  fd_info[fd].target = target;
  fd_info[fd].lun = lun;
  fd_info[fd].pdata = pdata;

#if USE == SOLARIS_INTERFACE || USE == SOLARIS_USCSI_INTERFACE
  /* verify that the device really exists: */
  if (!unit_ready (fd))
    {
      sanei_scsi_close (fd);
      return SANE_STATUS_INVAL;
    }
#endif
#if USE == SYSVR4_INTERFACE 
  memset(lastrcmd,0,16); /* reinitialize last read command block */ 
#endif

  if (fdp)
    *fdp = fd;

  return SANE_STATUS_GOOD;
}

#if USE == LINUX_INTERFACE
/* The "wrapper" for the old open call */
SANE_Status
sanei_scsi_open (const char *dev, int *fdp,
                SANEI_SCSI_Sense_Handler handler, void *handler_arg)
{
  int i = 0;
  int wanted_buffersize = SCSIBUFFERSIZE, real_buffersize;
  SANE_Status res;
  char *cc, *cc1;
  static int first_time = 1;

  if (first_time) 
    {
      cc = getenv("SANE_SG_BUFFERSIZE");
      if (cc)
        {
          i = strtol(cc, &cc1, 10);
          if (cc != cc1 && i >= 32768)
            wanted_buffersize = i;
        }
    }
  else
    wanted_buffersize = sanei_scsi_max_request_size;

  real_buffersize = wanted_buffersize;
  res = sanei_scsi_open_extended(dev, fdp, handler, handler_arg,
                                 &real_buffersize);

  /* make sure that we got as much memory as we wanted, otherwise
     the backend might be confused
  */
  if (!first_time && real_buffersize != wanted_buffersize)
    {
      DBG(1, "sanei_scsi_open: could not allocate SG buffer memory "
          "wanted: %i got: %i\n", wanted_buffersize, real_buffersize);
      sanei_scsi_close(*fdp);
      return SANE_STATUS_NO_MEM;
    }

  first_time = 0;
  return res;
}
#else
/* dummy for the proposed new open call */
SANE_Status
sanei_scsi_open_extended (const char *dev, int *fdp,
                         SANEI_SCSI_Sense_Handler handler,
                         void *handler_arg, int *buffersize)
{
  SANE_Status res;
  res = sanei_scsi_open(dev, fdp, handler, handler_arg);
  if (sanei_scsi_max_request_size < *buffersize)
    *buffersize = sanei_scsi_max_request_size;
  return res;
}
#endif

void
sanei_scsi_close (int fd)
{
#if USE == LINUX_INTERFACE
  if (fd_info[fd].pdata)
    {
      req *req, *next_req;

      /* make sure that there are no pending SCSI calls */
      sanei_scsi_req_flush_all_extended(fd);

      req = ((fdparms*) fd_info[fd].pdata)->sane_free_list;
      while (req)
        {
          next_req = req->next;
          free(req);
          req = next_req;
        }
      free(fd_info[fd].pdata);
    }
#endif

  fd_info[fd].in_use = 0;
  fd_info[fd].sense_handler = 0;
  fd_info[fd].sense_handler_arg = 0;
  if (!fd_info[fd].fake_fd)
    close (fd);

#if USE == FREEBSD_CAM_INTERFACE
  cam_close_device(cam_devices[fd]);
  cam_devices[fd] = NULL;
#elif USE == DOMAINOS_INTERFACE
  {
    static int index;
    static status_$t status;

    DBG (1, "sanei_scsi_close:  fd=%d\n", fd);

    /* Send the command to the server */
    if (!mutex_$lock (&com->CommandLock, Wait16S))
      {
	DBG (0, "Could not obtain mutex lock for Close command\n");
      }
    else
      {
	com->opcode = Close;
	com->fd = fd;
	CommandTriggerValue[0] = ec2_$read (com->CommandAccepted) + 1;
	ec2_$advance (&com->CommandAvailable, &status);
	DomainErrorCheck (status, "Can't advance CommandAvailable EC");
	CommandTriggerValue[1] = (ec2_$read (*CommandAcceptedPtr[1])
				  + DomainECWaitConstant);
	index = ec2_$wait_svc (CommandAcceptedPtr, CommandTriggerValue, 2,
			       &status);
	DomainErrorCheck (status,
			  "Error waiting on Close command acceptance EC");
	if (index != 1)
	  {
	    DBG (0, "Domain SANE Server never accepted Close Command\n");
	  }

	/* Read the result */
	status = com->CommandStatus;
	/* Release the lock */
	mutex_$unlock (&com->CommandLock);
      }

    /* Unmap the data area */
    ms_$unmap (fd_info[com->fd].pdata, DomainMaxDataSize + DomainSenseSize,
	       &status);
    DomainErrorCheck (status, "Error unmapping device data area");
  }
#endif /* USE == DOMAINOS_INTERFACE */

#if USE == OS2_INTERFACE
  close_aspi ();
#endif /* USE == OS2_INTERFACE */
}


#if USE == DOMAINOS_INTERFACE
# define WE_HAVE_ASYNC_SCSI

void
sanei_scsi_req_flush_all (void)
{
  status_$t status;

  DBG (1, "sanei_scsi_req_flush_all: ()\n");
  /* I have never seen this called, and I'm not sure what to do with it,
     so I guarantee that it will generate a fault, and I can add support
     for it.  */
  assert (1 == 0);
}


SANE_Status
sanei_scsi_req_enter2 (int fd, 
                      const void *cmd, size_t cmd_size,
                      const void *src, size_t src_size,
		      void *dst, size_t * dst_size, void **idp)
{
  SANEI_SCSI_Sense_Handler handler;
  static int index;
  static SANE_Status sane_status;
  static status_$t status;
  static scsi_$status_t SCSIStatus;
  static void *buf_ptr;

  if (dst_size)
    DBG (1, "sanei_scsi_req_enter2: (fd=%x, cmd=%p, cmd_size=%x, "
	 "src=%p, src_size=%x, dst=%p, dst_size=%x, *idp=%p)\n",
	 fd, cmd, cmd_size, src, src_size, dst, *dst_size, idp);
  else
    DBG (1, "sanei_scsi_req_enter2: (fd=%x, cmd=%p, cmd_size=%x, "
	 "src=%p, src_size=%x, dst=%p, dst_size=NULL, *idp=%p)\n", 
	 fd, src, src_size, dst, idp);

  /* Lock the command structure */
  if (!mutex_$lock (&com->CommandLock, mutex_$wait_forever))
    {
      DBG (0, "Could not obtain mutex lock for Enter Command\n");
      return SANE_STATUS_INVAL;
    }

  /* Fill in the command structure */
  com->opcode = Enter;
  com->fd = fd;
  com->cdb_size = cmd_size;
  if (dst_size)
    com->dst_size = *dst_size;
  memcpy (&com->cdb, cmd, com->cdb_size);

  /* figure out if this is a read or a write */
  if (dst_size && *dst_size)
    {
      /* dest buffer specified, must be a read */
      /* assert (com->cdb_size == src_size); */
      com->direction = scsi_read;
      buf_ptr = dst;
      com->buf_size = *dst_size;
    }
  else
    {
      /* no dest buffer, must be a write */
      /* assert (com->cdb_size <= src_size); */
      com->direction = scsi_write;
      buf_ptr = (char *) src;
      com->buf_size = src_size;
      if (com->buf_size)
	memcpy (fd_info[fd].pdata, buf_ptr, com->buf_size);
    }

  CommandTriggerValue[0] = ec2_$read (com->CommandAccepted) + 1;
  ec2_$advance (&com->CommandAvailable, &status);
  DomainErrorCheck (status, "Can't advance CommandAvailable EC");
  CommandTriggerValue[1] = (ec2_$read (*CommandAcceptedPtr[1])
			    + DomainECWaitConstant);
  index = ec2_$wait_svc (CommandAcceptedPtr, CommandTriggerValue, 2, &status);
  DomainErrorCheck (status, "Error waiting on Enter command acceptance EC");
  if (index != 1)
    {
      DBG (0, "Domain SANE Server never accepted Enter Command\n");
      return SANE_STATUS_INVAL;
    }

  /* Read the result */
  status = com->CommandStatus;
  SCSIStatus = com->SCSIStatus;

  /* Release the lock */
  mutex_$unlock (&com->CommandLock);

  /* Now decode the return status */
  if (status.all)
    DBG (1, "Server returned status %08x from Enter command\n", status.all);
  switch (status.all)
    {
    case status_$ok:
      sane_status = SANE_STATUS_GOOD;
      break;
    case scsi_$dma_underrun:
      sane_status = SANE_STATUS_IO_ERROR;
      /* This error is generated by the HP and UMAX backends.  They
         ask for too much data.  For now, the error is ignored :-( */
      sane_status = SANE_STATUS_GOOD;
      break;
    case scsi_$operation_timeout:
      sane_status = SANE_STATUS_DEVICE_BUSY;
      break;
    case scsi_$hdwr_failure:	/* received when both scanners were active */
      sane_status = SANE_STATUS_IO_ERROR;
      break;
    case (status_$ok | 0x80000000):
      /* Special - no Domain/OS error, but fail bit set means to check
         SCSI operation status. */
      DBG (1, "Server returned SCSI status of %08x\n", SCSIStatus);
      switch (SCSIStatus)
	{
	case scsi_check_condition:
	  /* Call the sense handler, if defined */
	  handler = fd_info[com->fd].sense_handler;
	  if (handler)
	    (*handler) (fd, ((u_char *) fd_info[fd].pdata
			     + DomainMaxDataSize),
			fd_info[com->fd].sense_handler_arg);
	  sane_status = SANE_STATUS_IO_ERROR;
	  break;
	case scsi_busy:
	  sane_status = SANE_STATUS_DEVICE_BUSY;
	  break;
	default:
	  DBG (0, "Error - Unrecognized SCSI status %08x returned from "
	       "Enter command\n", SCSIStatus);
	  sane_status = SANE_STATUS_IO_ERROR;
	  exit (EXIT_FAILURE);
	}
      break;
    default:
      DBG (0, "Unmapped status (%08x) returned from Domain SANE Server\n",
	   status.all);
      sane_status = SANE_STATUS_IO_ERROR;
    }

  /* If a read, copy the data into the destination buffer */
  if ((com->direction == scsi_read) && com->dst_size)
    memcpy (buf_ptr, fd_info[fd].pdata, com->dst_size);

  return sane_status;
}


SANE_Status
sanei_scsi_req_wait (void *id)
{
  SANE_Status status;
  DBG (1, "sanei_scsi_req_wait: (id=%p)\n", id);
  status = SANE_STATUS_GOOD;
  return status;
}


SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  SANE_Status status;
  void *id;

  DBG (1, "sanei_scsi_cmd2: (fd=%d)\n", fd);
  status = sanei_scsi_req_enter2 (fd, cmd, cmd_size, src, src_size, dst, dst_size, &id);
  if (status != SANE_STATUS_GOOD)
    return status;
  return sanei_scsi_req_wait (id);
}

#endif /* USE == DOMAINOS_INTERFACE */


#if USE == LINUX_INTERFACE

#include <ctype.h>
#include <signal.h>

#include <sys/time.h>

#define WE_HAVE_ASYNC_SCSI
#define WE_HAVE_FIND_DEVICES

static int pack_id = 0;
static int need_init = 1;
static sigset_t all_signals;

#define ATOMIC(s)					\
do							\
  {							\
    sigset_t old_mask;					\
							\
    if (need_init)					\
      {							\
	need_init = 0;					\
	sigfillset (&all_signals);			\
      }							\
    sigprocmask (SIG_BLOCK, &all_signals, &old_mask);	\
    {s;}						\
    sigprocmask (SIG_SETMASK, &old_mask, 0);		\
  }							\
while (0)

static void
issue (struct req *req)
{
  ssize_t nwritten;
  fdparms *fdp;
  struct req *rp;
  int retries;

  if (!req)
    return;

  fdp = (fdparms*) fd_info[req->fd].pdata;
  DBG (4, "sanei_scsi.issue: %p\n", req);

  rp = fdp->sane_qhead;
  while (rp && rp->running)
    rp = rp->next;

  while (rp && fdp->sg_queue_used < fdp->sg_queue_max)
    {
      retries = 20;
      while (retries)
        {
          errno = 0;
#ifdef SG_IO
          if (sg_version < 30000)
            {
#endif
              ATOMIC (rp->running = 1;
                      nwritten = write (rp->fd, &rp->sgdata.cdb,
                                        rp->sgdata.cdb.hdr.pack_len);
                      if (nwritten != rp->sgdata.cdb.hdr.pack_len)
                        {
                          /* ENOMEM can easily happen, if both command queueing
                             inside the SG driver and large buffers are used.
                             Therefore, if ENOMEM does not occur for the first
                             command in the queue, we simply try to issue
                             it later again.
                          */
                          if (    errno == EAGAIN
                              || (errno == ENOMEM && rp != fdp->sane_qhead))
                            {
                              /* don't try to send the data again, but
                                 wait for the next call to issue()
                              */
                              rp->running = 0;
                            }
                        }
                     );
#ifdef SG_IO
            }
          else
            {
              ATOMIC (rp->running = 1;
                      nwritten = write (rp->fd, &rp->sgdata.sg3.hdr, sizeof(Sg_io_hdr));
                      if (nwritten < 0)
                        {
                          /* ENOMEM can easily happen, if both command queuein
                             inside the SG driver and large buffers are used.
                             Therefore, if ENOMEM does not occur for the first
                             command in the queue, we simply try to issue
                             it later again.
                          */
                          if (    errno == EAGAIN
                              || (errno == ENOMEM && rp != fdp->sane_qhead))
                            {
                              /* don't try to send the data again, but
                                 wait for the next call to issue()
                              */
                              rp->running = 0;
                            }
                        }
                     );
              IF_DBG(if (DBG_LEVEL >= 255) system("cat /proc/scsi/sg/debug 1>&2");)
            }
#endif
          if (rp == fdp->sane_qhead && errno == EAGAIN)
            {
              retries--;
              usleep(10000);
            }
          else
            retries = 0;
        }

#ifndef SG_IO
      if (nwritten != rp->sgdata.cdb.hdr.pack_len)
#else
      if (   (sg_version <  30000 && nwritten != rp->sgdata.cdb.hdr.pack_len)
/* xxx doesn't work yet with kernel 2.3.18:
          || (sg_version >= 30000 && nwritten < sizeof(Sg_io_hdr)))
*/
          || (sg_version >= 30000 && nwritten < 0))
#endif
        {
          if (rp->running)
            {
              DBG (1, "sanei_scsi.issue: bad write (errno=%i) %s %i\n",
                      errno, strerror (errno), nwritten);
              rp->done = 1;
              if (errno == ENOMEM)
                {
                     DBG (1, "sanei_scsi.issue: SG_BIG_BUF inconsistency? "
                             "Check file PROBLEMS.\n");
                     rp->status = SANE_STATUS_NO_MEM;
                }
              else
                   rp->status = SANE_STATUS_IO_ERROR;
               }
          else
            {
              if (errno == ENOMEM)
                DBG(1, "issue: ENOMEM - cannot queue SCSI command. "
                       "Trying again later.\n");
              else
                DBG(1, "issue: EAGAIN - cannot queue SCSI command. "
                       "Trying again later.\n");
            }
          break; /* in case of an error don't try to queue more commands */
        }
      else
	req->status = SANE_STATUS_IO_ERROR;
      fdp->sg_queue_used++;
      rp = rp->next;
     }
}

void
sanei_scsi_req_flush_all_extended (int fd)
{
  fdparms *fdp;
  struct req *req, *next_req;

  fdp = (fdparms*) fd_info[fd].pdata;
  for (req = fdp->sane_qhead; req; req = next_req)
    {
      if (req->running && !req->done)
        {
#ifdef SG_IO
          if (sg_version < 30000)
#endif
            read (fd, &req->sgdata.cdb, req->sgdata.cdb.hdr.reply_len);
#ifdef SG_IO
          else
            read (fd, &req->sgdata.sg3.hdr, sizeof(Sg_io_hdr));
#endif
          ((fdparms*) fd_info[req->fd].pdata)->sg_queue_used--;
        }
      next_req = req->next;

      req->next = fdp->sane_free_list;
      fdp->sane_free_list = req;
    }
  fdp->sane_qhead = fdp->sane_qtail = 0;
}

void
sanei_scsi_req_flush_all ()
{
  int fd, i, j = 0;

  /* sanei_scsi_open allows only one open file handle, so we
     can simply look for the first entry where in_use is set
  */

  fd = num_alloced;
  for (i = 0; i < num_alloced; i++)
    if (fd_info[i].in_use)
      {
        j++;
        fd = i;
      }

  assert(j < 2);

  if (fd < num_alloced)
    sanei_scsi_req_flush_all_extended(fd);
}

SANE_Status
sanei_scsi_req_enter2 (int fd, 
                      const void *cmd, size_t cmd_size,
                      const void *src, size_t src_size,
		      void *dst, size_t * dst_size, void **idp)
{
  struct req *req;
  size_t size;
  fdparms *fdp;

  fdp = (fdparms*) fd_info[fd].pdata;

  if (fdp->sane_free_list)
    {
      req = fdp->sane_free_list;
      fdp->sane_free_list = req->next;
      req->next = 0;
    }
  else
    {
#ifdef SG_IO
      if (sg_version < 30000)
#endif
        size = (sizeof (*req) - sizeof (req->sgdata.cdb.data)
             + fdp->buffersize);
#ifdef SG_IO
      else
        size = sizeof(*req) + MAX_CDB + fdp->buffersize 
             - sizeof(req->sgdata.sg3.data);
#endif
      req = malloc (size);
      if (!req)
	{
	  DBG (1, "sanei_scsi_req_enter: failed to malloc %lu bytes\n",
	       (u_long) size);
	  return SANE_STATUS_NO_MEM;
	}
    }
  req->fd = fd;
  req->running = 0;
  req->done = 0;
  req->status = SANE_STATUS_GOOD;
  req->dst = dst;
  req->dst_len = dst_size;
#ifdef SG_IO
  if (sg_version < 30000)
    {
#endif
      memset (&req->sgdata.cdb.hdr, 0, sizeof (req->sgdata.cdb.hdr));
      req->sgdata.cdb.hdr.pack_id = pack_id++;
      req->sgdata.cdb.hdr.pack_len = cmd_size + src_size 
                                     + sizeof (req->sgdata.cdb.hdr);
      req->sgdata.cdb.hdr.reply_len = (dst_size ? *dst_size : 0)
                                      + sizeof (req->sgdata.cdb.hdr);
      memcpy (&req->sgdata.cdb.data, cmd, cmd_size);
      memcpy (&req->sgdata.cdb.data[cmd_size], src, src_size);
      if (CDB_SIZE (*(u_char *) cmd) != cmd_size)
        {
          if (ioctl(fd, SG_NEXT_CMD_LEN, &cmd_size))
            {
              DBG(1, "sanei_scsi_req_enter2: ioctl to set command length failed\n");
            }
        }
#ifdef SG_IO
    }
  else
    {
      memset (&req->sgdata.sg3.hdr, 0, sizeof (req->sgdata.sg3.hdr));
      req->sgdata.sg3.hdr.interface_id = 'S';
      req->sgdata.sg3.hdr.cmd_len = cmd_size;
      req->sgdata.sg3.hdr.iovec_count = 0;
      req->sgdata.sg3.hdr.mx_sb_len = SENSE_MAX;
      /* read or write? */
      if (dst_size && *dst_size)
        {
          req->sgdata.sg3.hdr.dxfer_direction = SG_DXFER_FROM_DEV;
          req->sgdata.sg3.hdr.dxfer_len = *dst_size;
          req->sgdata.sg3.hdr.dxferp = dst;
        }
      else if (src_size)
        {
          req->sgdata.sg3.hdr.dxfer_direction = SG_DXFER_TO_DEV;
          if (src_size > fdp->buffersize) 
            {
              DBG(1, "sanei_scsi_req_enter2 warning: truncating write data "
                     "from requested %i bytes to allowed %i bytes\n",
                     src_size, fdp->buffersize);
              src_size = fdp->buffersize;
            }
          req->sgdata.sg3.hdr.dxfer_len = src_size;
          memcpy(&req->sgdata.sg3.data[MAX_CDB], src, src_size);
          (const void*) req->sgdata.sg3.hdr.dxferp = &req->sgdata.sg3.data[MAX_CDB];
        }
      else
        {
          req->sgdata.sg3.hdr.dxfer_direction = SG_DXFER_NONE;
        }
      if (cmd_size > MAX_CDB)
        {
          DBG(1, "sanei_scsi_req_enter2 warning: truncating write data "
                 "from requested %i bytes to allowed %i bytes\n",
                 cmd_size, MAX_CDB);
          cmd_size = MAX_CDB;
        }
      memcpy(req->sgdata.sg3.data, cmd, cmd_size);
      (const void*) req->sgdata.sg3.hdr.cmdp = req->sgdata.sg3.data;
      req->sgdata.sg3.hdr.sbp = &(req->sgdata.sg3.sense_buffer[0]);
      /* 1 minute should be ok even for slow scanners */
      req->sgdata.sg3.hdr.timeout = 1000 * SANE_SCSICMD_TIMEOUT;
#ifdef ENABLE_SCSI_DIRECTIO
      /* for the adventurous: If direct IO is used,
         the kernel locks the buffer. This can lead to conflicts,
         if a backend uses shared memory.
         OTOH, direct IO may be faster, and it reduces memory usage
      */
      req->sgdata.sg3.hdr.flags = SG_FLAG_DIRECT_IO;
#else
      req->sgdata.sg3.hdr.flags = 0;
#endif
      req->sgdata.sg3.hdr.pack_id = pack_id++;
      req->sgdata.sg3.hdr.usr_ptr = 0;
    }
#endif

  req->next = 0;
  ATOMIC (if (fdp->sane_qtail)
	  {
            fdp->sane_qtail->next = req;
            fdp->sane_qtail = req;
	  }
	  else
          fdp->sane_qhead = fdp->sane_qtail = req);

  DBG (4, "scsi_req_enter: entered %p\n", req);

  *idp = req;
   issue(req);
 
   DBG(10, "scsi_req_enter: queue_used: %i, queue_max: %i\n",
          ((fdparms*) fd_info[fd].pdata)->sg_queue_used,
          ((fdparms*) fd_info[fd].pdata)->sg_queue_max);
 
  return SANE_STATUS_GOOD;
}

SANE_Status
sanei_scsi_req_wait (void *id)
{
  SANE_Status status = SANE_STATUS_GOOD;
  struct req *req = id;
  ssize_t nread = 0;

  /* we don't support out-of-order completion */
  assert (req == ((fdparms*)fd_info[req->fd].pdata)->sane_qhead);

  DBG (4, "sanei_scsi_req_wait: waiting for %p\n", req);

  issue (req);			/* ensure the command is running */
  if (req->done)
    {
      issue (req->next);	/* issue next command, if any */
      status = req->status;
    }
  else
    {
      fd_set readable;

      /* wait for command completion: */
      FD_ZERO (&readable);
      FD_SET (req->fd, &readable);
      select (req->fd + 1, &readable, 0, 0, 0);

      /* now atomically read result and set DONE: */
#ifdef SG_IO
      if (sg_version < 30000)
        {
#endif
          ATOMIC (nread = read (req->fd, &req->sgdata.cdb,
                                req->sgdata.cdb.hdr.reply_len);
                 req->done = 1);
#ifdef SG_IO
       }
      else
        {
          IF_DBG(if (DBG_LEVEL >= 255) system("cat /proc/scsi/sg/debug 1>&2");)
          ATOMIC (nread = read (req->fd, &req->sgdata.sg3.hdr, sizeof(Sg_io_hdr));
                 req->done = 1);
       }
#endif

      if (fd_info[req->fd].pdata)
        ((fdparms*) fd_info[req->fd].pdata)->sg_queue_used--;

      /* Now issue next command asap, if any.  We can't do this
         earlier since the Linux kernel has space for just one big
         buffer.  */
      issue (req->next);

      DBG (4, "sanei_scsi_req_wait: read %ld bytes\n", (long) nread);

      if (nread < 0)
	{
	  DBG (1, "sanei_scsi_req_wait: read returned %ld (errno=%d)\n",
	       (long) nread, errno);
	  status = SANE_STATUS_IO_ERROR;
	}
      else
	{
#ifdef SG_IO
          if (sg_version < 30000)
            {
#endif
              nread -= sizeof (req->sgdata.cdb.hdr);

              /* check for errors, but let the sense_handler decide.... */
              if ( (req->sgdata.cdb.hdr.result != 0) ||
                  ((req->sgdata.cdb.hdr.sense_buffer[0] & 0x7f) != 0))
                {
                  SANEI_SCSI_Sense_Handler handler
                    = fd_info[req->fd].sense_handler;
                  void *arg = fd_info[req->fd].sense_handler_arg;

                  DBG (1, "sanei_scsi_req_wait: SCSI command complained: %s\n",
                       strerror (req->sgdata.cdb.hdr.result));
                  DBG(10, "sense buffer: %02x %02x %02x %02x %02x %02x %02x %02x"
                          " %02x %02x %02x %02x %02x %02x %02x %02x\n",
                          req->sgdata.cdb.hdr.sense_buffer[0],
                          req->sgdata.cdb.hdr.sense_buffer[1],
                          req->sgdata.cdb.hdr.sense_buffer[2],
                          req->sgdata.cdb.hdr.sense_buffer[3],
                          req->sgdata.cdb.hdr.sense_buffer[4],
                          req->sgdata.cdb.hdr.sense_buffer[5],
                          req->sgdata.cdb.hdr.sense_buffer[6],
                          req->sgdata.cdb.hdr.sense_buffer[7],
                          req->sgdata.cdb.hdr.sense_buffer[8],
                          req->sgdata.cdb.hdr.sense_buffer[9],
                          req->sgdata.cdb.hdr.sense_buffer[10],
                          req->sgdata.cdb.hdr.sense_buffer[11],
                          req->sgdata.cdb.hdr.sense_buffer[12],
                          req->sgdata.cdb.hdr.sense_buffer[13],
                          req->sgdata.cdb.hdr.sense_buffer[14],
                          req->sgdata.cdb.hdr.sense_buffer[15]);
                  /* xxx compilation breaks with the original SG header: */
                  #if 0
                  DBG(10, "target status: %02x host status: %02x"
                          " driver status: %02x\n",
                          req->sgdata.cdb.hdr.target_status,
                          req->sgdata.cdb.hdr.host_status,
                          req->sgdata.cdb.hdr.driver_status);
                  #endif

                 if (   req->sgdata.cdb.hdr.host_status == DID_NO_CONNECT
                     || req->sgdata.cdb.hdr.host_status == DID_BUS_BUSY
                     || req->sgdata.cdb.hdr.host_status == DID_TIME_OUT
                     || req->sgdata.cdb.hdr.driver_status == DRIVER_BUSY
                     || req->sgdata.cdb.hdr.target_status == 0x04) /* BUSY */
                 /* if (req->sgdata.cdb.hdr.result == EBUSY) */
                   status = SANE_STATUS_DEVICE_BUSY;
                 else if (handler)
                   /* sense handler should return SANE_STATUS_GOOD if it
                      decided all was ok afterall */
                   status = (*handler) (req->fd, req->sgdata.cdb.hdr.sense_buffer,
                                        arg);
                 else
                   status = SANE_STATUS_IO_ERROR;
               }

              /* if we are ok so far, copy over the return data */
              if (status == SANE_STATUS_GOOD)
                {
                  if (req->dst)
                    memcpy (req->dst, req->sgdata.cdb.data, nread);

                 if (req->dst_len)
                   *req->dst_len = nread;
               }
#ifdef SG_IO
            }
          else
            {
             /* check for errors, but let the sense_handler decide.... */
             if (   ((req->sgdata.sg3.hdr.info & SG_INFO_CHECK) != 0)
                 || (req->sgdata.sg3.hdr.sb_len_wr > 0 &&
                 ((req->sgdata.sg3.sense_buffer[0] & 0x7f) != 0)))
               {
                 SANEI_SCSI_Sense_Handler handler
                   = fd_info[req->fd].sense_handler;
                 void *arg = fd_info[req->fd].sense_handler_arg;

                 DBG (1, "sanei_scsi_req_wait: SCSI command complained: %s\n",
                      strerror(errno));
                 DBG(10, "sense buffer: %02x %02x %02x %02x %02x %02x %02x %02x"
                         " %02x %02x %02x %02x %02x %02x %02x %02x\n",
                         req->sgdata.sg3.sense_buffer[0],
                         req->sgdata.sg3.sense_buffer[1],
                         req->sgdata.sg3.sense_buffer[2],
                         req->sgdata.sg3.sense_buffer[3],
                         req->sgdata.sg3.sense_buffer[4],
                         req->sgdata.sg3.sense_buffer[5],
                         req->sgdata.sg3.sense_buffer[6],
                         req->sgdata.sg3.sense_buffer[7],
                         req->sgdata.sg3.sense_buffer[8],
                         req->sgdata.sg3.sense_buffer[9],
                         req->sgdata.sg3.sense_buffer[10],
                         req->sgdata.sg3.sense_buffer[11],
                         req->sgdata.sg3.sense_buffer[12],
                         req->sgdata.sg3.sense_buffer[13],
                         req->sgdata.sg3.sense_buffer[14],
                         req->sgdata.sg3.sense_buffer[15]);
                 DBG(10, "target status: %02x host status: %04x"
                         " driver status: %04x\n",
                         req->sgdata.sg3.hdr.status,
                         req->sgdata.sg3.hdr.host_status,
                         req->sgdata.sg3.hdr.driver_status);

                 /* the first three tests below are an replacement of the
                    error "classification" as it was with the old SG driver,
                    the fourth test is new.
                 */
                 if (   req->sgdata.sg3.hdr.host_status == SG_ERR_DID_NO_CONNECT
                     || req->sgdata.sg3.hdr.host_status == SG_ERR_DID_BUS_BUSY
                     || req->sgdata.sg3.hdr.host_status == SG_ERR_DID_TIME_OUT
                     || req->sgdata.sg3.hdr.driver_status == DRIVER_BUSY
                     || req->sgdata.sg3.hdr.masked_status == 0x04) /* BUSY */
                   status = SANE_STATUS_DEVICE_BUSY;
                 else if (handler && req->sgdata.sg3.hdr.sb_len_wr)
                   /* sense handler should return SANE_STATUS_GOOD if it
                      decided all was ok afterall */
                   status = (*handler) (req->fd, req->sgdata.sg3.sense_buffer, arg);
  
                 /* status bits INTERMEDIATE and CONDITION MET should not
                    result in an error; neither should reserved bits
                 */
                 else if (   ((req->sgdata.sg3.hdr.status & 0x2a) == 0)
                          && (req->sgdata.sg3.hdr.host_status == SG_ERR_DID_OK)
                          && (req->sgdata.sg3.hdr.driver_status == SG_ERR_DRIVER_OK))
                   status = SANE_STATUS_GOOD;
                 else
                   status = SANE_STATUS_IO_ERROR;
               }

#if 0
             /* Sometimes the Linux SCSI system reports bogus resid values. 
                Observed with lk 2.4.5, 2.4.13, aic7xxx and sym53c8xx drivers, 
                if command queueing is used. So we better issue only a warning
             */
             if (status == SANE_STATUS_GOOD)
               {
                 if (req->dst_len)
                   { 
                     *req->dst_len -= req->sgdata.sg3.hdr.resid;
                   }
               }
#endif
             if (req->sgdata.sg3.hdr.resid)
               {
                 DBG(1, "sanei_scsi_req_wait: SG driver returned resid %i\n",
                   req->sgdata.sg3.hdr.resid);
                 DBG(1, "                     NOTE: This value may be bogus\n");
               }
           }
#endif
	}
    }

  /* dequeue and release processed request: */
  ATOMIC (((fdparms*) fd_info[req->fd].pdata)->sane_qhead
           = ((fdparms*) fd_info[req->fd].pdata)->sane_qhead->next;
         if (!((fdparms*) fd_info[req->fd].pdata)->sane_qhead)
             ((fdparms*) fd_info[req->fd].pdata)->sane_qtail = 0;
         req->next = ((fdparms*) fd_info[req->fd].pdata)->sane_free_list;
         ((fdparms*) fd_info[req->fd].pdata)->sane_free_list = req);
  return status;
}

SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  SANE_Status status;
  void *id;

  status = sanei_scsi_req_enter2 (fd, cmd, cmd_size, src, src_size, dst, dst_size, &id);
  if (status != SANE_STATUS_GOOD)
    return status;
  return sanei_scsi_req_wait (id);
}

/* The following code (up to and including sanei_scsi_find_devices() )
   is trying to match device/manufacturer names and/or SCSI addressing
   numbers (i.e. <host,bus,id,lun>) with a sg device file name
   (e.g. /dev/sg3).
*/
#define PROCFILE	"/proc/scsi/scsi"
#define DEVFS_MSK	"/dev/scsi/host%d/bus%d/target%d/lun%d/generic"
#define SCAN_MISSES 5

/* Some <scsi/scsi.h> headers don't have the following define */
#ifndef SCSI_IOCTL_GET_IDLUN
#define SCSI_IOCTL_GET_IDLUN 0x5382
#endif

static int lx_sg_dev_base = -1;
static int lx_devfs = -1;

static const struct lx_device_name_list_tag
    {
      const char *prefix;
      char base;
    } lx_dnl[] = {
      {"/dev/sg",  0},
      {"/dev/sg",  'a'},
      {"/dev/uk",  0},
      {"/dev/gsc", 0} };

static int    /* Returns open sg file descriptor, or -1 for no access,
                 or -2 for not found (or other error) */
lx_mk_devicename(int guess_devnum, char *name, size_t name_len)
{
  int dev_fd, k, dnl_len;
  const struct lx_device_name_list_tag * dnp;
  
  dnl_len = NELEMS (lx_dnl);
  k = ((-1 == lx_sg_dev_base) ? 0 : lx_sg_dev_base);
  for (; k < dnl_len; ++k)
    {
      dnp= &lx_dnl[k];
      if (dnp->base)
	  snprintf (name, name_len, "%s%c", dnp->prefix,
		    dnp->base + guess_devnum);
      else
	  snprintf (name, name_len, "%s%d", dnp->prefix, guess_devnum);
      dev_fd = open (name, O_RDWR | O_NONBLOCK);
      if (dev_fd >= 0)
	{
	  lx_sg_dev_base = k;
	  return dev_fd;
	}
      else if ((EACCES == errno) || (EBUSY == errno))
        {
	  lx_sg_dev_base = k;
	  return -1;
	}
      if (-1 != lx_sg_dev_base)
          return -2;
    }
  return -2;
}
  
static int   /* Returns 1 for match, else 0 */
lx_chk_id(int dev_fd, int host, int channel, int id, int lun)
{
#ifdef SG_GET_SCSI_ID_FOUND
  struct sg_scsi_id ssid;

  if ((ioctl(dev_fd, SG_GET_SCSI_ID, &ssid) >= 0))
    {
      DBG(2, "lx_chk_id: %d,%d  %d,%d  %d,%d  %d,%d\n", host, ssid.host_no,
      	  channel, ssid.channel, id, ssid.scsi_id, lun, ssid.lun);
      if ((host == ssid.host_no) &&
	  (channel == ssid.channel) &&
	  (id == ssid.scsi_id) &&
	  (lun == ssid.lun))
          return 1;
      else
          return 0;
    }
#endif
    {
      struct my_scsi_idlun {
          int dev_id;
	  int host_unique_id;
        } my_idlun;
      if (ioctl(dev_fd, SCSI_IOCTL_GET_IDLUN, &my_idlun) >= 0)
        {
	  if (((my_idlun.dev_id & 0xff) == id) &&
	      (((my_idlun.dev_id >> 8) & 0xff) == lun) &&
	      (((my_idlun.dev_id >> 16) & 0xff) == channel))
	      return 1;	/* cheating, assume 'host' number matches */
	}
    }
    return 0;
}

static int  /* Returns 1 if match with 'name' set, else 0 */
lx_scan_sg(int exclude_devnum, char * name, size_t name_len, 
	   int host, int channel, int id, int lun)
{
  int dev_fd, k, missed;

  if (-1 == lx_sg_dev_base)
    return 0;
  for (k = 0, missed = 0; (missed < SCAN_MISSES) && (k < 255); ++k, ++missed)
    {
      DBG(2, "lx_scan_sg: k=%d, exclude=%d, missed=%d\n", k, 
          exclude_devnum, missed);
      if (k == exclude_devnum)
        {
	  missed = 0;
	  continue;  /* assumed this one has been checked already */
        }
      if ((dev_fd = lx_mk_devicename(k, name, name_len)) >= 0)
	{
	  missed = 0;
	  if (lx_chk_id(dev_fd, host, channel, id, lun))
	    {
	      close(dev_fd);
	      return 1;
	    }
	  close(dev_fd);
	}
      else if (-1 == dev_fd)
          missed = 0;  /* no permissions but something found */
    }
  return 0;
}

static int  /* Returns 1 if match, else 0 */
lx_chk_devicename(int guess_devnum, char * name, size_t name_len, int host,
	          int channel, int id, int lun)
{
  int dev_fd;

  if (host < 0)
      return 0;
  if (0 != lx_devfs)
    {  /* simple mapping if we have devfs */
      if (-1 == lx_devfs)
        {
	  if ((dev_fd = lx_mk_devicename(guess_devnum, name, name_len)) >= 0)
	      close(dev_fd);   /* hack to load sg driver module */
	}
      snprintf (name, name_len, DEVFS_MSK, host, channel, id, lun);
      dev_fd = open (name, O_RDWR | O_NONBLOCK);
      if (dev_fd >= 0)
	{
	  close (dev_fd);
	  lx_devfs = 1;
	  DBG (1, "lx_chk_devicename: matched device(devfs): %s\n", name);
	  return 1;
	}
      else if (ENOENT == errno)
          lx_devfs = 0;
    }

  if ((dev_fd = lx_mk_devicename(guess_devnum, name, name_len)) < -1)
    {	/* no candidate sg device file name found, try /dev/sg0,1 */
      if ((dev_fd = lx_mk_devicename(0, name, name_len)) < -1)
        {
	  if ((dev_fd = lx_mk_devicename(1, name, name_len)) < -1)
	      return 0; /* no luck finding sg fd to open */
	}
    }
  if (dev_fd >= 0)
    {
/* now check this fd for match on <host, channel, id, lun> */
      if (lx_chk_id(dev_fd, host, channel, id, lun))
        {
          close(dev_fd);
          DBG (1, "lx_chk_devicename: matched device(direct): %s\n", name);
          return 1;
        }
      close(dev_fd);
    }
/* if mismatch then call scan algorithm */
  if (lx_scan_sg(guess_devnum, name, name_len, host, channel, id, lun))
    {
      DBG (1, "lx_chk_devicename: matched device(scan): %s\n", name);
      return 1;
    }
  return 0;
}

void  /* calls 'attach' function pointer with sg device file name iff match */
sanei_scsi_find_devices (const char *findvendor, const char *findmodel,
			 const char *findtype,
			 int findbus, int findchannel, int findid, int findlun,
			 SANE_Status (*attach) (const char *dev))
{
  #define FOUND_VENDOR  1
  #define FOUND_MODEL   2
  #define FOUND_TYPE    4
  #define FOUND_REV     8
  #define FOUND_HOST    16
  #define FOUND_CHANNEL 32
  #define FOUND_ID      64
  #define FOUND_LUN     128
  #define FOUND_ALL     255

  size_t findvendor_len = 0, findmodel_len = 0, findtype_len = 0;
  char vendor[32], model[32], type[32], revision[32];
  int bus, channel, id, lun;

  int number, i, j, definedd;
  char line[256], dev_name[128], *c1, *c2, ctmp;
  const char *string;
  FILE *proc_fp;
  char *end;
  struct
    {
      const char *name;
      size_t name_len;
      int is_int;		/* integer valued? (not a string) */
      union
	{
	  void *v;		/* avoids compiler warnings... */
	  char *str;
	  int *i;
	}
      u;
    }
  param[] =
    {
      {"Vendor:",  7, 0, {0}},
      {"Model:",   6, 0, {0}},
      {"Type:",    5, 0, {0}},
      {"Rev:",     4, 0, {0}},
      {"scsi",     4, 1, {0}},
      {"Channel:", 8, 1, {0}},
      {"Id:",      3, 1, {0}},
      {"Lun:",     4, 1, {0}}
    };

  param[0].u.str = vendor;
  param[1].u.str = model;
  param[2].u.str = type;
  param[3].u.str = revision;
  param[4].u.i = &bus;
  param[5].u.i = &channel;
  param[6].u.i = &id;
  param[7].u.i = &lun;

  DBG_INIT ();

  proc_fp = fopen (PROCFILE, "r");
  if (!proc_fp)
    {
      DBG (1, "could not open %s for reading\n", PROCFILE);
      return;
    }

  number = bus = channel = id = lun = -1;

  vendor[0] = model[0] = type[0] = '\0';
  if (findvendor)
    findvendor_len = strlen (findvendor);
  if (findmodel)
    findmodel_len = strlen (findmodel);
  if (findtype)
    findtype_len = strlen (findtype);

  definedd = 0;
  while (!feof (proc_fp))
    {
      fgets (line, sizeof (line), proc_fp);
      string = sanei_config_skip_whitespace (line);

      while (*string)
	{
	  for (i = 0; i < NELEMS (param); ++i)
	    {
	      if (strncmp (string, param[i].name, param[i].name_len) == 0)
		{
		  string += param[i].name_len;
		  /* Make sure that we don't read the next parameter name
		     as a value, if the real value consists only of spaces
		  */
		  c2 = string + strlen(string);
		  for (j = 0; j < NELEMS(param); ++j) 
		    {
		      c1 = strstr(string, param[j].name);
		      if ((j != i) && c1 && (c1 < c2))
		        c2 = c1;
		    }
		  ctmp = *c2;
		  *c2 = 0;
		  string = sanei_config_skip_whitespace (string);
		  
		  if (param[i].is_int)
		    {
		      if (*string)
		        {
		          *param[i].u.i = strtol (string, &end, 10);
		          string = (char *) end;
		        }
		      else
		         *param[i].u.i = 0;
		    }
		  else
		    {
		      strncpy (param[i].u.str, string, 32);
		      param[i].u.str[31] = '\0';
		      /* while (*string && !isspace (*string))
			++string;
		      */
		    }
		  /* string = sanei_config_skip_whitespace (string); */
		  *c2 = ctmp;
		  string = c2;
		  definedd |= 1 << i;

		  if (param[i].u.v == &bus)
		    {
		      ++number;
		      definedd = FOUND_HOST;
		    }
		  break;
		}
	    }
	  if (i >= NELEMS (param))
	    ++string;		/* no match */
	}

      if (FOUND_ALL != definedd)
	/* some info is still missing */
	continue;
  
      definedd = 0;
      if ((!findvendor || strncmp (vendor, findvendor, findvendor_len) == 0)
	  && (!findmodel || strncmp (model, findmodel, findmodel_len) == 0)
	  && (!findtype || strncmp (type, findtype, findtype_len) == 0)
	  && (findbus == -1 || bus == findbus)
	  && (findchannel == -1 || channel == findchannel)
	  && (findid == -1 || id == findid)
	  && (findlun == -1 || lun == findlun))
        {
          DBG(2, "sanei_scsi_find_devices: vendor=%s model=%s type=%s\n\t"
  	          "bus=%d chan=%d id=%d lun=%d  num=%d\n", findvendor, 
              findmodel, findtype, bus, channel, id, lun, number);
	  if (lx_chk_devicename(number, dev_name, sizeof (dev_name), bus,
	  		         channel, id, lun)
	      && ( (*attach)(dev_name) != SANE_STATUS_GOOD)) 
	    {
	      fclose (proc_fp);
	      return;
	    }
        }
      vendor[0] = model[0] = type[0] = 0;
      bus = channel = id = lun = -1;
    }
  fclose (proc_fp);
}

#endif /* USE == LINUX_INTERFACE */


#if USE == BSD_INTERFACE

#ifndef HAVE_SCSIREQ_ENTER
static int
scsireq_enter (int fd, scsireq_t * hdr)
{
  return ioctl (fd, SCIOCCOMMAND, hdr);
}
#endif /* !HAVE_SCSIREQ_ENTER */

SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  /* xxx obsolete: size_t cdb_size;
  */
  scsireq_t hdr;
  int result;

/* xxx obsolete: 
  cdb_size = CDB_SIZE (*(u_char *) src);
*/

  memset (&hdr, 0, sizeof (hdr));
  memcpy (hdr.cmd, cmd, cmd_size);
  if (dst_size && *dst_size)
    {
      /* xxx obsolete: assert (cdb_size == src_size);
      */
      hdr.flags = SCCMD_READ;
      hdr.databuf = dst;
      hdr.datalen = *dst_size;
    }
  else
    {
      /* xxx obsolete: assert (cdb_size <= src_size);
      */
      hdr.flags = SCCMD_WRITE;
      /* The old variant:
        hdr.databuf = (char *) src + cdb_size;
        hdr.datalen = src_size;
        xxxxxx huh? Shouldn´t the above line have been src_size - cdb_size)
      */
      hdr.databuf = (char *) src;
      hdr.datalen = src_size;
    }
  hdr.timeout = SANE_SCSICMD_TIMEOUT * 1000;		/* 1 minute timeout */
  hdr.cmdlen = cmd_size;
  hdr.senselen = sizeof (hdr.sense);

  result = scsireq_enter (fd, &hdr);
  if (result < 0)
    {
      DBG (1, "sanei_scsi_cmd: scsi_reqenter() failed: %s\n",
	   strerror (errno));
      return SANE_STATUS_IO_ERROR;
    }
  if (hdr.retsts != SCCMD_OK)
    {
      SANEI_SCSI_Sense_Handler handler;

      DBG (1, "sanei_scsi_cmd: scsi returned with status %d\n", hdr.retsts);
      switch (hdr.retsts)
	{
	case SCCMD_TIMEOUT:
	case SCCMD_BUSY:
	  return SANE_STATUS_DEVICE_BUSY;

	case SCCMD_SENSE:
	  handler = fd_info[fd].sense_handler;
	  if (handler)
	    return (*handler) (fd, &hdr.sense[0],
			       fd_info[fd].sense_handler_arg);
	  /* fall through */
	default:
	  return SANE_STATUS_IO_ERROR;
	}
    }

  if (dst_size)
    *dst_size = hdr.datalen_used;

  return SANE_STATUS_GOOD;
}
#endif /* USE == BSD_INTERFACE */

#if USE == FREEBSD_CAM_INTERFACE
SANE_Status sanei_scsi_cmd2(int fd, 
                           const void *cmd, size_t cmd_size,
                           const void *src, size_t src_size,
			   void *dst, size_t * dst_size) {

   /* xxx obsolete size_t 		cdb_size; 
   */
   struct cam_device	*dev;
   union ccb		*ccb;
   int			rv;
   u_int32_t		ccb_flags;
   char*		data_buf;
   size_t		data_len;
   
   if (fd < 0 || fd > CAM_MAXDEVS || cam_devices[fd] == NULL) {
      fprintf(stderr, "attempt to reference invalid unit %d\n", fd);
      return SANE_STATUS_INVAL;
   }

   /* xxx obsolete: cdb_size = CDB_SIZE (*(u_char *) src);
   */
   dev = cam_devices[fd];
   ccb = cam_getccb(dev);
    
   /* Build the CCB */
   bzero(&(&ccb->ccb_h)[1], sizeof(struct ccb_scsiio));
   bcopy(cmd, &ccb->csio.cdb_io.cdb_bytes, cmd_size);

   /*
    * Set the data direction flags.
    */
   if(dst_size && *dst_size) {
      /* xxx obsolete: assert (cdb_size == src_size);
      */
      ccb_flags = CAM_DIR_IN;
      data_buf = ((char*)(dst));
      data_len = *dst_size;
   }
   else if(src_size > 0) {
      ccb_flags = CAM_DIR_OUT;
      data_buf = ((char*)(src));
      data_len = src_size;
   }
   else {
      ccb_flags = CAM_DIR_NONE;
      data_buf = NULL;
      data_len = 0;
   }

   cam_fill_csio(&ccb->csio,
		 /* retries */ 1,
		 /* cbfncp */ NULL,
		 /* flags */ ccb_flags,
		 /* tag_action */ MSG_SIMPLE_Q_TAG,
		 /* data_ptr */ (u_int8_t *)data_buf,
		 /* dxfer_len */ data_len,
		 /* sense_len */ SSD_FULL_SIZE,
		 /* cdb_len */ cmd_size,
		 /* timeout */ SANE_SCSICMD_TIMEOUT * 1000);

   /* Run the command */
   errno = 0;
   if ((rv = cam_send_ccb(dev, ccb)) == -1) {
      cam_freeccb(ccb);
      return(SANE_STATUS_IO_ERROR);
   }
	
   if ((ccb->ccb_h.status & CAM_STATUS_MASK) != CAM_REQ_CMP) {
      SANEI_SCSI_Sense_Handler handler;

      DBG (1, "sanei_scsi_cmd: scsi returned with status %d\n", 
	   (ccb->ccb_h.status & CAM_STATUS_MASK));

      if((ccb->ccb_h.status & CAM_STATUS_MASK) 
	 == SANE_STATUS_DEVICE_BUSY)
	 return SANE_STATUS_DEVICE_BUSY;

      handler = fd_info[fd].sense_handler;
      if (handler) {
	 SANE_Status st = (*handler) (fd, ((u_char*)(&ccb->csio.sense_data)),
				      fd_info[fd].sense_handler_arg);
	 cam_freeccb(ccb);
	 return st;
      }
      else
	 return SANE_STATUS_IO_ERROR;
   }
   cam_freeccb(ccb);
   return SANE_STATUS_GOOD;
}

#define WE_HAVE_FIND_DEVICES

int
cam_compare_inquiry(int fd, path_id_t path_id,
		    target_id_t target_id, lun_id_t target_lun,
		    const char *vendor, const char *product, const char *type)
{
  struct ccb_dev_match	cdm;
  struct device_match_pattern *pattern;
  struct scsi_inquiry_data *inq;
  int retval = 0;

  /* build ccb for device match */
  bzero(&cdm, sizeof(cdm));
  cdm.ccb_h.func_code = XPT_DEV_MATCH;

  /* result buffer */
  cdm.match_buf_len = sizeof(struct dev_match_result);
  cdm.matches = (struct dev_match_result *)malloc(cdm.match_buf_len);
  cdm.num_matches = 0;

  /* pattern buffer */
  cdm.num_patterns = 1;
  cdm.pattern_buf_len = sizeof(struct dev_match_pattern);
  cdm.patterns = (struct dev_match_pattern *)malloc(cdm.pattern_buf_len);

  /* assemble conditions */
  cdm.patterns[0].type = DEV_MATCH_DEVICE;
  pattern = &cdm.patterns[0].pattern.device_pattern;
  pattern->flags = DEV_MATCH_PATH | DEV_MATCH_TARGET | DEV_MATCH_LUN;
  pattern->path_id = path_id;
  pattern->target_id = target_id;
  pattern->target_lun = target_lun;
  
  if (ioctl(fd, CAMIOCOMMAND, &cdm) == -1) {
    DBG (1, "error sending CAMIOCOMMAND ioctl");
    retval = -1;
    goto ret;
  }

  if ((cdm.ccb_h.status != CAM_REQ_CMP)
      || ((cdm.status != CAM_DEV_MATCH_LAST)
	  && (cdm.status != CAM_DEV_MATCH_MORE))) {
    DBG (1, "got CAM error %#x, CDM error %d\n",
	 cdm.ccb_h.status, cdm.status);
    retval = -1;
    goto ret;
  }

  if (cdm.num_matches == 0) {
    DBG (1, "not found\n");
    retval = -1;
    goto ret;
  }
	    
  if (cdm.matches[0].type != DEV_MATCH_DEVICE) {
    DBG (1, "no device match\n");
    retval = -1;
    goto ret;
  }

  inq = &cdm.matches[0].result.device_result.inq_data;
  if ((vendor && cam_strmatch(inq->vendor, vendor, SID_VENDOR_SIZE)) ||
      (product && cam_strmatch(inq->product, product, SID_PRODUCT_SIZE)))
    retval = 1;
 
 ret:
  free(cdm.patterns);
  free(cdm.matches);
  return(retval);
}

void
sanei_scsi_find_devices (const char *findvendor, const char *findmodel,
			 const char *findtype,
			 int findbus, int findchannel, int findid, int findlun,
			 SANE_Status (*attach) (const char *dev))
{
  int		fd;
  struct ccb_dev_match	cdm;
  struct periph_match_pattern *pattern;
  struct periph_match_result *result;
  int		i;
  char devname[16];

  DBG_INIT();

  if ((fd = open(XPT_DEVICE, O_RDWR)) == -1) {
    DBG (1, "could not open %s\n", XPT_DEVICE);
    return;
  }

  /* build ccb for device match */
  bzero(&cdm, sizeof(cdm));
  cdm.ccb_h.func_code = XPT_DEV_MATCH;

  /* result buffer */
  cdm.match_buf_len = sizeof(struct dev_match_result) * 100;
  cdm.matches = (struct dev_match_result *)malloc(cdm.match_buf_len);
  cdm.num_matches = 0;

  /* pattern buffer */
  cdm.num_patterns = 1;
  cdm.pattern_buf_len = sizeof(struct dev_match_pattern);
  cdm.patterns = (struct dev_match_pattern *)malloc(cdm.pattern_buf_len);

  /* assemble conditions ... findchannel is ignored */
  cdm.patterns[0].type = DEV_MATCH_PERIPH;
  pattern = &cdm.patterns[0].pattern.periph_pattern;
  pattern->flags = PERIPH_MATCH_NAME;
  strcpy(pattern->periph_name, "pass");
  if (findbus != -1) {
    pattern->path_id = findbus;
    pattern->flags |= PERIPH_MATCH_PATH;
  }    
  if (findid != -1) {
    pattern->target_id = findid;
    pattern->flags |= PERIPH_MATCH_TARGET;
  }
  if (findlun != -1) {
    pattern->target_lun = findlun;
    pattern->flags |= PERIPH_MATCH_LUN;
  }

  /* result loop */
  do {
    if (ioctl(fd, CAMIOCOMMAND, &cdm) == -1) {
      DBG (1, "error sending CAMIOCOMMAND ioctl");
      break;
    }

    if ((cdm.ccb_h.status != CAM_REQ_CMP)
	|| ((cdm.status != CAM_DEV_MATCH_LAST)
	    && (cdm.status != CAM_DEV_MATCH_MORE))) {
      DBG (1, "got CAM error %#x, CDM error %d\n", 
	   cdm.ccb_h.status, cdm.status);
      break;
    }
    
    for (i = 0; i < cdm.num_matches; i++) {
      if (cdm.matches[i].type != DEV_MATCH_PERIPH)
	continue;
      result = &cdm.matches[i].result.periph_result;
      DBG (4, "%s%d on scbus%d %d:%d\n",
	      result->periph_name, result->unit_number,
	      result->path_id, result->target_id, result->target_lun);
      if (cam_compare_inquiry(fd, result->path_id,
			      result->target_id, result->target_lun,
			      findvendor, findmodel, findtype) == 0) {
	sprintf(devname, "/dev/%s%d", result->periph_name, result->unit_number);
	(*attach) (devname);
      }
    }
  } while ((cdm.ccb_h.status == CAM_REQ_CMP)
	   && (cdm.status == CAM_DEV_MATCH_MORE));

  free(cdm.patterns);
  free(cdm.matches);
  close(fd);
  return;
}

#endif



#if USE == HPUX_INTERFACE
/* XXX untested code! */
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  struct sctl_io hdr;
  /* xxx obsolete size_t cdb_size;

     cdb_size = CDB_SIZE (*(u_char *) src);
  */

  memset (&hdr, 0, sizeof (hdr));
  memcpy (hdr.cdb, cmd, cmd_size);
  if (dst_size && *dst_size)
    {
      /* xxx obsolete assert (cdb_size == src_size);
      */
      hdr.flags = SCTL_READ;
      hdr.data = dst;
      hdr.data_length = *dst_size;
    }
  else
    {
      /* xxx obsolete assert (cdb_size <= src_size);
      */
      hdr.data = (char *) src;
      hdr.data_length = src_size;
    }
  hdr.cdb_length = cmd_size;
  hdr.max_msecs = SANE_SCSICMD_TIMEOUT * 1000;	/* 1 minute timeout */
  if (ioctl (fd, SIOC_IO, &hdr) < 0)
    {
      DBG (1, "sanei_scsi_cmd: ioctl(SIOC_IO) failed: %s\n",
	   strerror (errno));
      return SANE_STATUS_IO_ERROR;
    }
  if (hdr.cdb_status)
    DBG (1, "sanei_scsi_cmd: SCSI completed with cdb_status=%d\n",
	 hdr.cdb_status);
  if (dst_size)
    *dst_size = hdr.data_xfer;

  if (hdr.sense_xfer > 0 && (hdr.sense[0] & 0x80) && fd_info[fd].sense_handler)
    return (*fd_info[fd].sense_handler) (fd, hdr.sense,
					 fd_info[fd].sense_handler_arg);
  return SANE_STATUS_GOOD;
}
#endif /* USE == HPUX_INTERFACE */


#if USE == OPENSTEP_INTERFACE
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  struct scsi_req hdr;
  /* xxx obsolete size_t cdb_size;

     cdb_size = CDB_SIZE (*(u_char *) src);
  */

  memset (&hdr, 0, sizeof (hdr));
  memcpy (&hdr.sr_cdb, cmd, cmd_size);
  if (dst_size && *dst_size)
    {
      /* xxx obsolete assert (cdb_size == src_size);
      */
      hdr.sr_dma_dir = SR_DMA_RD;
      hdr.sr_addr = dst;
      hdr.sr_dma_max = *dst_size;
    }
  else
    {
      /* xxx obsolete assert (cdb_size <= src_size);
      */
      hdr.sr_dma_dir = SR_DMA_WR;
      hdr.sr_addr = (char *) src;
      hdr.sr_dma_max = src_size;
    }
  hdr.sr_ioto = SANE_SCSICMD_TIMEOUT;		/* I/O timeout in seconds */

  if (ioctl (fd, SGIOCREQ, &hdr) == -1)
    {
      DBG (1, "sanei_scsi_cmd: ioctl(SGIOCREQ) failed: %s\n",
	   strerror (errno));
      return SANE_STATUS_IO_ERROR;
    }
  if (hdr.sr_io_status != 1)
    DBG (1, "sanei_scsi_cmd: SGIOCREQ completed with sr_io_status=%d\n",
	 hdr.sr_io_status);
  if (hdr.sr_scsi_status == SR_IOST_CHKSV && fd_info[fd].sense_handler)
    return (*fd_info[fd].sense_handler) (fd, (u_char *) & hdr.sr_esense,
					 fd_info[fd].sense_handler_arg);
  if (dst_size)
    *dst_size = hdr.sr_dma_xfr;
  return SANE_STATUS_GOOD;
}
#endif /* USE == OPENSTEP_INTERFACE */


#if USE == DECUNIX_INTERFACE
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  u_char sense[64];
  UAGT_CAM_CCB hdr;
  CCB_SCSIIO ccb;
  /* xxx obsolete size_t cdb_size;

     cdb_size = CDB_SIZE (*(u_char *) src);
  */

  memset (&ccb, 0, sizeof (ccb));
  ccb.cam_ch.my_addr = (CCB_HEADER *) & ccb;
  ccb.cam_ch.cam_ccb_len = sizeof (ccb);
  ccb.cam_ch.cam_func_code = XPT_SCSI_IO;
  ccb.cam_ch.cam_path_id = fd_info[fd].bus;
  ccb.cam_ch.cam_target_id = fd_info[fd].target;
  ccb.cam_ch.cam_target_lun = fd_info[fd].lun;
  ccb.cam_ch.cam_flags = 0;

  if (dst_size && *dst_size)
    {
      /* xxx obsolete assert (cdb_size == src_size);
      */
      ccb.cam_ch.cam_flags |= CAM_DIR_IN;
      ccb.cam_data_ptr = (u_char *) dst;
      ccb.cam_dxfer_len = *dst_size;
    }
  else
    {
      /* xxx obsolete assert (cdb_size <= src_size);
      */
      if (0 == src_size)
	ccb.cam_ch.cam_flags |= CAM_DIR_NONE;
      else
	ccb.cam_ch.cam_flags |= CAM_DIR_OUT;
      ccb.cam_data_ptr = (u_char *) src;
      ccb.cam_dxfer_len = src_size;
    }
  ccb.cam_timeout = SANE_SCSICMD_TIMEOUT;		/* set timeout in seconds */
  ccb.cam_cdb_len = cmd_size;
  memcpy (&ccb.cam_cdb_io.cam_cdb_bytes[0], cmd, cmd_size);

  memset (&hdr, 0, sizeof (hdr));
  hdr.uagt_ccb = (CCB_HEADER *) & ccb;
  hdr.uagt_ccblen = sizeof(ccb);
  hdr.uagt_buffer = ccb.cam_data_ptr;
  hdr.uagt_buflen = ccb.cam_dxfer_len;
  hdr.uagt_snsbuf = sense;
  hdr.uagt_snslen = sizeof (sense);
  hdr.uagt_cdb = 0;		/* indicate that CDB is in CCB */
  hdr.uagt_cdblen = 0;

  if (ioctl (cam_fd, UAGT_CAM_IO, &hdr) < 0)
    {
      DBG (1, "sanei_scsi_cmd: ioctl(UAGT_CAM_IO) failed: %s\n",
	   strerror (errno));
      return SANE_STATUS_IO_ERROR;
    }
  if (ccb.cam_ch.cam_status != CAM_REQ_CMP)
    {
      DBG (1, "sanei_scsi_cmd: UAGT_CAM_IO completed with cam_status=%d\n",
	   ccb.cam_ch.cam_status);

      if (ccb.cam_ch.cam_status == CAM_AUTOSNS_VALID
	  && fd_info[fd].sense_handler)
	return (*fd_info[fd].sense_handler) (fd, sense,
					     fd_info[fd].sense_handler_arg);
      else
	return SANE_STATUS_INVAL;
    }
  if (dst_size)
    *dst_size = ccb.cam_dxfer_len;
  return SANE_STATUS_GOOD;
}
#endif /* USE == DECUNIX_INTERFACE */


#if USE == SCO_OS5_INTERFACE
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  static u_char sense_buffer[256];
  struct scsicmd2 sc2;
  struct scsicmd *sc;
  /* xxx obsolete int cdb_size;
  */
  int opcode;
  int i;

  if (fd < 0)
    return SANE_STATUS_IO_ERROR;

  memset (&sc2, 0, sizeof (sc2));
  sc = &sc2.cmd;
  sc2.sense_len = sizeof (sense_buffer);
  sc2.sense_ptr = sense_buffer;

  /* xxx obsolete cdb_size = CDB_SIZE (*(u_char *) src);
  */
  if (dst_size && *dst_size)
    {
      sc->is_write = 0;
      sc->data_ptr = dst;
      sc->data_len = *dst_size;
    }
  else
    {
      sc->data_len = src_size;
      sc->data_ptr = (char *) src;
      sc->is_write = 1;
    }
  memcpy (sc->cdb, cmd, cmd_size);
  sc->cdb_len = cmd_size;

  /* Send the command down via the "pass-through" interface */
  if (ioctl (fd, SCSIUSERCMD2, &sc2) < 0)
    {
      DBG (1, "sanei_scsi_cmd: ioctl(SCSIUSERCMD2) failed: %s\n",
	   strerror (errno));
      return SANE_STATUS_IO_ERROR;
    }
  if (sc->host_sts || sc->target_sts)
    {
      DBG (1, "sanei_scsi_cmd: SCSIUSERCMD2 completed with "
	   "host_sts=%x, target_sts=%x\n", sc->host_sts, sc->target_sts);
      if (fd_info[fd].sense_handler)
	return (*fd_info[fd].sense_handler) (fd, sense_buffer,
					     fd_info[fd].sense_handler_arg);
      return SANE_STATUS_IO_ERROR;
    }
  return SANE_STATUS_GOOD;
}
#endif /* USE == SCO_OS5_INTERFACE */
#if USE == SYSVR4_INTERFACE

/*
 * UNIXWARE 2.x interface
 * (c) R=I+S Rapp Informatik System Germany
 * Email: wolfgang@rapp-informatik.de
 *
 * The driver version should run with other scsi componets like disk
 * attached to the same controller at the same time.
 *
 * Attention : This port needs a sane kernel driver for Unixware 2.x
 * The driver is available in binary pkgadd format
 * Plese mail me.
 *
 */
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void * cmd, size_t cmd_size,
                const void * src, size_t src_size,
		void * dst, size_t * dst_size)
{
  struct sb sb, *sb_ptr; /* Command block and pointer */
  struct scs *scs; /* group 6 command pointer */
  struct scm *scm; /* group 10 command pointer */
  struct scv *scv; /* group 12 command pointer */
  char sense[32]; /* for call of sens req */
  char cmd[16]; /* global for right alignment */
  char * cp;

  /* xxx obsolete size_t cdb_size;

  cdb_size = CDB_SIZE (*(u_char *) src);
  */
  memset (&cmd, 0, 16);
  sb_ptr = &sb;
  sb_ptr->sb_type = ISCB_TYPE;
  cp = (char *) cmd;
  DBG(1, "cdb_size = %d src = {0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x ...}\n", cmd_size,
      cp[0],cp[1],cp[2],cp[3],cp[4],cp[5],cp[6],cp[7],cp[8],cp[9]);
  switch (cmd_size)
    {
    default:
      return SANE_STATUS_IO_ERROR;
    case 6:
      scs = (struct scs *) cmd;
      memcpy(SCS_AD(scs),cmd,SCS_SZ);
      scs->ss_lun = 0;
      sb_ptr->SCB.sc_cmdpt = SCS_AD(scs);
      sb_ptr->SCB.sc_cmdsz = SCS_SZ;
      break;
    case 10:
      scm = (struct scm *) cmd;
      memcpy(SCM_AD(scm),cmd,SCM_SZ);
      scm->sm_lun = 0;
      sb_ptr->SCB.sc_cmdpt = SCM_AD(scm);
      sb_ptr->SCB.sc_cmdsz = SCM_SZ;
      break;
    case 12:
      scv = (struct scv *) cmd;
      memcpy(SCV_AD(scv),cmd,SCV_SZ);
      scv->sv_lun = 0;
      sb_ptr->SCB.sc_cmdpt = SCV_AD(scv);
      sb_ptr->SCB.sc_cmdsz = SCV_SZ;
      break;
    }
  if (dst_size && *dst_size)
    {
      assert (0 == src_size);
      sb_ptr->SCB.sc_mode = SCB_READ;
      sb_ptr->SCB.sc_datapt = dst;
      sb_ptr->SCB.sc_datasz = *dst_size;
    }
  else
    {
      assert (0 <= src_size);
      sb_ptr->SCB.sc_mode = SCB_WRITE;
      sb_ptr->SCB.sc_datapt = (char *) src;
      if ( (sb_ptr->SCB.sc_datasz = src_size) > 0 ) {
	sb_ptr->SCB.sc_mode = SCB_WRITE;
      } else {
	/* also use READ mode if the backends have write with length 0 */
	sb_ptr->SCB.sc_mode = SCB_READ;
      }
    }
  sb_ptr->SCB.sc_time = SANE_SCSICMD_TIMEOUT * 1000; /* 1 min timeout */
  DBG(1, "sanei_scsi_cmd: sc_mode = %d, sc_cmdsz = %d, sc_datasz = %d\n",
      sb_ptr->SCB.sc_mode, sb_ptr->SCB.sc_cmdsz, sb_ptr->SCB.sc_datasz);
  {
    /* do read write by normal read or write system calls */
    /* the driver will lock process in momory and do optimized transfer */
    cp = (char *) cmd;
    switch (*cp)
      {
      case 0x0: /* test unit ready */
	if (ioctl(fd, SS_TEST, NULL) < 0) {
	  return SANE_STATUS_DEVICE_BUSY;
	}
	break;
      case SS_READ:
      case SM_READ:
	if (*dst_size > 0x2048) {
	  sb_ptr->SCB.sc_datapt = NULL;
	  sb_ptr->SCB.sc_datasz = 0;
	  if (memcmp(sb_ptr->SCB.sc_cmdpt,lastrcmd,sb_ptr->SCB.sc_cmdsz) ) {
	    /* set the command block for the next read or write */
	    memcpy(lastrcmd,sb_ptr->SCB.sc_cmdpt,sb_ptr->SCB.sc_cmdsz);
	    if (!ioctl (fd, SDI_SEND, sb_ptr)) {
	      *dst_size = read(fd , dst, *dst_size);
	      if (*dst_size == -1) {
		perror("sanei-scsi:UW-driver read ");
		return SANE_STATUS_IO_ERROR;
	      }
	      break;
	    }
	  } else {
	    *dst_size = read(fd , dst, *dst_size);
	    if (*dst_size == -1) {
	      perror("sanei-scsi:UW-driver read ");
	      return SANE_STATUS_IO_ERROR;
	    }
	    break;
	  }
	  return SANE_STATUS_IO_ERROR;
	}
	/* fall through for small read */
      default:
	if (ioctl (fd, SDI_SEND, sb_ptr) < 0)
	  {
	    DBG(1, "sanei_scsi_cmd: ioctl(SDI_SEND) FAILED: %s\n",
		strerror (errno));
	    return SANE_STATUS_IO_ERROR;
	  }
	if (dst_size) *dst_size = sb_ptr->SCB.sc_datasz;
#ifdef UWSUPPORTED /* at this time not supported by driver */
	if (sb_ptr->SCB.sc_comp_code != SDI_ASW ) {
	  DBG(1, "sanei_scsi_cmd: scsi_cmd failture %x\n",
	      sb_ptr->SCB.sc_comp_code);
	  if (sb_ptr->SCB.sc_comp_code == SDI_CKSTAT && sb_ptr->SCB.sc_status == S_CKCON)
	    if (fd_info[fd].sense_handler) {
	      void *arg = fd_info[fd].sense_handler_arg;
	      return (*fd_info[fd].sense_handler) (fd, (u_char *)&sb_ptr->SCB.sc_link, arg);
	    }
	  return SANE_STATUS_IO_ERROR;
	}
#endif
	break;
      }
    return SANE_STATUS_GOOD;
  }
}
#endif /* USE == SYSVR4_INTERFACE */
#if USE == SCO_UW71_INTERFACE
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  static u_char sense_buffer[24];
  struct scb cmdblk;
  time_t elapsed;
  uint_t compcode, status;
  /* xxx obsolete int cdb_size, mode;
  */
  int mode;
  int i;

  if (fd < 0)
    return SANE_STATUS_IO_ERROR;

  cmdblk.sc_cmdpt = (caddr_t) cmd;
  /* xxx obsolete cdb_size = CDB_SIZE (*(u_char *) src);
  */
  cmdblk.sc_cmdsz = cmd_size;
  cmdblk.sc_time = 60000; /* 60 secs */

  if (dst_size && *dst_size)
    {
      /* xxx obsolete assert (cdb_size == src_size);
      */
      cmdblk.sc_datapt = (caddr_t) dst;
      cmdblk.sc_datasz = *dst_size;
      mode = SCB_READ;
    }
  else
    {
      /* xxx obsolete assert (cdb_size <= src_size);
      */
      cmdblk.sc_datapt = (char *) src;
      cmdblk.sc_datasz = src_size;
      mode = SCB_WRITE;
    }

  if (pt_send(fd, cmdblk.sc_cmdpt, cmdblk.sc_cmdsz, cmdblk.sc_datapt,
	      cmdblk.sc_datasz, mode, cmdblk.sc_time, &elapsed, &compcode,
	      &status, sense_buffer, sizeof(sense_buffer)) != 0)
    {
      DBG (1, "sanei_scsi_cmd: pt_send failed: %s!\n", strerror(errno));
    }
  else
    {
      DBG (2, "sanei_scsi_cmd completed with: compcode = %x, status = %x\n",
	   compcode, status);

      switch (compcode)
	{
	case SDI_ASW: /* All seems well */
	  return SANE_STATUS_GOOD;
	case SDI_CKSTAT:
	  DBG (2, "Sense Data:\n");
	  for (i=0; i<sizeof(sense_buffer); i++)
	    DBG (2, "%.2X ", sense_buffer[i]);
	  DBG (2, "\n");
	  if (fd_info[fd].sense_handler)
	    return (*fd_info[fd].sense_handler)(fd, sense_buffer,
						fd_info[fd].sense_handler_arg);
	  /* fall through */
	default:
	  return SANE_STATUS_IO_ERROR;
	}
    }
}
#endif /* USE == SCO_UW71_INTERFACE */

#if USE == OS2_INTERFACE

#define WE_HAVE_FIND_DEVICES

static int
get_devicename (int bus, int target, int lun, char *name, size_t name_len)
{
  snprintf (name, name_len, "b%dt%dl%d", bus, target, lun);
  DBG (1, "OS/2 searched device is %s\n", name);
  return 0;
}

void
sanei_scsi_find_devices (const char *findvendor, const char *findmodel,
			 const char *findtype,
			 int findbus, int findchannel, int findid, int findlun,
			 SANE_Status (*attach) (const char *dev))
{
  size_t findvendor_len = 0, findmodel_len = 0, findtype_len = 0;
  char vendor[32], model[32], type[32], revision[32];
  int bus, channel, id, lun, number, i;
  char line[256], dev_name[128];
  const char *string;
  FILE *proc_fp;
  char *end;
  const struct
    {
      const char *name;
      size_t name_len;
      int is_int;		/* integer valued? (not a string) */
      union
	{
	  void *v;		/* avoids compiler warnings... */
	  char *str;
	  int *i;
	}
      u;
    }
  param[] =
    {
      {"Vendor:",  7, 0, { 0 }},
      {"Model:",   6, 0, { 0 }},
      {"Type:",    5, 0, { 0 }},
      {"Rev:",     4, 0, { 0 }},
      {"scsi",     4, 1, { 0 }},
      {"Channel:", 8, 1, { 0 }},
      {"Id:",      3, 1, { 0 }},
      {"Lun:",     4, 1, { 0 }}
  };

  param[0].u.str = vendor;
  param[1].u.str = model;
  param[2].u.str = type;
  param[3].u.str = revision;
  param[4].u.i = &bus;
  param[5].u.i = &channel;
  param[6].u.i = &id;
  param[7].u.i = &lun;

  DBG_INIT ();

  open_aspi ();			/* open aspi manager if not already done */

  DBG (2, "find_devices: open temporary file '%s'\n", tmpAspi);
  proc_fp = fopen (tmpAspi, "r");
  if (!proc_fp)
    {
      DBG (1, "could not open %s for reading\n", tmpAspi);
      return;
    }

  number = bus = channel = id = lun = -1;

  vendor[0] = model[0] = type[0] = '\0';
  if (findvendor)
    findvendor_len = strlen (findvendor);
  if (findmodel)
    findmodel_len = strlen (findmodel);
  if (findtype)
    findtype_len = strlen (findtype);

  while (!feof (proc_fp))
    {
      if (!fgets (line, sizeof (line), proc_fp))
	break;			/* at eof exit */

      string = sanei_config_skip_whitespace (line);

      while (*string)
	{
	  for (i = 0; i < NELEMS (param); ++i)
	    {
	      if (strncmp (string, param[i].name, param[i].name_len) == 0)
		{
		  string += param[i].name_len;
		  string = sanei_config_skip_whitespace (string);
		  if (param[i].is_int)
		    {
		      *param[i].u.i = strtol (string, &end, 10);
		      string = (char *) end;
		    }
		  else
		    {
		      strncpy (param[i].u.str, string, 32);
		      param[i].u.str[31] = '\0';
		      while (*string && !isspace (*string))
			++string;
		    }
		  string = sanei_config_skip_whitespace (string);

		  if (param[i].u.v == &bus)
		    ++number;
		  break;
		}
	    }
	  if (i >= NELEMS (param))
	    ++string;		/* no match */
	}

      if ((findvendor && !vendor[0]) || (findmodel && !model[0])
	  || (findtype && !type[0])
       || (findbus >= 0 && bus == -1) || (findchannel >= 0 && channel == -1)
	  || (findlun >= 0 && lun == -1))
	/* some info is still missing */
	continue;

      if ((!findvendor || strncmp (vendor, findvendor, findvendor_len) == 0)
	  && (!findmodel || strncmp (model, findmodel, findmodel_len) == 0)
	  && (!findtype || strncmp (type, findtype, findtype_len) == 0)
	  && (findbus == -1 || bus == findbus)
	  && (findchannel == -1 || channel == findchannel)
	  && (findid == -1 || id == findid)
	  && (findlun == -1 || lun == findlun)
	  && get_devicename (bus, id, lun, dev_name, sizeof (dev_name)) >= 0
	  && (*attach) (dev_name) != SANE_STATUS_GOOD)
	return;

      vendor[0] = model[0] = type[0] = 0;
      bus = channel = id = lun = -1;
    }

  DBG (2, "find_devices: close temporary file '%s'\n", tmpAspi);
  fclose (proc_fp);

  close_aspi ();				/* close aspi manager */
}

/* XXX untested code! */
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  ULONG rc;			/* Returns. */
  unsigned long cbreturn;
  unsigned long cbParam;
  SRB srb;			/* SCSI Request Block */
  /* xxx obsolete size_t cdb_size;
  */

  memset ((char *) &srb, 0, sizeof (srb));	/* Okay, I'm paranoid. */
  /* xxx obsolete cdb_size = CDB_SIZE (*(u_char *) src);*/ /* Size of command block. */
  srb.cmd = SRB_Command;	/* execute SCSI cmd */
  srb.ha_num = fd_info[fd].bus;	/* host adapter number */
  srb.u.cmd.target = fd_info[fd].target;	/* Target SCSI ID */
  srb.u.cmd.lun = fd_info[fd].lun;	/* Target SCSI LUN */
  srb.flags = SRB_Post;		/* posting enabled */
  if (dst_size && *dst_size)
    {
      /* Reading. */
      assert (*dst_size <= sanei_scsi_max_request_size);
      /* xxx obsolete assert (cdb_size == src_size);
      */
      srb.u.cmd.data_len = *dst_size;
      DBG (1, "sanei_scsi_cmd: Reading srb.u.cmd.data_len= %lu\n",srb.u.cmd.data_len); /* fraba */
      srb.flags |= SRB_Read;
    }
  else
    {
      /* Writing. */
      srb.u.cmd.data_len = src_size;
      DBG (1, "sanei_scsi_cmd: Writing srb.u.cmd.data_len= %lu\n",srb.u.cmd.data_len); /* fraba */
      /* xxx obsolete assert (cdb_size <= src_size);
      */
      assert (srb.u.cmd.data_len <= sanei_scsi_max_request_size);
      if (srb.u.cmd.data_len)
	srb.flags |= SRB_Write;
      else
	srb.flags |= SRB_NoTransfer;
      memcpy (aspi_buf, (char *) src, srb.u.cmd.data_len);
    }
  srb.u.cmd.sense_len = 16;	/* length of sense buffer */
  srb.u.cmd.data_ptr = NULL;	/* pointer to data buffer already registered */
  srb.u.cmd.link_ptr = NULL;	/* pointer to next SRB */
  srb.u.cmd.cdb_len = cmd_size;	/* SCSI command length */
  memcpy (&srb.u.cmd.cdb_st[0], (char *) cmd, cmd_size);

  /* Do the command. */
  rc = DosDevIOCtl (driver_handle, 0x92, 0x02, (void *) &srb,
		    sizeof (SRB), &cbParam,
		    (void *) &srb, sizeof (SRB), &cbreturn);

  if (rc)
    {
      DBG (1, "sanei_scsi_cmd: DosDevIOCtl failed. rc= %d \n",rc);
      return SANE_STATUS_IO_ERROR;
    }

  /* Get sense data if available. */
  if ((srb.status == SRB_Aborted || srb.status == SRB_Error) &&
      srb.u.cmd.target_status == SRB_CheckStatus
      && fd_info[fd].sense_handler != 0)
    {
      SANEI_SCSI_Sense_Handler s_handler = fd_info[fd].sense_handler;
      return (*s_handler) (fd, &srb.u.cmd.cdb_st[cmd_size],
			   fd_info[fd].sense_handler_arg);
    }
  if (srb.status != SRB_Done ||
      srb.u.cmd.ha_status != SRB_NoError ||
      srb.u.cmd.target_status != SRB_NoStatus) {
    DBG (1, "sanei_scsi_cmd:  command 0x%02x failed.\n"
            "srb.status= 0x%02x\n"
            "srb.u.chm.ha_status= 0x%02x\n"
            "srb.u.cmd.target_status= 0x%02x\n",
             srb.u.cmd.cdb_st[0],
             srb.status,
             srb.u.cmd.ha_status,
             srb.u.cmd.target_status);
    return SANE_STATUS_IO_ERROR;
  }

  if (dst_size && *dst_size)	/* Reading? */
    memcpy ((char *) dst, aspi_buf, *dst_size);
  return SANE_STATUS_GOOD;
}
#endif /* USE == OS2_INTERFACE */

#if USE == STUBBED_INTERFACE
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  return SANE_STATUS_UNSUPPORTED;
}
#endif /* USE == STUBBED_INTERFACE */

#if USE == IRIX_INTERFACE

#define WE_HAVE_FIND_DEVICES

SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  dsreq_t scsi_req;			/* SCSI request */
  /* xxx obsolete size_t  cdb_size; */	/* Size of SCSI command */
  static u_char	*cmdbuf = NULL,		/* Command buffer */
		*sensebuf = NULL,	/* Request sense buffer */
		*databuf  = NULL;	/* Data buffer */

  /*
   * Allocate the sense and command data buffers as necessary; we have
   * to do this to avoid buffer alignment problems, since some
   * hardware requires these buffers to be 32-bit aligned.
   */
  if (cmdbuf == NULL)
    {
      cmdbuf   = malloc(64);
      sensebuf = malloc(1024); /* may be can reduced to 128 */
      databuf  = malloc(MAX_DATA);

      if (cmdbuf == NULL || sensebuf == NULL || databuf == NULL)
	return SANE_STATUS_NO_MEM;
    }

  /*
   * Build the SCSI request...
   */
  /* xxx obsolete cdb_size = CDB_SIZE (*(u_char *) src);
  */

  DBG(1, "sanei_scsi_cmd: cmd_size = %d\n", cmd_size);

  if (dst != NULL)
    {
      /*
       * SCSI command returning/reading data...
       */
      scsi_req.ds_flags    = DSRQ_READ | DSRQ_SENSE;
      scsi_req.ds_time     = 120 * 1000;
      scsi_req.ds_cmdbuf   = (caddr_t) cmdbuf;
      scsi_req.ds_cmdlen   = cmd_size;
      scsi_req.ds_databuf  = (caddr_t) databuf;
      scsi_req.ds_datalen  = *dst_size;
      scsi_req.ds_sensebuf = (caddr_t) sensebuf;
      scsi_req.ds_senselen = 128; /* 1024 does not work, 128 is tested (O.Rauch) */

      /*
       * Copy command to cmdbuf to assure 32-bit alignment.
       */
      memcpy(cmdbuf, cmd, cmd_size);
    }
  else
    {
      /*
       * SCSI command sending/writing data...
       */
      scsi_req.ds_flags    = DSRQ_WRITE | DSRQ_SENSE;
      scsi_req.ds_time     = 120 * 1000;
      scsi_req.ds_cmdbuf   = (caddr_t) cmdbuf;
      scsi_req.ds_cmdlen   = cmd_size;
      scsi_req.ds_databuf  = (caddr_t) databuf;
      scsi_req.ds_datalen  = src_size;
      scsi_req.ds_sensebuf = (caddr_t) sensebuf;
      scsi_req.ds_senselen = 128;

      /*
       * Copy command and data to local buffers to ensure 32-bit alignment...
       */
      memcpy (cmdbuf, (u_char *) cmd, cmd_size);
      memcpy (databuf, (u_char *) src, src_size);
    }

  bzero(sensebuf, 128);

  /*
   * Do SCSI request...
   */
  if (ioctl (fd, DS_ENTER, &scsi_req) < 0)
    {
      DBG(1, "sanei_scsi_cmd: ioctl failed - %s\n", strerror (errno));
      return SANE_STATUS_IO_ERROR;
    }
 
  DBG(1, "sanei_scsi_cmd: status = %d\n", scsi_req.ds_status);

  /*
   * Set the incoming data size and copy the destination data as needed...
   */
  if (dst != NULL)
    {
      *dst_size = scsi_req.ds_datasent;

      DBG(1, "sanei_scsi_cmd: read %d bytes\n", scsi_req.ds_datasent);

      if (scsi_req.ds_datasent > 0)
        memcpy (dst, databuf, scsi_req.ds_datasent);
    }

  /*
   * Return the appropriate status code...
   */
  if (scsi_req.ds_status != 0)
    {
      if (scsi_req.ds_status == STA_BUSY)
	return SANE_STATUS_DEVICE_BUSY;
      else if (fd_info[fd].sense_handler)
	return (*fd_info[fd].sense_handler) (fd, sensebuf, fd_info[fd].sense_handler_arg);
      else
	return SANE_STATUS_IO_ERROR;
    }
  return SANE_STATUS_GOOD;
}

void
sanei_scsi_find_devices (const char *findvendor, const char *findmodel,
			 const char *findtype,
			 int findbus, int findchannel, int findid, int findlun,
			 SANE_Status (*attach) (const char *dev))
{
  size_t findvendor_len = 0, findmodel_len = 0;
					/* Lengths of search strings */
  inventory_t *inv;			/* Current hardware inventory entry */
  int         bus, id, lun;		/* Current Bus, ID, and LUN */
  char        dev_name[128];		/* SCSI device name */
  int         fd;			/* SCSI file */
  size_t      inqsize;			/* Size of returned inquiry data */
  char        vendor[9],		/* Vendor name */
              model[17];		/* Model/product name */
  u_char      inqdata[128],		/* Inquiry data buffer */
	      inqcommand[6];		/* Inquiry command (0x12) buffer */

  DBG_INIT ();

  vendor[0] = model[0] = '\0';
  if (findvendor)
    findvendor_len = strlen (findvendor);
  if (findmodel)
    findmodel_len = strlen (findmodel);

  if (findvendor != NULL)
    DBG (1, "sanei_scsi_find_devices: looking for vendors starting "
	 "with \"%s\".\n", findvendor);

  if (findmodel != NULL)
    DBG (1, "sanei_scsi_find_devices: looking for models starting "
	 "with \"%s\".\n", findmodel);

  setinvent();

  while ((inv = getinvent()) != NULL)
    {
      if (inv->inv_class != INV_SCSI ||
          (inv->inv_type != INV_SCANNER && inv->inv_type != INV_CPU))
        continue;

      bus = inv->inv_controller;
      id  = inv->inv_unit;
      lun = inv->inv_state >> 8;

      DBG (1, "sanei_scsi_find_devices: found %s on controller %d, "
	   "ID %d, LUN %d.\n",
	   inv->inv_type == INV_SCANNER ? "scanner" : "processor",
	   bus, id, lun);

      if ((findbus >= 0 && bus != findbus) ||
          (findid >= 0 && id != findid) ||
          (findlun >= 0 && lun != findlun))
	{
	  DBG (1, "sanei_scsi_find_devices: ignoring this device.\n");
	  continue;
	}

      sprintf (dev_name, "/dev/scsi/sc%dd%dl%d", bus, id, lun);
      DBG (1, "sanei_scsi_find_devices: device name is \"%s\".\n", dev_name);

     /*
      * Open the SCSI device and get the inquiry data...
      */

      if (sanei_scsi_open(dev_name, &fd, NULL, NULL) != SANE_STATUS_GOOD)
	{
	  DBG (1,
	       "sanei_scsi_find_devices: unable to open device file - %s.\n",
	       strerror(errno));
	  continue;
	}

      DBG (1, "sanei_scsi_find_devices: device fd = %d.\n", fd);

      inqsize = sizeof(inqdata);

      inqcommand[0] = 0x12;
      inqcommand[1] = 0;
      inqcommand[2] = 0;
      inqcommand[3] = sizeof(inqdata) >> 8;
      inqcommand[4] = sizeof(inqdata);
      inqcommand[5] = 0;

      if (sanei_scsi_cmd (fd, inqcommand, sizeof(inqcommand), inqdata,
			  &inqsize) != SANE_STATUS_GOOD)
	{
	  DBG (1,
	       "sanei_scsi_find_devices: unable to get inquiry data - %s.\n",
	       strerror (errno));
	  continue;
	}

      sanei_scsi_close(fd);

      strncpy(vendor, (char *)inqdata + 8, 8);
      vendor[8] = '\0';
      strncpy(model, (char *)inqdata + 16, 16);
      model[16] = '\0';

      DBG (1, "sanei_scsi_find_devices: vendor = \'%s\', model = \'%s'.\n",
           vendor, model);

     /*
      * Compare as necessary...
      */

      if ((findvendor != NULL && strncmp (findvendor, vendor, findvendor_len))
	  || (findmodel != NULL && strncmp (findmodel, model, findmodel_len)))
	{
	  DBG (1, "sanei_scsi_find_devices: ignoring this device.\n");
	  continue;
	}

     /*
      * OK, this one matches, so use it!
      */

      DBG (1, "sanei_scsi_find_devices: attaching this device.\n");

      (*attach) (dev_name);
    }
}
#endif /* USE == IRIX_INTERFACE */

#if USE == AIX_GSC_INTERFACE
SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  scmd_t scmd;
  /* xxx obsolete size_t cdb_size;
  */
  char sense_buf[32];
  char status;

  /* xxx obsolete cdb_size = CDB_SIZE (*(u_char *) src);
  */

  memset (&scmd, 0, sizeof (scmd));
  if (dst_size && *dst_size)
    {
      /* xxx obsolete assert (cdb_size == src_size);
      */
      scmd.rw = 1;
      scmd.data_buf = dst;
      scmd.datalen = *dst_size;
    }
  else
    {
      /* assert (cdb_size <= src_size);
      */
      scmd.data_buf = (char *) src;
      scmd.datalen = src_size;
    }
  scmd.cdb = (char *) cmd;
  scmd.cdblen = cmd_size;
  scmd.timeval = SANE_SCSICMD_TIMEOUT;		/* 1 minute timeout */
  scmd.sense_buf = sense_buf;
  scmd.senselen = sizeof (sense_buf);
  scmd.statusp = &status;
  DBG (1, "sanei_scsi_cmd: scmd.rw = %d, scmd.cdblen = %d, ",
       scmd.rw, scmd.cdblen);
  DBG (1, "scmd.cdb = {0x%x,0x%x,0x%x,0x%x,0x%x,0x%x, ...}\n",
       scmd.cdb[0], scmd.cdb[1], scmd.cdb[2],
       scmd.cdb[3], scmd.cdb[4], scmd.cdb[5]);
  if (ioctl (fd, GSC_CMD, &scmd) < 0)
    {
      DBG (1, "sanei_scsi_cmd: ioctl(SIOC_IO) failed: %s\n",
	   strerror (errno));
      return SANE_STATUS_IO_ERROR;
    }
  if (*scmd.statusp)
    DBG (1, "sanei_scsi_cmd: SCSI completed with status=%d\n", *scmd.statusp);

  DBG (1, "sanei_scsi_cmd: dst = {0x%x,0x%x,0x%x,0x%x,0x%x,0x%x, ...}\n",
       *((char *) dst + 0), *((char *) dst + 1), *((char *) dst + 2),
       *((char *) dst + 3), *((char *) dst + 4), *((char *) dst + 5));

  if (dst_size)
    *dst_size = scmd.datalen;

  if (scmd.senselen > 0
      && (scmd.sense_buf[0] & 0x80) && fd_info[fd].sense_handler)
    return (*fd_info[fd].sense_handler) (fd, (u_char *) scmd.sense_buf,
					 fd_info[fd].sense_handler_arg);
  return SANE_STATUS_GOOD;
}
#endif /* USE == AIX_GSC_INTERFACE */

#if USE == SOLARIS_SG_INTERFACE

#ifndef CCS_SENSE_LEN
# define CCS_SENSE_LEN 18
#endif

SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  struct user_scsi us;
  /* xxx obsolete size_t cdb_size;
  */
  char sensebf[CCS_SENSE_LEN];

  /* xxx obsolete cdb_size = CDB_SIZE (*(u_char *) src);
  */

  /* first put the user scsi structure together.  */
  memset (&us, 0, sizeof (us));
  us.us_cdbp = (caddr_t) cmd;
  us.us_cdblen = cmd_size;
  us.us_sensep = sensebf;
  us.us_senselen = CCS_SENSE_LEN;
  if (dst && dst_size && *dst_size)
    {
      us.us_bufp = (caddr_t) dst;
      us.us_buflen = *dst_size;
      us.us_flags = USER_SCSI_READ;
    }
  else
    {
      us.us_bufp = (caddr_t) src;
      us.us_buflen = src_size;
      us.us_flags = USER_SCSI_WRITE;
    }
  /* now run it */
  if (ioctl (fd, USER_SCSI, &us) < 0)
    return SANE_STATUS_IO_ERROR;
  if (dst_size)
    *dst_size -= us.us_resid;

  return SANE_STATUS_GOOD;
}
#endif /* USE == SOLARIS_SG_INTERFACE */

#if USE == SOLARIS_INTERFACE

#ifndef SC_NOT_READ
# define SC_NOT_READY		0x02
#endif

#ifndef SC_BUSY
# define SC_BUSY		0x08
#endif
#define DEF_TIMEOUT SANE_SCSICMD_TIMEOUT;		/* 1 minute */

/* Choosing one of the following DEF_SCG_FLG's SCG_DISRE_ENA allows
   the SCSI driver to disconnect/reconnect.  SCG_CMD_RETRY allows a
   retry if a retryable error occurs.

   Disallowing SCG_DISRE_ENA slows down the operation of the SCSI bus
   while the scanner is working. If you have severe problems try to
   set it to 0.

   SCG_CMD_RETRY allows the driver to retry some commands.  It should
   normally be set.  For some kinds of odd problems, it may cause the
   machine to hang for some time.  */

#define DEF_SCG_FLG	SCG_DISRE_ENA
/* #define DEF_SCG_FLG  0                               */
/* #define DEF_SCG_FLG  SCG_DISRE_ENA | SCG_CMD_RETRY   */
/* #define DEF_SCG_FLG  SCG_CMD_RETRY                   */

static int d_errs = 100;

static SANE_Status
scsi_cmd (int fd, 
          const void *cmd, size_t cmd_size,
          const void *src, size_t src_size,
	  void *dst, size_t * dst_size, int probing)
{
  struct scg_cmd scmd;
  /* xxx obsolete size_t cdb_size;
  */
  SANEI_SCSI_Sense_Handler handler;

  /* xxx obsolete cdb_size = CDB_SIZE (*(u_char *) src);
  */

  memset (&scmd, 0, sizeof (scmd));
  scmd.flags = DEF_SCG_FLG | (probing ? SCG_SILENT : 0);
  if (dst && dst_size && *dst_size)
    {
      /* xxx obsolete assert (cdb_size == src_size);
      */
      scmd.flags |= SCG_RECV_DATA;
      scmd.addr = dst;
      scmd.size = *dst_size;
    }
  else
    {
      /* xxx obsolete assert (cdb_size <= src_size);
      */
      scmd.addr = (caddr_t) src;
      scmd.size = src_size;
    }
  scmd.cdb_len = cmd_size;
  scmd.sense_len = CCS_SENSE_LEN;
  scmd.target = fd_info[fd].target;
  /* use 2 second timeout when probing, 60 seconds otherwise: */
  scmd.timeout = probing ? 2 : DEF_TIMEOUT;
  memcpy (&scmd.cdb.g0_cdb.cmd, cmd, cmd_size);
  scmd.cdb.cmd_cdb[1] |= fd_info[fd].lun << 5;
  if (ioctl (fd, SCGIO_CMD, &scmd) < 0)
    return SANE_STATUS_IO_ERROR;
  if (dst_size)
    *dst_size = scmd.size - scmd.resid;
  if (scmd.error == 0 && scmd.errno == 0 && *(u_char *) & scmd.scb == 0)
    return SANE_STATUS_GOOD;

  if (scmd.error == SCG_TIMEOUT)
    DBG (0, "sanei_scsi_cmd %x: timeout\n", scmd.cdb.g0_cdb.cmd);
  else if (probing)
    {
      struct scsi_ext_sense *ext_sense = (struct scsi_ext_sense *) &scmd.sense;

      if (scmd.error < SCG_FATAL
	  && ((scmd.sense.code < 0x70 && scmd.sense.code != 0x04)
	    || (scmd.sense.code >= 0x70 && ext_sense->key != SC_NOT_READY)))
	return SANE_STATUS_GOOD;
    }
  else
    {
      char errbf[128];
      int i, rv, lifes;

      handler = fd_info[fd].sense_handler;
      DBG (3, "cmd=%x, error=%d:%s, bsiz=%d, stat=%x,%x,%x, slen=%d\n",
	   scmd.cdb.g0_cdb.cmd, scmd.error, strerror (scmd.errno),
	   ((dst_size != NULL)?(*dst_size):0), scmd.u_scb.cmd_scb[0], 
	   scmd.u_scb.cmd_scb[1], scmd.u_scb.cmd_scb[2], scmd.sense_count);
      *errbf = '\0';
      for (i = 0; i < scmd.sense_count; i++)
	sprintf (errbf + strlen (errbf), "%x,", scmd.u_sense.cmd_sense[i]);
      DBG (3, "sense=%s\n", errbf);

      /* test_unit_ready on a busy unit returns error = 0 or 2 with
         errno=EIO.  I've seen 0 on a CDrom without a CD, and 2 on a
         scanner just busy.

         If (SANE_DEBUG_SANEI_SCSI > 100) lifes =
         SANE_DEBUG_SANEI_SCSI - 100 use up one life for every
         scmd.error abort and dump core when no lifes left
         test_unit_ready commands are not counted.  */
      if (scmd.error)
	{
	  if (sanei_debug_sanei_scsi > 100 &&
	      scmd.cdb.g0_cdb.cmd != SC_TEST_UNIT_READY)
	    {
	      lifes = sanei_debug_sanei_scsi - ++d_errs;
	      DBG (1, "sanei_scsi_cmd: %d lifes left\n", lifes);
	      assert (lifes > 0);
	    }
	  return SANE_STATUS_IO_ERROR;
	}
      if (scmd.u_scb.cmd_scb[0] == SC_BUSY)
	return SANE_STATUS_DEVICE_BUSY;
      if (*(u_char *) & scmd.sense && handler)
	{
	  rv = (*handler) (fd, scmd.u_sense.cmd_sense,
			   fd_info[fd].sense_handler_arg);
	  DBG (2, "sanei_scsi_cmd: sense-handler returns %d\n", rv);
	  return rv;
	}
    }
  return SANE_STATUS_IO_ERROR;
}

SANE_Status
sanei_scsi_cmd2 (int fd, 
                const void *cmd, size_t cmd_size,
                const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  return scsi_cmd (fd, cmd, cmd_size, src, src_size, dst, dst_size, 0);
}

static int
unit_ready (int fd)
{
  static const u_char test_unit_ready[] =
  {0, 0, 0, 0, 0, 0};
  int status;

  status = scsi_cmd (fd, test_unit_ready, sizeof (test_unit_ready), 
                     0, 0, 0, 0, 1);
  return (status == SANE_STATUS_GOOD);
}

#endif /* USE == SOLARIS_INTERFACE */


#if USE == SOLARIS_USCSI_INTERFACE

#define DEF_TIMEOUT SANE_SCSICMD_TIMEOUT;		/* 1 minute */

static int d_errs = 100;
typedef struct scsi_extended_sense extended_sense_t;
typedef struct scsi_inquiry scsi_inquiry_t;

static SANE_Status
scsi_cmd (int fd, 
          const void *cmd, size_t cmd_size,
          const void *src, size_t src_size,
          void *dst, size_t * dst_size, int probing)
{
  struct uscsi_cmd  us;
  scsi_inquiry_t   inquiry, *iq = &inquiry;
  extended_sense_t sense, *sp = &sense;
  SANEI_SCSI_Sense_Handler handler;

  memset (&us, 0, sizeof (us));
  memset (sp, 0, sizeof (*sp));
  
  us.uscsi_flags = USCSI_SILENT | USCSI_RQENABLE | USCSI_DIAGNOSE;
  us.uscsi_timeout = probing ? 2 : DEF_TIMEOUT;
  us.uscsi_rqbuf = (caddr_t)sp;         /* sense data address */
  us.uscsi_rqlen = sizeof(extended_sense_t); /* length of sense data */

  if (dst && dst_size && *dst_size)
    {
      us.uscsi_flags |= USCSI_READ;
      us.uscsi_bufaddr = (caddr_t)dst;
      us.uscsi_buflen = *dst_size;
    }
  else
    {
      us.uscsi_flags |= USCSI_WRITE;
      us.uscsi_bufaddr = (caddr_t) src;
      us.uscsi_buflen = src_size;
    }

  us.uscsi_cdblen = cmd_size;
  us.uscsi_cdb = (caddr_t)cmd;

  if (ioctl (fd, USCSICMD, &us) < 0)
    return SANE_STATUS_IO_ERROR;

  if (dst_size)
    *dst_size = us.uscsi_buflen - us.uscsi_resid;

  if ((us.uscsi_status & STATUS_MASK) == STATUS_GOOD)
    return SANE_STATUS_GOOD;

  if (sp->es_key == SUN_KEY_TIMEOUT)
    DBG (0, "sanei_scsi_cmd %x: timeout\n", *(char *)cmd);
  else
    {
      char errbf[128];
      int i, rv, lifes;

      handler = fd_info[fd].sense_handler;
      DBG (3, "cmd=%x, scsi_status=%x\n",
           *(char *)cmd, us.uscsi_status);
      *errbf = '\0';

      for (i = 0; i < us.uscsi_rqlen; i++)
        sprintf (errbf + strlen (errbf), "%x,", *(sp + i));

      DBG (3, "sense=%s\n", errbf);

#if 0
      if (us.error)
        {
          if (sanei_debug_sanei_scsi > 100 &&
              scmd.cdb.g0_cdb.cmd != SC_TEST_UNIT_READY)
            {
              lifes = sanei_debug_sanei_scsi - ++d_errs;
              DBG (1, "sanei_scsi_cmd: %d lifes left\n", lifes);
              assert (lifes > 0);
            }
          return SANE_STATUS_IO_ERROR;
        }

      if (scmd.u_scb.cmd_scb[0] == SC_BUSY)
        return SANE_STATUS_DEVICE_BUSY;
#endif

      if (handler)
        {
          rv = (*handler) (fd, (unsigned char *)sp,
                           fd_info[fd].sense_handler_arg);
          DBG (2, "sanei_scsi_cmd: sense-handler returns %d\n", rv);
          return rv;
        }
    }

  return SANE_STATUS_IO_ERROR;
}

SANE_Status
sanei_scsi_cmd2 (int fd, 
                 const void *cmd, size_t cmd_size,
                 const void *src, size_t src_size,
                 void *dst, size_t * dst_size)
{
  return scsi_cmd (fd, cmd, cmd_size, src, src_size, dst, dst_size, 0);
}

static int
unit_ready (int fd)
{
  static const u_char test_unit_ready[] =
  {0, 0, 0, 0, 0, 0};
  int status;

  status = scsi_cmd (fd, test_unit_ready, sizeof (test_unit_ready), 
                     0, 0, 0, 0, 1);
  return (status == SANE_STATUS_GOOD);
}
#endif /* USE == SOLARIS_USCSI_INTERFACE */


#ifndef WE_HAVE_ASYNC_SCSI

SANE_Status

sanei_scsi_req_enter2 (int fd, const void *cmd, size_t cmd_size,
                      const void *src, size_t src_size,
		      void *dst, size_t * dst_size, void **idp)
{
  return sanei_scsi_cmd2 (fd, cmd, cmd_size, src, src_size, dst, dst_size);
}

SANE_Status
sanei_scsi_req_wait (void *id)
{
  return SANE_STATUS_GOOD;
}

void
sanei_scsi_req_flush_all (void)
{
}

void
sanei_scsi_req_flush_all_extended (int fd)
{
}

#endif /* WE_HAVE_ASYNC_SCSI */

SANE_Status sanei_scsi_req_enter (int fd,
                      const void *src, size_t src_size,
		      void *dst, size_t * dst_size, void **idp)
{
  size_t cmd_size = CDB_SIZE (*(char *) src);
  
  if (dst_size && *dst_size)
    assert(src_size == cmd_size);
  else
    assert(src_size >= cmd_size);

  return sanei_scsi_req_enter2(fd,  src, cmd_size, 
                               (char*) src + cmd_size, src_size - cmd_size, 
                               dst, dst_size, idp);
}

SANE_Status
sanei_scsi_cmd (int fd, const void *src, size_t src_size,
		void *dst, size_t * dst_size)
{
  size_t cmd_size = CDB_SIZE (*(char *) src);
  
  if (dst_size && *dst_size)
    assert(src_size == cmd_size);
  else
    assert(src_size >= cmd_size);

  return sanei_scsi_cmd2(fd,  src, cmd_size, 
                             (char*) src + cmd_size, src_size - cmd_size, 
                             dst, dst_size);
}



#ifndef WE_HAVE_FIND_DEVICES

void
sanei_scsi_find_devices (const char *findvendor, const char *findmodel,
			 const char *findtype,
			 int findbus, int findchannel, int findid, int findlun,
			 SANE_Status (*attach) (const char *dev))
{
  DBG_INIT();
  DBG (1, "sanei_scsi_find_devices: not implemented for this platform\n");
}

#endif /* WE_HAVE_FIND_DEVICES */
