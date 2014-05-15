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


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdarg.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "brother_advini.h"

#define  NETSCANLIBMAIN

#include "brother_netdev.h"


#define  SOCKET_TYPE  SOCK_STREAM
//#define  SOCKET_TYPE  SOCK_RAW

#define BRSCAN_PORT (54921)
#define TIMEOUTACK  3
#define HEADERSIZE 3

int log_flags = 0;


//    LOCAL FUNCTION
//      get_address_from_inifile(int *ptype)
//           ptype = pointer to the value of Address Type
//                   it is one of following
//                      ADRTYPE_IPADDRESS
//                      ADRTYPE_NODENAME
//
char *get_address_from_inifile(int dev_index,int *ptype){
  static char address[500];
  char *adr;

  *ptype = ADRTYPE_UNKNOWN;

  if( NULL != get_net_ini_value(dev_index,KEY_IP,address,sizeof(address))){
      *ptype = ADRTYPE_IPADDRESS;
  }
  if(!(*address)){
    if( NULL != get_net_ini_value(dev_index,KEY_NODE,address,sizeof(address))){
      *ptype = ADRTYPE_NODENAME;
    }
  }
  adr = address;
  if(*ptype == ADRTYPE_UNKNOWN){
    adr = NULL;
  }
  return adr;
}


//   GLOBAL FUNCTION
//  int open_device_net(int *pscan_socket_fd,
//		    const char *target, 
//		    int   type)
//                  
//
//
br_net_dev_handle  open_device_net(int dev_index,
		    const char *target, 
		    int   type){
  int rc = -1;
  br_net_dev_handle  h_dev;
  char *address;
  if(target){
    logprintf(DLF_OPENCLOSE_FUNC,"   open_device_net:   index=%d   target=%s   type=%d\n",
	      index,target,type);
  }
  else{
    logprintf(DLF_OPENCLOSE_FUNC,"   open_device_net:   index=%d   type=%d\n",index,type);
  }

  h_dev = calloc(1,sizeof(br_net_dev_handle));
  if(!h_dev)return NULL;

  h_dev->index = dev_index;
  switch(type){
  case ADRTYPE_IPADDRESS:
    rc = open_device_net_sub(h_dev,BRSCAN_PORT,target,NULL);
    break;
  case ADRTYPE_NODENAME:
    rc = open_device_net_sub(h_dev,BRSCAN_PORT,NULL,target);
    break;
  case ADRTYPE_DEPENDONINI:
    address = get_address_from_inifile(dev_index,&type);
    switch(type){
    case ADRTYPE_IPADDRESS:
      logprintf(DLF_OPENCLOSE_FUNC,
		"   ADRTYPE_DEPENDONINI ADRTYPE_IPADDRESS: %s:\n", address);
      rc = open_device_net_sub(h_dev,BRSCAN_PORT,address,NULL);
      break;
    case ADRTYPE_NODENAME:
      logprintf(DLF_OPENCLOSE_FUNC,
		"   ADRTYPE_DEPENDONINI ADRTYPE_NODENAME: %s:\n", address);
      rc = open_device_net_sub(h_dev,BRSCAN_PORT,NULL,address);
      break;
    }
    break;
  }
  if(rc == -1){
    close_device_net(h_dev);
    h_dev = 0;
  }
  return h_dev;
}


