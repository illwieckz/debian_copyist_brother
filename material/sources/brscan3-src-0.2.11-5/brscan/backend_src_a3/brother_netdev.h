/*

 This file is part of the Brother MFC/DCP backend for SANE.

 This program is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2 of the License, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place, Suite 330, Boston, MA  02111-1307  USA

*/



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//	Source filename: brother_devaccs.h
//
//		Copyright(c) 2005-2008 Brother Industries, Ltd.  
//               All Rights Reserved.
//
//
//	Abstract:
//		
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



#ifndef __NETSCANL___
#define __NETSCANL___

//#ifdef  NETSCANLIBMAIN
//int log_flags = 0;
//#else
//extern int log_flags;
//#endif

#define   DLF_OPENCLOSE_FUNC      (1<<0)
#define   DLF_OPENCLOSE_DETAIL    (1<<1)
#define   DLF_READ_FUNC           (1<<2)
#define   DLF_READ_DETAIL         (1<<3)
#define   DLF_WRITE_FUNC          (1<<4)
#define   DLF_WRITE_DETAIL        (1<<5)
#define   DLF_ERROR               (1<<6)

#define   DLF_MAX                 (0xffff)
#define   DLF_READ                (DLF_READ_FUNC+DLF_READ_DETAIL)  
#define   DLF_WRITE               (DLF_WRITE_FUNC+DLF_WRITE_DETAIL)  
#define   DLF_EXCEPTRW            (DLF_MAX-DLF_READ-DLF_WRITE)

#define   DLF_AUTO                (0xffffffff)

#define   INIFILE                 "/usr/local/Brother/scanner/brsane.cfg"


#define ADRTYPE_IPADDRESS     0
#define ADRTYPE_NODENAME      1
#define ADRTYPE_DEPENDONINI   2
#define ADRTYPE_UNKNOWN       -1



void logprintf(int level, const char * format,...);
void logprintf_error(const char * format,...);



typedef struct {
  int scan_socket_fd;
  int index;
}  *br_net_dev_handle,br_net_dev_handle_body;  //add M-LNX-88



br_net_dev_handle open_device_net(int dev_index,
		    const char *address, 
		    int  type);
int open_device_net_sub(br_net_dev_handle h_dev,
		    const unsigned short nport ,
		    const char *address, 
		    const char *nodename);


int close_device_net(br_net_dev_handle h_dev);



int read_device_net(br_net_dev_handle h_dev,
		    char *buffer, 
		    int size , 
		    int *preadsize,
		    struct timeval *ptimeout);


int write_device_net(br_net_dev_handle h_dev,
		     char *buffer, 
		     int size , 
		     int *pwritesize,
		     struct timeval *ptimeout);

void set_log_flag(int   flags ,char *mode);


void set_log_fp(FILE *fp);

void set_log_by_filename(char *filename);

void close_log();



//GLOBAL FUNC
//    int get_interface_type()
//
//        return   USB
//                 NETWORK
//                 PARALLEL
//
//        default is USB
//
int get_interface_type();

#define   IFTYPE_NET       0
#define   IFTYPE_USB       1
#define   IFTYPE_UNKNOWN   2



int get_device_id(int index,int *idvendor,int *idproduct);



#endif
