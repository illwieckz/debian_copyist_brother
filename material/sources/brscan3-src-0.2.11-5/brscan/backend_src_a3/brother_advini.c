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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <curses.h>
#include <unistd.h>

#include "brother_advini.h"
#include "brother_bugchk.h"

#ifdef DEBUGMAIN
#define  WriteLog printf
#else 
#define  WriteLog if(0)printf
#endif  //DEBUGMAIN



sane_model_info ERRORINFO={"","",-1,-1,-1,-1,-1,-1,-1,"","","",NULL};



sane_model_info *root_adv_model = NULL;

char g_logfile[260];
int  g_loglevel;
char  g_logmode;



//++++++++++++++++++
// Local functions
//++++++++++++++++++

//
//  delete space and tab code
//
void reform_line(char *line){
  char *s,c;
  s = line;
  while((c = *(line++))){
    if(c != ' ' && c != '\t' && c != 0x0a && c != 0x0d && c != 0x0c )*(s++) = c;
  }
  *s = 0;
}



//------------------------------------------
// get model informations from lines of inifile
// and register the gotten information to the
// model information list chain
//     void *def  must be static pointer 
//------------------------------------------
char *get_1_element(char *line,void *element,char *format,void *def){
  //
  //  void *def  must be static pointer 
  //
  char *p;

  if(line == NULL){
    *(int *)element = *(int *)def;
    return NULL;
  }
  p = strchr(line,',');
  if(p)*p = 0;
  sscanf(line,format,element);
  if(p){
    return p + 1;
  }
  else{
    return NULL;
  }
  return NULL; //dummy
}

int parse_and_add_model_info(char *line){
  static sane_model_info *current;
  char tmp[50],*p;
  sane_model_info *current_t;
  static int  def_vid = 0x4f9;
  static int  def_int = 0xffff;
  static char def_str[2] ="";
  reform_line(line);
  current_t = (sane_model_info *)CALLOC(sizeof(sane_model_info),1);

  if (current_t == NULL )return -1;

  if(root_adv_model == NULL){
    root_adv_model = current = current_t;
  }
  else{
    current->next = current_t;
    current = current->next;
  }
  current_t->r_endpoint = -1;
  current_t->w_endpoint = -1;


  //  sscanf(line,"0x%x,0x%x,%d,%d,%s,%d,%d",
  //	 &current_t->vid,
  //	 &current_t->pid,
  //	 &current_t->func_type,
  //	 &current_t->model_type,
  //	 &current_t->model_name[0],
  //	 &current_t->r_endpoint,
  //     &current_t->w_endpoint);

  

  line = get_1_element(line,tmp,                      "%s",&def_str);
  current_t->pid = def_int;
  current_t->vid = def_vid;
  if((p=strstr(tmp , ":0x")) != NULL ){
    sscanf(tmp,"0x%x:0x%x",&current_t->vid,&current_t->pid);
  }
  else{
    sscanf(tmp,"0x%x",&current_t->pid);
  }
  line = get_1_element(line,&current_t->func_type,    "%d"  ,&def_int);
  line = get_1_element(line,&current_t->model_type,   "%d"  ,&def_int);
  line = get_1_element(line,&current_t->model_name[0],"%s"  ,&def_str);
  line = get_1_element(line,&current_t->r_endpoint,   "%d"  ,&def_int);
  line = get_1_element(line,&current_t->w_endpoint,   "%d"  ,&def_int);
  line = get_1_element(line,&current_t->colmatchDL[0],"%s"  ,&def_str);

  // M-LNX-20
  line = get_1_element(line,&current_t->colmatchTBL[0],"%s"  ,&def_str);
  line = get_1_element(line,&current_t->graylevelTBL[0],"%s"  ,&def_str);
  current_t->expanded_func_type = current_t->func_type / 100;
  current_t->func_type = current_t->func_type % 100;
  // M-LNX-20
  return 0;
}




//----------------------------------------
// get model information structure
//  and registerd to the model information
//  list chain
//----------------------------------------
#define MODELSECTION "[Support Model]"