// LOCAL FUNC
//
//open_device_net_sub
//
int open_device_net_sub(br_net_dev_handle h_dev,
		    const unsigned short nport ,
		    const char *address, 
		    const char *nodename){
  struct protoent *protocol_ent;
  int result;
  struct hostent *host;
  char  ip_address[4*4*2+1] ;
  char  ip_address2[4*4*2+1] ;
  struct sockaddr_in scan_socket_address;
  struct timeval tv = {TIMEOUTACK,0};
  int rc;
  char ack_buf[32];
  int readsize=0;

  logprintf(DLF_OPENCLOSE_FUNC,"   open_device_net_sub: %x %x %x %x:",
                    &(h_dev->scan_socket_fd),   nport ,    address,     nodename);

  if(address)logprintf(DLF_OPENCLOSE_FUNC,"address = %s",address);
  if(nodename)logprintf(DLF_OPENCLOSE_FUNC,"nodename = %s",nodename);
  logprintf(DLF_OPENCLOSE_FUNC,"\n");
  //
  //   Get protocol number
  //
  protocol_ent = getprotobyname("TCP");
  if(protocol_ent == NULL){
    logprintf_error("oops:proto error\n");
  }
  else{
    logprintf (DLF_OPENCLOSE_DETAIL, 
	       "protocol = %s %d\n",protocol_ent->p_name,protocol_ent->p_proto);
  }
  h_dev->scan_socket_fd = socket(AF_INET, SOCKET_TYPE, protocol_ent->p_proto);

  if (h_dev->scan_socket_fd == -1){
    logprintf_error("open_device_net:soket open error\n");
    return -1;
  }
  //
  //   Get host informations (IP address , etc )
  //
  ip_address[0] = 0;
  if (nodename != NULL && nodename[0] != 0){
    host = gethostbyname(nodename);
    if (host == NULL){
      logprintf_error("open_device_net:IP error (gethostbyname) [%s]\n",nodename);      
    }
    else{
      sprintf(ip_address,"%d.%d.%d.%d",
	 (host->h_addr_list[0])[0] & 0xff,
	 (host->h_addr_list[0])[1] & 0xff,
	 (host->h_addr_list[0])[2] & 0xff,
	 (host->h_addr_list[0])[3] & 0xff);

    //
    //  check address familly
    //
      if(host->h_addrtype != AF_INET ){
	logprintf_error( "open_device_net:incorrect address_family  (%d)\n",host->h_addrtype);
	return -1;
      }

      logprintf(DLF_OPENCLOSE_DETAIL,"host name = %s\n",host->h_name);
    }

  }

  if(ip_address[0] == 0 && address){
    strcpy(ip_address , address);
    host = gethostbyaddr(address, strlen(address),AF_INET);
    if (host == NULL){
      logprintf_error("open_device_net:IP error (gethostbyaddr) :%s",address);
    }
    else{
      sprintf(ip_address2,"%d.%d.%d.%d",
	 (host->h_addr_list[0])[0] & 0xff,
	 (host->h_addr_list[0])[1] & 0xff,
	 (host->h_addr_list[0])[2] & 0xff,
	 (host->h_addr_list[0])[3] & 0xff);
      logprintf(DLF_OPENCLOSE_DETAIL,"gethostbyaddr  %s %s\n",ip_address2,host->h_name);
    }
  }
  if (ip_address[0] == 0){
    if(!address)address = "NULL";
    if(!nodename)nodename = "NULL";

    logprintf_error("open_device_net:Cannot get IP address %s %s\n", address , nodename);
    return -1;
  }



  //
  //  fill the scoket address structure
  //
  scan_socket_address.sin_family = AF_INET;
  scan_socket_address.sin_port = htons(nport);
  //  scan_socket_address.sin_addr.s_addr = htonl(inet_addr(ip_address));
  scan_socket_address.sin_addr.s_addr = inet_addr(ip_address);
  
  //scan_socket_address.sin_addr.s_addr = inet_addr(ip_address);


  logprintf(DLF_OPENCLOSE_DETAIL,"IP address = %s(%08x) port=%d(%d)\n",
	 ip_address,
	 scan_socket_address.sin_addr.s_addr,
	 scan_socket_address.sin_port,nport);

  logprintf (DLF_OPENCLOSE_DETAIL,"begin connect \n");


  //
  //    try to connet
  //
  result = connect(h_dev->scan_socket_fd, (struct sockaddr *)&scan_socket_address,
		   sizeof(struct sockaddr_in));
  logprintf (DLF_OPENCLOSE_DETAIL,"end connect \n");

  if (result == -1){
    logprintf_error("open_device_net: client :%08x:",
		    scan_socket_address.sin_addr.s_addr
		    );
    return -1;
  }

  memset(ack_buf,0,sizeof(ack_buf));

  rc = read_device_net(h_dev,ack_buf,sizeof(ack_buf),&readsize,&tv);
  if(rc < 0 || (strcmp(ack_buf,"+OK 200\015\012") != 0)){
    logprintf(DLF_OPENCLOSE_DETAIL,"open_device_net: ACK error [%s]\n",ack_buf);
    logprintf_error("open_device_net: ack error\n");
    
    return -1;
  }

  logprintf(DLF_OPENCLOSE_DETAIL,"open_device_net: ACK [%s]\n",ack_buf);

  return result; 

}


