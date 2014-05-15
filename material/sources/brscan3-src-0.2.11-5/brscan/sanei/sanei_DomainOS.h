/*
This file defines the structure of the communication area used by the
SANE Domain/OS server.  This area must be initialized before the server
is invoked with a path to the area.

To send an open command to the server, follow these steps:
    1) Obtain a mutex lock on CommandLock.
    2) Fill in opcode and open_path variables.
    3) Advance the CommandAvailable EC.
    4) Wait for the CommandAccepted EC to advance.
    5) Get the CommandStatus, which has the Domain/OS completion status.
       Get the fd value, which will be used for subsequent commands.
       Use the open_path variable to map the data section for the device.
    6) Release the CommandLock mutex lock.

To send a close command to the server, follow these steps:
    1) Obtain a mutex lock on CommandLock.
    2) Fill in appropriate Command Data Areas.
    3) Advance the CommandAvailable EC.
    4) Wait for the CommandAccepted EC to advance.
    5) Get the CommandStatus, which has the Domain/OS completion status.
    6) Release the CommandLock mutex lock.

To send an enter command to the server, follow these steps:
    1) Obtain a mutex lock on CommandLock.
    2) Fill in the fd field.
    3) Advance the CommandAvailable EC.
    4) Wait for the CommandAccepted EC to advance.
    5) Get the handle for the command.
    5) Release the CommandLock mutex lock.

*/


#ifndef DomainSenseSize

/* Amount of data in a sense request */
#define DomainSenseSize 18

/* Maximum amound of data in a transfer, per Domain/OS SCSI spec */
#define DomainMaxDataSize (32 * 1024)

/* Timeout for ec2_$wait calls, in 1/4 second intervals */
#define DomainECWaitConstant 120

typedef enum {Open, Close, Enter, Exit} DomainOSOpCode;

struct DomainServerCommon
   {
   /* Basic communication/synchronization items */
   ec2_$eventcount_t CommandAvailable;
   ec2_$eventcount_t CommandAccepted;
   ec2_$eventcount_t ResultReady;
   ec2_$eventcount_t ResultAccepted;
   mutex_$lock_rec_t CommandLock;
   mutex_$lock_rec_t ResultLock;

   /* Command Data Areas - locked by CommandLock */
   DomainOSOpCode opcode;
   int fd;
   name_$long_pname_t open_path;
   status_$t CommandStatus;
   scsi_$status_t SCSIStatus;
   unsigned long CommandHandle;
   linteger cdb_size;
   scsi_$cdb_t cdb;
   scsi_$direction_t direction;
   size_t dst_size;
   size_t buf_size;

   /* Result data areas */
   status_$t status;
   };

#endif /*DomainSenseSize*/