int get_model_structure(char *file){
  FILE *fp;
  int result;
  char line[1024],*line2;             //M-LNX-110
  if((fp=fopen(file,"r")) == NULL){
    WriteLog("inifile open error [%s]\n",file);
    return 0;
  }
    // serch "[Support Model]"
  while(fgets(line,sizeof(line),fp)){
    if(strncmp(line,MODELSECTION,sizeof(MODELSECTION)-1) == 0)break;
  }
  result = 0;
  while(fgets(line,sizeof(line),fp)){
    //    printf("file = %s\n",file);
    //    printf("line = %s\n\n-----------------------\n\n",line);

    line2 = line;                                          //M-LNX-110
    while(*line2 == ' ' || *line2 == '\t' )line2++;        //M-LNX-110

    if(line2[0] == '\n')continue;                          //M-LNX-110
    if(line2[0] == '#')continue;                           //M-LNX-110
    if(line2[0] == '[')break;                              //M-LNX-110
    result ++;
    //WriteLog("parse line %s",line);
    parse_and_add_model_info(line2);                       //M-LNX-110
  }
  fclose(fp);
  return result;
}


//------------------------------------------
// check file name has ".ini" extension or not
//
//------------------------------------------
int check_inifile_name(char *file){
  if(file == NULL)return -1;
  if(strlen(file) <= 3)return -1;
  if(file[0] == '.')return -1;

  if (strcmp(file+strlen(file)-4,".ini") != 0){
    return -1;
  }
  return 0;
}




//------------------------------------
//  scan all ini-file in the model directory
//   and registers the model informations 
//------------------------------------
int scan_model_directory(){
  DIR *dir;
  struct dirent *entry;
  char file[1024];

  //WriteLog("int scan_model_directory()\n");
  if(root_adv_model != NULL)return -1;

  if ( NULL == (dir = opendir(MODELINIDIR)) ){
    WriteLog("scan_model_directory : No extension files  %s",MODELINIDIR);
  }
  else {
    while((entry = readdir(dir))){
      if(check_inifile_name(entry->d_name) == -1)continue;

      sprintf(file,"%s/%s",MODELINIDIR,entry->d_name);
      get_model_structure(file);
    }
    closedir(dir);
  }
  get_model_structure(MAININIFILE);

  return 0;
}



//------------------------------------
//
//
//------------------------------------
sane_model_info  get_model_info_from_ini_by_product_id(int id){
  static sane_model_info *current = NULL;
  static int pre_id=-1;

  if(id == pre_id && current != NULL){
    return *current;
  }
  scan_model_directory();
  current = root_adv_model;
  while(current){
    if(current->pid == id){
      pre_id = id;
      return *current;
    }
    current = current->next;
  }
  pre_id=-1;
  current = &ERRORINFO;
  return ERRORINFO;
}
sane_model_info  *get_p_model_info_from_ini_by_product_id(int id){
  static sane_model_info *current = NULL;
  static int pre_id=-1;

  if(id == pre_id && current != NULL){
    return current;
  }
  scan_model_directory();

  current = root_adv_model;
  while(current){
    if(current->pid == id){
      pre_id = id;
      return current;
    }
    current = current->next;
  }
  pre_id=-1;
  current = &ERRORINFO;
  return &ERRORINFO;
}


sane_model_info  *get_p_model_info_by_index(int index){
  static sane_model_info *current = NULL;
  static int pre_index;

  if(index == pre_index && current != NULL){
    return current;
  }
  pre_index = index;
  scan_model_directory();

  current = root_adv_model;
  while((index-- > 0) && current){
    current = current->next;
  }
  if(!current){
    pre_index = -1;
    current = &ERRORINFO;
  }
  return current;
}




int free_inifile_tree(){
  sane_model_info *current,*current_t = root_adv_model;

  while(current_t){
    current = current_t;
    current_t = current_t->next;
    FREE(current);
  }
  root_adv_model = NULL;
  return 0;
}





//---------------------------------------------

netini *root_netini=NULL;