//GLOBAL FUNCTION
//   close_device_net(int h_dev->scan_socket_fd)
//
//
int close_device_net(br_net_dev_handle h_dev){
  
  logprintf(DLF_OPENCLOSE_FUNC,"close_device_net\n");
  if(h_dev->scan_socket_fd != -1){
    close(h_dev->scan_socket_fd);
  }
  free(h_dev);
  return 0;
}




//GLOBAL FUNCTION
//int read_device_net(int scan_socket_fd,        port handle
//		    char *buffer,                buffer to store the data in it
//		    int size ,                   buffer size
//		    int *preadsize,              read data size
//		    struct timeval *ptimeout)    timeout 
//
int read_device_net(br_net_dev_handle h_dev,
		    char *buffer, 
		    int size , 
		    int *preadsize,
		    struct timeval *ptimeout){

  int rc;
  fd_set  fds;
  FD_ZERO(&fds);
  FD_SET(h_dev->scan_socket_fd,&fds);
  logprintf(DLF_READ_FUNC,"read_device_net start\n");
  *preadsize=0;

  rc = select(FD_SETSIZE,&fds,NULL,NULL,ptimeout);
  switch(rc){
   case 0:
    logprintf(DLF_READ_FUNC,"\nread_device_net: select timeout error\n");
    return -1;
    break;
  case -1:
    logprintf_error("\nread_device_net: select unknown error");
    return -1;
    break;
  default:
    logprintf(DLF_READ_FUNC,"\nread_device_net: select success");
    if(FD_ISSET(h_dev->scan_socket_fd,&fds)){
      memset(buffer,0,size);
      *preadsize=recv(h_dev->scan_socket_fd,buffer,size,0);
      logprintf(DLF_READ_DETAIL,":buffer=%08x,size=%d,readsize=%d\n",buffer,size,*preadsize);
    }
  }
  logprintf(DLF_READ_FUNC,"read_device_net: return %d\n",*preadsize);
  return 0;
}


//GLOBAL FUNC
//int write_device_net(int scan_socket_fd,          port handle
//		     char *buffer,                  buffer write data are stored in
//		     int size ,                     data size
//		     int *pwritesize,               written data size 
//		     struct timeval *ptimeout){     timeout
//

int write_device_net(br_net_dev_handle h_dev,
		     char *buffer, 
		     int size , 
		     int *pwritesize,
		     struct timeval *ptimeout){


  int rc;
  fd_set  fds;
  FD_ZERO(&fds);
  FD_SET(h_dev->scan_socket_fd,&fds);
  logprintf(DLF_WRITE_FUNC,"write_device_net start\n");
  *pwritesize=0;

  rc = select(FD_SETSIZE,NULL,&fds,NULL,ptimeout);
  switch(rc){
   case 0:
    logprintf(DLF_WRITE_FUNC,"write_device_net: select timeout error\n");
    break;
  case -1:
    logprintf_error("write_device_net: select unknown error");
    break;
  default:
    logprintf(DLF_WRITE_FUNC,"\nwrite_device_net: select success ");
    if(FD_ISSET(h_dev->scan_socket_fd,&fds)){
      *pwritesize=send(h_dev->scan_socket_fd,buffer,size,0);
      logprintf(DLF_WRITE_DETAIL,":buffer=%08x,size=%d,readsize=%d\n",buffer,size,*pwritesize);
    }
  }
  return rc;

}





