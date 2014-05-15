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
// brscan3 - brother sane backend
// Copyright (C) 2005-2008  Brother Industries, Ltd.
//          All Rights Reserved.



#ifndef  _ADVANCED_INIFILE_
#define _ADVANCED_INIFILE_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>



#define pintf_error printf

#define NAMELEN  64

typedef struct _sane_model_info  {
  char                       model_name[NAMELEN];
  char                       friendly_name[NAMELEN];
  int                        pid;
  int                        vid;
  int                        func_type;
  int                        expanded_func_type;    //M-LNX-20
  int                        model_type;
  int                        r_endpoint;
  int                        w_endpoint;
  char                       colmatchDL[NAMELEN];
  char                       colmatchTBL[NAMELEN];  //M-LNX-20
  char                       graylevelTBL[NAMELEN]; //M-LNX-20
  struct _sane_model_info    *next;
  //void     *next;
}  sane_model_info;

//
//expanded_func_type
//
#define  EXP_CAPDUPLEX   (1<<0)



#define MODELSECTION "[Support Model]"




sane_model_info  get_model_info_from_ini_by_product_id(int id);
sane_model_info  *get_p_model_info_from_ini_by_product_id(int id);
int free_inifile_tree();


#define IPDEVICEFORMAT      "DEVICE=%s , %s , %s , IP-ADDRESS=%s "
#define NODEDEVICEFORMAT    "DEVICE=%s , %s , %s , NODENAME=%s "

#define  KEY_DEVICE   0
#define  KEY_MODEL    1
#define  KEY_IP       2
#define  KEY_NODE     3
#define  KEY_ID       4
#define  KEY_CMDLL    5

//---------------------------------------------

#define NETINI_KEYSIZE  32
#define NETINI_VAL_SIZE 256
typedef struct _netini {
  //  char key[NETINI_KEYSIZE];
  //  char value[NETINI_VAL_SIZE];
  char friendlyname[NAMELEN];
  char model[NAMELEN];
  char ip[64];
  char node[NAMELEN];
  char id[64];
  struct _netini *next;
} netini;


#define CONFDIR       "/usr/local/Brother/sane/"


char *get_net_ini_value(int index ,int key, char *value, int size);


int free_net_inifile_tree();

char *get_net_ini_value2(char *key, char *value, int size);
int get_net_device_num();

int ReadModelInfo2(char *lpReturnedString,int nSize);
int ReadModelInfoSize2(int *size, int *record);

sane_model_info  *get_p_model_info_by_index(int index);

#define  FOR_OEM

//files depend on besane revisions


#ifndef BRSANESUFFIX
#include "brother_sanesufix.h"
#endif

#if BRSANESUFFIX == 1

#define  MODELINIDIR     "/usr/local/Brother/sane/models"
#define  CONFFILE        "/usr/local/Brother/sane/brsanenetdevice.cfg"
#define  MAININIFILE     "/usr/local/Brother/sane/Brsane.ini"
#define  BRSANETMPFILE   "/tmp/brsane_tempfile_XXXXXX"
#define  SANESAVEFILE    ".brsaneconfig"
#define  S_BRSANESUFFIX  ""

#elif BRSANESUFFIX == 2

#define  MODELINIDIR     "/usr/local/Brother/sane/models2"
#define  CONFFILE        "/usr/local/Brother/sane/brsanenetdevice2.cfg"
#define  MAININIFILE     "/usr/local/Brother/sane/Brsane2.ini"
#define  BRSANETMPFILE   "/tmp/brsane_tempfile2_XXXXXX"
#define  SANESAVEFILE    ".brsaneconfig2"
#define  S_BRSANESUFFIX  "2"

#elif BRSANESUFFIX == 3
#define  MODELINIDIR     "/usr/local/Brother/sane/models3"
#define  CONFFILE        "/usr/local/Brother/sane/brsanenetdevice3.cfg"
#define  MAININIFILE     "/usr/local/Brother/sane/Brsane3.ini"
#define  BRSANETMPFILE   "/tmp/brsane_tempfile3_XXXXXX"
#define  SANESAVEFILE    ".brsaneconfig3"
#define  S_BRSANESUFFIX  "3"

#endif


// for brsaneconfig
int check_inifile_name(char *file);
int scan_model_directory();
void reform_line(char *line);
int debug_disp_model_info();
char *get_net_ini_value_by_name(char *name ,int key, char *value, int size);


#endif
