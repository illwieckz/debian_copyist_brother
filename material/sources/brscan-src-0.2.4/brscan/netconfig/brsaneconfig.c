//* This program is free software; you can redistribute it and/or modify it
//* under the terms of the GNU General Public License as published by the Free
//* Software Foundation; either version 2 of the License, or (at your option)
//* any later version.
//*
//* This program is distributed in the hope that it will be useful, but WITHOUT
//* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//* more details.
//*
//* You should have received a copy of the GNU General Public License along with
//* this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//* Place, Suite 330, Boston, MA  02111-1307  USA
//*


//#define  SUPPORT_INTERACTIVE


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <curses.h>
#include <unistd.h>

#include "brother_advini.h"


extern  sane_model_info *root_adv_model;


/*
**    brsaneconfig
**      configureing program for brsane
**
**
*/

#define OPTION_S   (('-'<<16) | ('s'<<8) | ':')
#define OPTION_L   (('-'<<16) | ('l'<<8) | ':')
#define OPTION_Q   (('-'<<8)  |  'q')
#define OPTION_A   (('-'<<8)  |  'a')
#define OPTION_R   (('-'<<8)  |  'r')
#define OPTION_D   (('-'<<8)  |  'd')
#define OPTION_H   (('-'<<8)  |  'h')
#define OPTION_I   (('-'<<8)  |  'i')
#define OPTION_P   (('-'<<8)  |  'p')
#define OPTION_H2  (('-'<<8)  |  'H')
#define OPTION_H3  (('-'<<8)  |  '?')
#define OPTION_LI  (('-'<<16) | ('L'<<8) | 'I')
#define OPTION_LO  (('-'<<16) | ('L'<<8) | 'O')
#define OPTION_NUL 0


#include "resource.h"









#define SAVECONFIG 0
#define LOADCONFIG 1

int check_model_name_available(char *model){
  sane_model_info *current;

  current = root_adv_model;
  while(current){
    if(strcmp(current->model_name,model) == 0){
      return 1;
    }
    current = current->next;
  }
  puts(INVALIDMODELNAME);
  return 0;
}
int get_ids_with_modelname(char *model,int *vid, int *pid){
 sane_model_info *current;
  int rc = 0;
  current = root_adv_model;
  while(current){
    if(strcmp(current->model_name,model) == 0){
      *vid = current->vid;
      *pid = current->pid;
      rc = 1;
      break;
    }
    current = current->next;
  }
  return rc;

}

int get_1_element_from_arg(char **arg,int argc,char *label,char *buffer,int size){
  char *p,*pp,c;
  int i,rc = -1;
  if(size && buffer){
    for ( i=0 ; i< argc ; i++){
      if((p=strstr(arg[i],label)) ){
	if(arg[i] != p)continue;
	pp = buffer;
	p += strlen(label);
	size --;
	while(size--){
	  c = *p++;
	  if(c == '\0' || c ==  ' ' || c == '\t' )break;
	  *pp++ = c;
	}
	*pp = 0;
	if(size > 0)rc = 0;
	break;
      }
    }
    return rc;
  }
  else{
    return -1;
  }
  return 0; //dummy
}
int check_friendly_name(char *friendly){
  char c;
  if(strlen(friendly)==0)return 0;
  if ( (strchr(friendly,' ') || strchr(friendly,'\t'))) return 0;
  while((c=*friendly)){
    if( !(
       ((c >= '0') && (c <= '9')) ||
       ((c >= 'a') && (c <= 'z')) ||
       ((c >= 'A') && (c <= 'Z')) ||
       (c == '-')                 ||
       (c == '_')               ))return 0;
    friendly ++;
  }
  return 1;
}