int get_device_id(int index ,  int *idvendor,int *idproduct){
  static char idstring[500],*pid;
  int gidvendor=0,gidproduct=0;
  int find = 0;

  if(gidvendor!=0){
    *idvendor  = gidvendor;
    *idproduct = gidproduct;
  }

  if     ( NULL != (pid=get_net_ini_value(index,
					  KEY_ID,idstring,sizeof(idstring)))){
    find = 1;
  }
  else{
    pid = NULL;
    return -1;
  }

  *idvendor=0; *idproduct=0;
  sscanf(pid ,"%x:%x", idvendor, idproduct);
  //  logprintf(DLF_OPENCLOSE_FUNC,"   get_device_id    %x, %x:\n",*idvendor, *idproduct);

  gidvendor  = *idvendor;
  gidproduct = *idproduct;
  
  if(*idvendor==0 && *idproduct ==0){
    logprintf_error("get_device_id: ERROR");
  }
  return 0;
}






//--------------------for debug----------------------

//  DLF_OPENCLOSE_FUNC
//  DLF_OPENCLOSE_DETAIL
//  DLF_READ_FUNC
//  DLF_READ_DERAIL
//  DLF_WRITE_FUNC
//  DLF_WRITE_DERAIL
//  DLF_ERROR


extern int errno;
char *logfilemode;
int  use_std_fp  = 0;
int  set_log_flags_done = 0;

extern int   g_loglevel;
extern char  g_logfile[];
extern char  g_logmode;


#define USE_STDOUT_FP 1
#define USE_SANE_LOG  2

extern netini *root_netini;
int init_netini();
void set_log_flags(int   flags);

void logprintf(int flag, const char * format,...){
  FILE  *fp_log;
  char buffer[800];
  va_list  argp;
  va_start(argp,format);

  if(root_netini == NULL){
    init_netini();
  }
  if(set_log_flags_done == 0){
    set_log_flags(DLF_AUTO);
    set_log_flags_done =1;
    if(g_logmode != 'a'){
      unlink(g_logfile);
    }
  }
  if ((flag  & log_flags) == 0){
    return ;
  }
  vsprintf(buffer,format,argp);

  if(use_std_fp  == USE_SANE_LOG){
    WriteLog("%s\n",buffer);
    return ;
  }
  if(use_std_fp  == USE_STDOUT_FP){
    printf("%s\n",buffer);
    return ;
  }

  fp_log = fopen(g_logfile,"a");
  if(fp_log){
    fwrite(buffer, strlen(buffer), 1, fp_log);
    fclose(fp_log);
  }
}


void logprintf_error(const char * format,...){
  char buffer[500];
  va_list  argp;
  va_start(argp,format);

  if(root_netini == NULL){
    init_netini();
  }
  if(set_log_flags_done == 0){
    set_log_flags(DLF_AUTO);
    set_log_flags_done =1;
    if(g_logmode != 'a'){
      unlink(g_logfile);
    }
  }

  vsprintf(buffer,format,argp);
  strcat((char *)buffer,(const char *)strerror(errno));

  if(use_std_fp  != USE_SANE_LOG){
    WriteLog("%s\n",buffer);
  }
  logprintf(DLF_ERROR, "%s\n", buffer);
  return;
}


void set_log_flags(int   flags){
  if(flags == (int)DLF_AUTO){
    switch(g_loglevel){
    case 0:
      log_flags = 0;
      break;
    case 1:
      log_flags = DLF_EXCEPTRW;
      break;
    case 2:
      log_flags = DLF_MAX;
      break;
    default :
      log_flags = 0;
      break;
    }

  }

  use_std_fp = 0;

  if( strcmp(g_logfile , "SANELOG" ) == 0){
    use_std_fp = USE_SANE_LOG;
  }
  if( strcmp(g_logfile , "STDOUT" ) == 0){
    use_std_fp = USE_STDOUT_FP;
  }
}