int init_netini(){

  FILE *fp_ini;
  netini **next_netini=NULL;
  netini *current_netini;
  char line[32+256+2];
  int i,n,len;
  int num = 0;
  if ( NULL == (fp_ini = fopen(CONFFILE,"r") ) ){
    WriteLog("oops:inifile(%s) error:",CONFFILE);
    return -1;
  }
  while(fgets(line,sizeof(line)-(2*6)-1,fp_ini) != NULL){
    //reform_line(line);
    line[sizeof(line)-(2*6)-1] = 0;
    len = strlen(line);
    for (i = 0, n = 0; (line[i] != 0) && (n < 3) && (i < len); i ++){
      if(line[i] == ','){
	memmove(&line[i+3],&line[i+1],strlen(&line[i+1]) + 1 /*null char*/);
	memmove(&line[i]," , ",3);
	i += 2;
	n ++;
      }
    }
    switch(line[0]){
    case '#':
    case '\n':
    case '[':
      continue;
      break;
    default:
      if(strchr(line,'=') == NULL)continue;
      if(strncmp(line,"DEVICE=",strlen("DEVICE="))!=0){
	if(strncmp(line,"log=",strlen("log="))==0){
	  sscanf(line,"log=%s , %d , %c",g_logfile,&g_loglevel,&g_logmode);
	  continue;
	}
      }
      break;
    }
    if((current_netini = (netini *)CALLOC(sizeof(netini),1)) == NULL){
      return -1;
    }

    if      (strstr(line,"IP-ADDRESS=")){
      sscanf(line,IPDEVICEFORMAT,
	     current_netini->friendlyname,
	     current_netini->model,
	     current_netini->id,
	     current_netini->ip);
    }
    else if (strstr(line,"NODENAME=")){
      sscanf(line,NODEDEVICEFORMAT,
	     current_netini->friendlyname,
	     current_netini->model,
	     current_netini->id,
	     current_netini->node);
    }
    else{
      current_netini->friendlyname[0] = 0;
      current_netini->model[0] = 0;
      current_netini->node[0] = 0;
      current_netini->id[0] = 0;
    }



    if(root_netini == NULL){
      root_netini = current_netini;
      num = 1;
      // WriteLog("REG1   %x   %x\n",next_netini , current_netini);
    }
    else{
      *next_netini = current_netini;
      num ++;
      // WriteLog("REG2   %x   %x\n",next_netini , current_netini);
    }
    next_netini = &current_netini->next;
  }
  fclose(fp_ini);
  return num;
}


int get_net_device_num(){
  int i;
  netini *current_netini;
  if(root_netini == NULL)init_netini();
  current_netini = root_netini;
  i = 0;
  while(current_netini){
    i ++;
    current_netini = current_netini->next;
  }
  return i;
}

char *get_net_ini_value(int dev_index ,int key, char *value, int size){
  netini *current_netini;
  char *src = NULL;
  //  WriteLog("get_net_ini_value  key=%s",key);

  if(dev_index < 0){
    *value = 0;
    return NULL;
  }
  if(root_netini == NULL)init_netini();
  current_netini = root_netini;

  while(dev_index--){
    current_netini = current_netini->next;
    if(!current_netini)break;
  }

  if(current_netini){
    switch(key){
    case  KEY_DEVICE:
      src = current_netini->friendlyname;
      break;
    case  KEY_MODEL:
      src = current_netini->model;
      break;
    case  KEY_IP:
      src = current_netini->ip;
      break;
    case  KEY_NODE:
      src = current_netini->node;
      break;
    case  KEY_ID:
      src = current_netini->id;
      break;
    default:
      src = NULL;
      break;
    }
  }
  else{
    *value = 0;
  }
  if(src && *src){
    strncpy(value,src,size);
  }
  else{
    *value = 0;
    value = NULL;
  }
  return value;
}




int free_net_inifile_tree(){
  netini *current,*current_t = root_netini;

  while(current_t){
    current = current_t;
    current_t = current_t->next;
    FREE(current);
  }
  root_netini = NULL;
  return 0;
}



int ReadModelInfoSub(char *buff,int bufsize,int *psize){
  char line[512];

  int size = 0,record=0,len = 0;
  sane_model_info *current;

  if(root_adv_model == NULL){
    scan_model_directory();
  }


  current = root_adv_model;
  while(current){
    record ++;
    sprintf(line,"%d,0x%x,0x%x,%d,%d,%s",
// M-LNX-20
	    current->expanded_func_type,
// M-LNX-20
	    current->vid,
	    current->pid,
	    current->func_type,
	    current->model_type,
	    current->model_name
	    );
    //    len = strlen(line) + sizeof('\0');
    len = strlen(line) + 1;
    if (buff){
      if(bufsize >= size + len){
	strcpy(buff + size, line);
      }
    }
    size += len;
    current = current->next;
  }
  if(psize)*psize = size;
#ifdef DEBUG_ADV
 if(buff){
   int i,len;
   char *p;
   FILE *fp_debugadv;
   if (NULL != (fp_debugadv = fopen("/tmp/debugadv","w") ) ){
     fwrite(buff,size,1,fp_debugadv);
     fclose(fp_debugadv);
   }
   p = buff;
   for (i = 0; i < record; i ++){
     printf("DEBUG_ADV:%s\n",p);
     len = strlen(p);
     p += len +1;
   }
 }
#endif
  return record;
}