int get_config_from_commandline(char **arg,  int argc,
				char *friendly,int nfriendly,
				char *model,int nmodel,
				char *port, int nport,
				char *ip,   int nip,
				char *node, int nnode,
				char *id,   int nid){
  int  vid,pid,ret;
  char check[200];
  if(nid <= (int)sizeof("0xnnnn:0xnnnn"))return -1;
  if(nport  <= 4 )return -1;
  if(nnode  <= 16)return -1;
  if(nmodel <= 32)return -1;
  scan_model_directory();
  *ip = *model = *port = *node = *id =0;

  get_1_element_from_arg(arg,argc,"name=",friendly,nfriendly);
  get_1_element_from_arg(arg,argc,"model=",model+1,nmodel-2);
  get_1_element_from_arg(arg,argc,"ip=",ip,nip);
  get_1_element_from_arg(arg,argc,"nodename=",node,nnode);


  if(!check_friendly_name(friendly)){
    puts(INVALIDFNAME);
    return -1;
  }

  if(get_net_ini_value_by_name(friendly ,KEY_DEVICE, check, 
			       sizeof(check)) != NULL){
    printf(INVALIDFNAME2,friendly);
    return -1;
  }

  if(*ip == 0 && *node == 0){
    puts(INVALIDADDRESS);
    return -1;
  }
  else{
    strcpy(port , "NET");
  }
  *model = '\"';
  strcat(model,"\"");
  if(check_model_name_available(model)){
    get_ids_with_modelname(model,&vid,&pid);
    sprintf(id,"0x%x:0x%x",vid,pid);
    ret = 0;
  }
  else{
    ret = -1;
  }
  free_inifile_tree();

  return ret;
}

int check_ipaddress_is_valid(char *ip){
  char c;
  int len;
  int  dot = 0;
  int  ip1,ip2,ip3,ip4;
  sscanf(ip,"%d.%d.%d.%d",&ip1,&ip2,&ip3,&ip4);

  if(ip1 > 255 || ip1 < 0)return 0;
  if(ip2 > 255 || ip2 < 0)return 0;
  if(ip3 > 255 || ip3 < 0)return 0;
  if(ip4 > 255 || ip4 < 0)return 0;

  len = strlen(ip);
  if(len < 7)return 0;
  if(strstr(ip,".."))return 0;
  if(*ip == '.')return 0;
  if( *(ip + len -1) == '.' )return 0;

  while((c=*ip++)){
    if(c == '0' && c == '\n')break;
    if(c == '.' ){
      dot ++;
      continue;
    }
    if( (c > '9' ||  c < '0') &&  (c != '.')  )      return 0;    
  }
  if(dot != 3)return 0;
  return 1;
}



int copy_file(char *src, char *dst,char *mode){
  FILE *fp_src=NULL,*fp_dst=NULL;
  char c;
  fp_src = fopen(src , "r");
  if(fp_src == NULL)return 0;

  fp_dst = fopen(dst , mode);
  if(fp_src != NULL && fp_dst != NULL){
    while(EOF != (c = fgetc(fp_src))){
      fputc(c,fp_dst);
    }
  }
  if(fp_src)fclose(fp_src);
  if(fp_dst)fclose(fp_dst);
  return (fp_src != NULL && fp_dst != NULL);
}

int add_network_device(char **arg,int argc){
  int rc;
  FILE *fp;
  char c;
  char model[NAMELEN],port[100], ip[30],node[NAMELEN],id[32],friendly[NAMELEN];
  char uniq[100];
  char tmpconffile[32]=BRSANETMPFILE;
  int tmp;

  tmp = mkstemp(tmpconffile);
  if(tmp == -1)return -1;
  else close(tmp);

  if(argc >= 5  ){
    rc = get_config_from_commandline(arg,argc,
				     friendly,sizeof(friendly),
				     model,sizeof(model),
				     port,sizeof(port),
				     ip,sizeof(ip),
				     node,sizeof(node),
				     id,sizeof(id));
  }
#ifdef SUPPORT_INTERACTIVE
  else{
    rc = get_config_from_by_ui(
			       friendly,sizeof(friendly),
			       model,sizeof(model),
			       port,sizeof(port),
			       ip,sizeof(ip),
			       node,sizeof(node),
			       id,sizeof(id));
  }
#else  //SUPPORT_INTERACTIVE
  else{
    printf(INVALIDPARAMETERN,0);
    return 0;
  }
#endif  //SUPPORT_INTERACTIVE


#ifdef _DEBUG
  printf("set_current_config %s %s %s %s %s   %d\n",
	 model,port,ip,node,id,rc);
#endif

  if(rc == -1){
    return 1;
  }
  reform_line(model);
  reform_line(port);
  reform_line(ip);
  reform_line(node);
  reform_line(id);

  if(*ip == 0 || check_ipaddress_is_valid(ip)){
    if(arg == NULL){
      printf("Save this setting (Y/N)   ->");
      c = getchar(); getchar();
    }
    else{
      c = 'Y';
    }
    if(c == 'Y' || c == 'y'){
      fp = fopen(tmpconffile , "w");
      if(fp == NULL){
	return -1;
      }
      if(node && *node){
	fprintf(fp,NODEDEVICEFORMAT,
		friendly,model,id,node);
      }
	 
      else if(ip && *ip){
	fprintf(fp,IPDEVICEFORMAT,
		friendly,model,id,ip);
      }
      fprintf(fp,"\n");
      fclose(fp);
      copy_file(CONFFILE ,tmpconffile ,"a");
      sprintf(uniq,"uniq %s > %s",tmpconffile,CONFFILE);
      system(uniq);
    }
  }
  else  if(*ip != 0 || !check_ipaddress_is_valid(ip)){
    printf(INVALIDADDRESS2,ip);
  }
  else {
    printf(INVALIDPARAMETERN,100);
  }
  unlink(tmpconffile);
  return 1;
}

int remove_network_device(char *label[], int n){
#if 0   //M-LNX-37
  char filter[100];
#else   //M-LNX-37
  char filter[512];
#endif   //M-LNX-37
  char tmpconffile[32]=BRSANETMPFILE;
  int tmp;
  int i;
  
  tmp = mkstemp(tmpconffile);
  if(tmp == -1)return -1;
  else close(tmp);

  if(n <= 1)return -1;

  for (i = 1 ; i <= n-1 ; i ++){
    copy_file(CONFFILE ,tmpconffile ,"w");
    sprintf(filter,"cat %s | sed s/'DEVICE=%s .*$'// | uniq > %s",
	  tmpconffile,label[i],CONFFILE);
    system(filter);
    unlink(tmpconffile);
  }
  return 0;
}

int save_load_current_config(int mode ,char *file){
  switch(mode){
  case SAVECONFIG:
    return copy_file(CONFFILE,file,"w");
    break;
  case LOADCONFIG:
    return copy_file(file,CONFFILE,"w");
    break;
  }
  return 0; //dummy
}


#define TAB 20

int query_available_models(){
  int i = 0,dev_num,ii;
  char name[64],model[64],ip[64],node[64];
  int  iname,imodel;
  sane_model_info *current;

  scan_model_directory();
  current = root_adv_model;
  while(current){
    printf("%3d %s\n",i,current->model_name);
    current = current->next;
    i ++;
  }

  free_inifile_tree();

  dev_num =  get_net_device_num();
  printf("\nDevices on network\n");
  for (i = 0; i<dev_num; i++){
    get_net_ini_value(i,KEY_DEVICE,name,sizeof(name));
    get_net_ini_value(i,KEY_MODEL ,model,sizeof(model));
    get_net_ini_value(i,KEY_IP ,ip,sizeof(ip));
    get_net_ini_value(i,KEY_NODE ,node,sizeof(node));

    iname = imodel = 0; 
    if(name[0] )iname  =strlen(name);
    if(model[0])imodel =strlen(model);


    printf("%3d %s",i,name);
    ii = 1;

    if(iname < (TAB-1))ii = TAB - iname;
    while(ii--)putchar(' ');

    printf("%s",model);
    ii = 1;
    if(imodel < (TAB-1))ii = TAB - imodel;
    while(ii--)putchar(' ');

    if(node && node[0]){
      printf("N:%s",node);
    }
    else if(ip && ip[0]){
      printf("I:%s",ip);
    }
    printf("\n");
  }
  return 1;
}