int ReadModelInfoSize2(int *size, int *record){

  *record = ReadModelInfoSub(NULL,0,size);
  //#ifdef DEBUGMAIN
  //printf("*record(%d) = ReadModelInfoSub(0,0,&size(%d))\n",*record , *size);
  //#endif
  return 1;
}

int ReadModelInfo2(char *lpReturnedString,int nSize){

  ReadModelInfoSub(lpReturnedString,nSize,NULL);
  //#ifdef DEBUGMAIN
  //printf("ReadModelInfoSub(lpReturnedString(%s),nSize(%d),NULL);\n",lpReturnedString,nSize);
  //#endif
  return 1;
}



int debug_disp_model_info(){
  sane_model_info *current = root_adv_model;

  while(current){
    printf(
        "MODEL:%s,ID:0x%x:0x%x,TYPE:%d,%d,RE:0x%x,WE:0x%x CM:%s,%s,%s\n",
	current->model_name,
	current->vid,
	current->pid,
	current->func_type,
	current->model_type,
	current->r_endpoint,
	current->w_endpoint,
	current->colmatchDL,
	current->colmatchTBL,
	current->graylevelTBL
	);
    current = current->next;
  }
  return 0;
}


char *get_net_ini_value_by_name(char *name ,int key, char *value, int size){
  netini *current_netini;
  char *src = NULL;
  if(root_netini == NULL)init_netini();
  current_netini = root_netini;
  if(current_netini->friendlyname == NULL || 
     current_netini->friendlyname[0] == 0){
    return NULL;
  }
  while(strcmp(name,current_netini->friendlyname)){
    current_netini = current_netini->next;
    if(!current_netini)break;
  }
  if(current_netini){
    switch(key){
    case  KEY_DEVICE:
      src = current_netini->friendlyname;
      break;
    case  KEY_MODEL:
      src = current_netini->model;
      break;
    case  KEY_IP:
      src = current_netini->ip;
      break;
    case  KEY_NODE:
      src = current_netini->node;
      break;
    case  KEY_ID:
      src = current_netini->id;
      break;
    default:
      src = NULL;
      break;
    }
  }
  else{
    *value = 0;
  }
  if(src && *src){
    strncpy(value,src,size);
  }
  else{
    *value = 0;
    value = NULL;
  }
  return value;
}





//------------------ for debug---------------------------
#ifdef DEBUGMAIN


char *debug_disp_netini(){
  netini *current_netini;

  if(root_netini == NULL)init_netini();
  current_netini = root_netini;

  while(current_netini){
    printf("%s %s %s %s %s\n",
	   current_netini->friendlyname,
	   current_netini->model,
	   current_netini->ip,
	   current_netini->node,
	   current_netini->id
	   );
    current_netini = current_netini->next;
  }
  return NULL;
}

int main(){
  char value[100];
  sane_model_info target;

  printf("debug_disp\n");

  printf("0x0333      %s %d\n",
	   (get_p_model_info_by_index(0))->model_name,
	   (get_p_model_info_by_index(1))->func_type);
  printf("0x0174      %s %d\n",
	   (get_p_model_info_from_ini_by_product_id(0x0174))->model_name,
	   (get_p_model_info_from_ini_by_product_id(0x0174))->func_type);

  target = get_model_info_from_ini_by_product_id(0x0333);
  printf("0x0333      %s %d\n",
	   target.model_name,
	   target.func_type);


  target = get_model_info_from_ini_by_product_id(0x0174);
  printf("0x0174      %s %d\n",
	   target.model_name,
	   target.func_type);

  
  debug_disp_model_info();

  debug_disp_netini();

  printf("1 %s\n",get_net_ini_value(1,KEY_IP,value,sizeof(value)) );
  printf("2 %s\n",get_net_ini_value(0,KEY_NODE,value,sizeof(value)) );


  free_net_inifile_tree();
  free_inifile_tree();
  return 0;
}


#endif