int set_sane_initial_config(){
  return 1;
}


int  system2(char *command){
  puts(command);
  fflush(stdout);
  return system(command);
}


//
//
//
int test_ping(){
  char value[64],ss[128];
  *value = 0;
  int i;
  i = get_net_device_num();
  while(i--){
    get_net_ini_value(i,KEY_DEVICE,value,sizeof(value));
    printf("test %s\n",value);
    if(!get_net_ini_value(i,KEY_NODE,value,sizeof(value))){
        get_net_ini_value(i,KEY_IP,value,sizeof(value));
    }
    if(*value){
      sprintf(ss,"ping %s -w 10\n",value);
      system2(ss);
    }
  }
  return 1;
}



int scan_and_cat_for_diagnosis(){
  DIR *dir;
  struct dirent *entry;
  char command[1024];
  
  if(root_adv_model != NULL)return -1;

  if ( NULL != (dir = opendir(MODELINIDIR)) ){

    printf("-----------------------------\n%s:\n",CONFFILE);
    fflush(stdout);
    sprintf(command,"cat %s",CONFFILE);
    system(command);


    printf("-----------------------------\n%s:\n",MAININIFILE);
    fflush(stdout);
    sprintf(command,"cat %s",MAININIFILE);
    system(command);


    while((entry = readdir(dir))){
      if(check_inifile_name(entry->d_name) == -1)continue;
      printf("-----------------------------\n%s/%s:\n",
	     MODELINIDIR,  entry->d_name);
      fflush(stdout);
      sprintf(command,"cat %s/%s",MODELINIDIR,entry->d_name);
      system(command);
    }
    closedir(dir);
  }
  sprintf(command,"cat %s",MAININIFILE);
  return 0;
}


int brsane_diagnosis(){
  puts("-----------------------------");
  fflush(stdout);
  system2("cat /etc/fstab");
  puts("-----------------------------");
  fflush(stdout);
  system2("sane-find-scanner");
  puts("-----------------------------");
  fflush(stdout);
  system2("ls -R -all /proc/bus/usb");
  puts("-----------------------------");
  fflush(stdout);
  system2("cat /proc/bus/usb/devices");
  puts("-----------------------------");
  fflush(stdout);
  system2("scanimage -L");
  puts("-----------------------------");
  fflush(stdout);
  scan_and_cat_for_diagnosis();
  puts("-----------------------------");
  scan_model_directory();
  debug_disp_model_info();
  puts("-----------------------------");
  query_available_models();
  puts("ping");
  test_ping();
  return 1;
}
//
//
//
int display_usage(){
  char **p = &HELP[1];
  printf(HELP[0],S_BRSANESUFFIX);
  while(*p){
    puts(*p);
    p ++;
  }
  return 1;
}
//
//
int make_filename(char *arg,char *file,int size){

  if(size < (int)sizeof(SANESAVEFILE) + (int)strlen(CONFDIR) )return -1;

  while(*arg == ' '|| *arg == '\t')arg ++;
  strcpy(file,CONFDIR);
  strcat(file, arg + sizeof("-S:") -1);
  strcat(file,SANESAVEFILE);  
  return 1;
}


int loggingon(){
  FILE *fp;
  char uniq[100];
  char tmpconffile[32]=BRSANETMPFILE;
  int tmp;

  tmp = mkstemp(tmpconffile);
  if(tmp == -1)return -1;
  else close(tmp);

  fp = fopen(tmpconffile , "a");
  fputs("log=/tmp/br_sane.log , 1 , w\n",fp);
  fclose(fp);
  copy_file(CONFFILE ,tmpconffile ,"a");
  sprintf(uniq,"uniq %s > %s",tmpconffile,CONFFILE);
  system(uniq);
  unlink(tmpconffile);
  return 0;
}

int loggingoff(){
  char filter[100];
  char tmpconffile[32]=BRSANETMPFILE;
  int tmp;

  tmp = mkstemp(tmpconffile);
  if(tmp == -1)return -1;
  else close(tmp);

  copy_file(CONFFILE ,tmpconffile ,"a");
  sprintf(filter,"cat %s | sed s/'log=.*'// | uniq > %s",tmpconffile,CONFFILE);
  system(filter);
  unlink(tmpconffile);
  return 0;
}



int main(int argc , char *argv[]){
  int option = 0;
  char file[512];
  if(argc > 1){
    option = ((argv[1][0]&0xff) << 8) | ((argv[1][1]&0xff) << 0) ;
    if(argv[1][2] != 0){
      option <<= 8;
      option |= ((argv[1][2]&0xff) << 0) ;
    }
  }
  else{
    option = 0;
  }
  
  //printf("option = %x\n",option);
  switch(option){
  case OPTION_S:
    scan_model_directory();
    make_filename(argv[1],file,(int)sizeof(file));
    if(save_load_current_config(SAVECONFIG,file) == 0){
      puts(INVALIDPARAMETER);
    }
    break;
  case OPTION_L:
    make_filename(argv[1],file,(int)sizeof(file));
    if(save_load_current_config(LOADCONFIG,file) == 0){
      puts(INVALIDPARAMETER);
    }
    break;
  case OPTION_Q:
    query_available_models();
    break;
  case OPTION_A:
    add_network_device(argv,argc);
    break;
  case OPTION_R:
    remove_network_device(argv,argc);
    break;
  case OPTION_I:
    set_sane_initial_config(NULL);
    break;
  case OPTION_D:
    brsane_diagnosis();
    break;
  case OPTION_P:
    test_ping();
    break;
  case OPTION_H:
  case OPTION_H2:
  case OPTION_H3:
    display_usage();
    break;
  case OPTION_LI:
    loggingon();
    break;
  case OPTION_LO:
    loggingoff();
    break;
  case OPTION_NUL:
  default:
    display_usage();
    break;
  }

  return 0;
}







#ifdef SUPPORT_INTERACTIVE
int get_config_from_by_ui(
				char *friendly,int nfriendly,
				char *model,int nmodel,
				char *port, int nport,
				char *ip,   int nip,
				char *node, int nnode,
				char *id,   int nid){
  char c,*p;
  int  vid,pid;
  if(nid <= (int)sizeof("0xnnnn:0xnnnn"))return -1;
  if(nport  <= 4 )return -1;
  if(nnode  <= 16)return -1;
  if(nmodel <= 32)return -1;
  
  *model = 0;
  *port = 0;
  *ip = 0;
  *node = 0; 
  *id = 0;

  scan_model_directory();
  while(1){
    printf("Input friendly name ->");
    fgets(friendly,nfriendly,stdin);
    if(!check_friendly_name(friendly)){
      puts("Invalid friendly name");
      continue;
    }
    if((p=strchr(friendly,'\n')))*p=0;
    break;
  }
  while(1){
    printf("Input model name ->");
    model[0] = '\"';
    fgets(model+1,nmodel-2,stdin);

    reform_line(model);
    strcat(model,"\"");
    if(check_model_name_available(model)){
      break;
    }
  }
  printf("Select the network address type\n");
  printf("     I: Specified by IP address\n");
  printf("     N: Specified by Node name\n");
  printf("   ->");
  while(1){
    c = getchar(); getchar();
    if ( c == 'I' || c == 'i' ||
	 c == 'N' || c == 'n' )break;
  }
  switch(c){
  case 'I':
  case 'i':
    strncpy(port,"NET",nport);
    while(1){
      printf("Input IP address of your MFP device:");
      fgets(ip,nip,stdin);
      if((p=strchr(ip,'\n')))*p=0;
      if(check_ipaddress_is_valid(ip))break;
    }
    break;
  case 'N':
  case 'n':
    strncpy(port,"NET",nport);
    while(1){
      printf("Input NODENAME of your MFP device:");
      fgets(node,nnode,stdin);
      if(strlen(node) > 3)break;
    }
    break;
  }
  get_ids_with_modelname(model,&vid,&pid);
  sprintf(id,"0x%x:0x%x",vid,pid);
  free_inifile_tree();
  return 1;
}
#endif
